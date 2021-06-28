#include <3ds.h>
#include <string>
#include "thread.hpp"

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

Result downloadToFile(const std::string &url, const std::string &path);
Result downloadFromRelease(const std::string &url, const std::string &asset, const std::string &path, bool includePrereleases);

const std::string &lurl = "";
const std::string &lpath = "";

void Download()
{
    while (true)
    {
        downloadToFile(lurl, lpath);
        break;
        RenderD7::Thread::sleep(1000 * 3);
    }
}

void StartDownload(const std::string &url, const std::string &path)
{
    lurl = url;
    lpath = path;
    RenderD7::Thread dl(Download);
    dl.start();

}
