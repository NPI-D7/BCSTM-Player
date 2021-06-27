#include <3ds.h>
#include <curl/curl.h>
#include <dirent.h>
#include <malloc.h>
#include <regex>
#include <string>
#include <unistd.h>
#include <vector>

#include "network.hpp"
#define USER_AGENT APP_TITLE "-" VERSION_STRING

static char *result_buf = nullptr;
static size_t result_sz = 0;
static size_t result_written = 0;

#define TIME_IN_US 1
#define TIMETYPE curl_off_t
#define TIMEOPT CURLINFO_TOTAL_TIME_T
#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL	3000000

curl_off_t downloadTotal = 1; // Dont initialize with 0 to avoid division by zero later.
curl_off_t downloadNow = 0;
curl_off_t downloadSpeed = 0;

static FILE *downfile = nullptr;
static size_t file_buffer_pos = 0;
static size_t file_toCommit_size = 0;
static char *g_buffers[2] = { nullptr };
static u8 g_index = 0;
static Thread fsCommitThread;
static LightEvent readyToCommit;
static LightEvent waitCommit;
static bool killThread = false;
static bool writeError = false;
#define FILE_ALLOC_SIZE 0x60000
CURL *CurlHandle = nullptr;

static int curlProgress(CURL *hnd,
					curl_off_t dltotal, curl_off_t dlnow,
					curl_off_t ultotal, curl_off_t ulnow)
{
	downloadTotal = dltotal;
	downloadNow = dlnow;

	return 0;
}

bool filecommit() {
	if (!downfile) return false;
	fseek(downfile, 0, SEEK_END);
	u32 byteswritten = fwrite(g_buffers[!g_index], 1, file_toCommit_size, downfile);
	if (byteswritten != file_toCommit_size) return false;
	file_toCommit_size = 0;
	return true;
}

static void commitToFileThreadFunc(void *args) {
	LightEvent_Signal(&waitCommit);

	while (true) {
		LightEvent_Wait(&readyToCommit);
		LightEvent_Clear(&readyToCommit);
		if (killThread) threadExit(0);
		writeError = !filecommit();
		LightEvent_Signal(&waitCommit);
	}
}

static size_t file_handle_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	//if (getAvailableSpace() < (u64)downloadTotal) return 0; // Out of space.
	if (writeError) return 0;
	//if (QueueSystem::CancelCallback) return 0;

	(void)userdata;
	const size_t bsz = size * nmemb;
	size_t tofill = 0;


	if (!g_buffers[g_index]) {
		LightEvent_Init(&waitCommit, RESET_STICKY);
		LightEvent_Init(&readyToCommit, RESET_STICKY);

		s32 prio = 0;
		svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
		fsCommitThread = threadCreate(commitToFileThreadFunc, NULL, 0x1000, prio - 1, -2, true);

		g_buffers[0] = (char*)memalign(0x1000, FILE_ALLOC_SIZE);
		g_buffers[1] = (char*)memalign(0x1000, FILE_ALLOC_SIZE);

		if (!fsCommitThread || !g_buffers[0] || !g_buffers[1]) return 0;
	}

	if (file_buffer_pos + bsz >= FILE_ALLOC_SIZE) {
		tofill = FILE_ALLOC_SIZE - file_buffer_pos;
		memcpy(g_buffers[g_index] + file_buffer_pos, ptr, tofill);

		LightEvent_Wait(&waitCommit);
		LightEvent_Clear(&waitCommit);
		file_toCommit_size = file_buffer_pos + tofill;
		file_buffer_pos = 0;
		svcFlushProcessDataCache(CUR_PROCESS_HANDLE, (u32)g_buffers[g_index], file_toCommit_size);
		g_index = !g_index;
		LightEvent_Signal(&readyToCommit);
	}

	memcpy(g_buffers[g_index] + file_buffer_pos, ptr + tofill, bsz - tofill);
	file_buffer_pos += bsz - tofill;
	return bsz;
}

/*
	Download a file.
	const std::string &url: The download URL.
	const std::string &path: Where to place the file.
*/
Result downloadToFile(const std::string &url, const std::string &path) {
	//if (!checkWifiStatus()) return -1; // NO WIFI.

	bool needToDelete = false;
	downloadTotal = 1;
	downloadNow = 0;
	downloadSpeed = 0;

	CURLcode curlResult;
	Result retcode = 0;
	int res;

	printf("Downloading from:\n%s\nto:\n%s\n", url.c_str(), path.c_str());

	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		retcode = -1;
		goto exit;
	}

	res = socInit((u32 *)socubuf, 0x100000);
	if (R_FAILED(res)) {
		retcode = res;
		goto exit;
	}

	/* make directories. */
	for (char *slashpos = strchr(path.c_str() + 1, '/'); slashpos != NULL; slashpos = strchr(slashpos + 1, '/')) {
		char bak = *(slashpos);
		*(slashpos) = '\0';

		mkdir(path.c_str(), 0777);

		*(slashpos) = bak;
	}

	downfile = fopen(path.c_str(), "wb");
	if (!downfile) {
		retcode = -2;
		goto exit;
	}

	CurlHandle = curl_easy_init();
	curl_easy_setopt(CurlHandle, CURLOPT_BUFFERSIZE, FILE_ALLOC_SIZE);
	curl_easy_setopt(CurlHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(CurlHandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(CurlHandle, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(CurlHandle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(CurlHandle, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(CurlHandle, CURLOPT_ACCEPT_ENCODING, "gzip");
	curl_easy_setopt(CurlHandle, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(CurlHandle, CURLOPT_XFERINFOFUNCTION, curlProgress);
	curl_easy_setopt(CurlHandle, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(CurlHandle, CURLOPT_WRITEFUNCTION, file_handle_data);
	curl_easy_setopt(CurlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(CurlHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(CurlHandle, CURLOPT_STDERR, stdout);

	curlResult = curl_easy_perform(CurlHandle);
	curl_easy_cleanup(CurlHandle);
	CurlHandle = nullptr;

	if (curlResult != CURLE_OK) {
		retcode = -curlResult;
		needToDelete = true;
		goto exit;
	}

	LightEvent_Wait(&waitCommit);
	LightEvent_Clear(&waitCommit);

	file_toCommit_size = file_buffer_pos;
	svcFlushProcessDataCache(CUR_PROCESS_HANDLE, (u32)g_buffers[g_index], file_toCommit_size);
	g_index = !g_index;

	if (!filecommit()) {
		retcode = -3;
		needToDelete = true;
		goto exit;
	}

	fflush(downfile);

exit:
	if (fsCommitThread) {
		killThread = true;
		LightEvent_Signal(&readyToCommit);
		threadJoin(fsCommitThread, U64_MAX);
		killThread = false;
		fsCommitThread = nullptr;
	}

	socExit();

	if (socubuf) free(socubuf);

	if (downfile) {
		fclose(downfile);
		downfile = nullptr;
	}

	if (g_buffers[0]) {
		free(g_buffers[0]);
		g_buffers[0] = nullptr;
	}

	if (g_buffers[1]) {
		free(g_buffers[1]);
		g_buffers[1] = nullptr;
	}

	g_index = 0;
	file_buffer_pos = 0;
	file_toCommit_size = 0;
	writeError = false;

	/*if (needToDelete) {
		if (access(path.c_str(), F_OK) == 0) deleteFile(path.c_str()); // Delete file, cause not fully downloaded.
	}*/

	//if (QueueSystem::CancelCallback) return 0;
	return retcode;
}

/*
	following function is from
	https://github.com/angelsl/libctrfgh/blob/master/curl_test/src/main.c
*/
/*static size_t handle_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(void)userdata;
	const size_t bsz = size*nmemb;

	if (result_sz == 0 || !result_buf) {
		result_sz = 0x1000;
		result_buf = (char *)malloc(result_sz);
	}

	bool need_realloc = false;
	while (result_written + bsz > result_sz) {
		result_sz <<= 1;
		need_realloc = true;
	}

	if (need_realloc) {
		char *new_buf = (char *)realloc(result_buf, result_sz);
		if (!new_buf) return 0;

		result_buf = new_buf;
	}

	if (!result_buf) return 0;

	memcpy(result_buf + result_written, ptr, bsz);
	result_written += bsz;
	return bsz;
}*/

/*
	This + Above is Used for No File Write and instead into RAM.
*/
/*static Result setupContext(CURL *hnd, const char *url) {
	curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
	curl_easy_setopt(hnd, CURLOPT_URL, url);
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, USER_AGENT);
	curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, handle_data);
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(hnd, CURLOPT_STDERR, stdout);

	return 0;
}*/

/*
	Download a file of a GitHub Release.
	const std::string &url: Const Reference to the URL. (https://github.com/Owner/Repo)
	const std::string &asset: Const Reference to the Asset. (File.filetype)
	const std::string &path: Const Reference, where to store. (sdmc:/File.filetype)
	bool includePrereleases: If including Pre-Releases.
*/
/*Result downloadFromRelease(const std::string &url, const std::string &asset, const std::string &path, bool includePrereleases) {
	Result ret = 0;
	CURL *hnd;

	void *socubuf = memalign(0x1000, 0x100000);
	if (!socubuf) {
		return -1;
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret)) {
		free(socubuf);
		return ret;
	}

	std::regex parseUrl("github\\.com\\/(.+)\\/(.+)");
	std::smatch result;
	regex_search(url, result, parseUrl);

	std::string repoOwner = result[1].str(), repoName = result[2].str();

	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repoOwner << "/" << repoName << (includePrereleases ? "/releases" : "/releases/latest");
	std::string apiurl = apiurlStream.str();

	printf("Downloading latest release from repo:\n%s\nby:\n%s\n", repoName.c_str(), repoOwner.c_str());
	printf("Crafted API url:\n%s\n", apiurl.c_str());

	hnd = curl_easy_init();

	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return ret;
	}

	CURLcode cres = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
	char *newbuf = (char *)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; // nullbyte to end it as a proper C style string.

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = nullptr;
		result_sz = 0;
		result_written = 0;
		return -1;
	}

	printf("Looking for asset with matching name:\n%s\n", asset.c_str());
	std::string assetUrl;

	if (nlohmann::json::accept(result_buf)) {
		nlohmann::json parsedAPI = nlohmann::json::parse(result_buf);

		if (parsedAPI.size() == 0) ret = -2; // All were prereleases and those are being ignored.

		if (ret != -2) {
			if (includePrereleases) parsedAPI = parsedAPI[0];

			if (parsedAPI["assets"].is_array()) {
				for (auto jsonAsset : parsedAPI["assets"]) {
					if (jsonAsset.is_object() && jsonAsset["name"].is_string() && jsonAsset["browser_download_url"].is_string()) {
						std::string assetName = jsonAsset["name"];

						if (ScriptUtils::matchPattern(asset, assetName)) {
							assetUrl = jsonAsset["browser_download_url"];
							break;
						}
					}
				}
			}
		}

	} else {
		ret = -3;
	}

	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = nullptr;
	result_sz = 0;
	result_written = 0;

	if (assetUrl.empty() || ret != 0) ret = DL_ERROR_GIT;
	else ret = downloadToFile(assetUrl, path);

	return ret;
}*/