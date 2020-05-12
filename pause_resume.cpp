#include <iostream>
#include <string>
#include <thread>

#include <updaterInterface.h>

const std::string baseUrl("https://gitlab.com/ronme/updateexample/-/raw/master/");
#if _MSC_FULL_VER > 0
const std::string infoUrl(baseUrl + "appUpdateSample_Win.xml");
#elif defined(__APPLE__)
const std::string infoUrl(baseUrl + "appUpdateSample_MacOS.xml");
#elif defined(__linux__)
const std::string infoUrl(baseUrl + "appUpdateSample_Linux.xml");
#endif

void onResultEvent(UPDATER_PTR updater, OperationType o, Result r, const ExtraInfo &i)
{
    switch (o)
    {
    case TYPE_DOWNLOAD_INFO:
    {
        std::cout << "TYPE_DOWNLOAD_INFO :" << r << std::endl;
        break;
    }
    case TYPE_CHECK_UPDATE_VERSION:
    {
        std::cout << "TYPE_CHECK_UPDATE_VERSION : " << r << std::endl;
        break;
    }
    case TYPE_DOWNLOAD_UPDATE:
    {
        if (r == RESULT_INPROGRESS)
        {
            if (i.infoLength == sizeof(DownloadBytesInfo))
            {
                DownloadBytesInfo progressInfo;
                memcpy(&progressInfo, i.info, i.infoLength);
                std::cout << "Downloading update progress: "
                          << progressInfo.current << " / " << progressInfo.total << std::endl;
            }
        }

        std::cout << "TYPE_DOWNLOAD_UPDATE : " << r << std::endl;
        break;
    }
    case TYPE_CHECK_SIGNATURE:
    {
        std::cout << "TYPE_CHECK_SIGNATURE : " << r << std::endl;
        break;
    }
    case TYPE_RUN_INSTALLER:
    {
        std::cout << "TYPE_RUN_INSTALLER : " << r << std::endl;
        break;
    }
    }

    if ((o == TYPE_RUN_INSTALLER && r == RESULT_SUCCESS) || (r == RESULT_FAILED || r == RESULT_CANCELED))
    {
        std::cout << (r == RESULT_SUCCESS ? "SUCCESS" : "RESULT_FAILED || RESULT_CANCELED") << std::endl;
        stopOperation(updater);
    }

    if (getCurrentState(updater) == STATE_DOWNLOADING)
    {
        std::cout << "PAUSE..." << std::endl;
        pauseDownloading(updater);
        std::cout << "SLEEP FOR 3000 ms." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        std::cout << "RESUME..." << std::endl;
        resumeDownloading(updater);
    }
}

int main(int argc, char const *argv[])
{
    auto updater = getUpdaterInstance();

    setOperationResultEvent(updater, [updater](OperationType o, Result r, const ExtraInfo &i) {
        onResultEvent(updater, o, r, i);
    });

    std::string version("0.0.0.1");
    setCurrentVersion(updater, version.c_str(), version.length());
    setInfoUrl(updater, infoUrl.c_str(), infoUrl.length());
    setChannel(updater, "alpha", 5);

    checkAndUpdate(updater);

    while (getCurrentState(updater) != STATE_READY)
    {
        std::this_thread::yield();
    }

    std::cout << __FUNCTION__ << " :: END";
    return 0;
}
