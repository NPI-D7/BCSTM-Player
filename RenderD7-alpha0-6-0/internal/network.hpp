#include <3ds.h>
#include <string>

#define APP_TITLE "Nightly-Downloader"
#define VERSION_STRING "1.0.0"

enum DownloadError {
	DL_ERROR_NONE = 0,
	DL_ERROR_WRITEFILE,
	DL_ERROR_ALLOC,
	DL_ERROR_STATUSCODE,
	DL_ERROR_GIT,
	DL_CANCEL, // No clue if that's needed tho.
};

void Download(const std::string &url, const std::string &path)
{
    while (true)
    {
        downloadToFile(url, path);
        break;
    }
}
Result downloadToFile(const std::string &url, const std::string &path);
Result downloadFromRelease(const std::string &url, const std::string &asset, const std::string &path, bool includePrereleases);