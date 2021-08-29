#include <iostream>
#include <string>
#include <thread>

#include <updaterInterface.h>

const std::string baseUrl("https://gitlab.com/desktopsoftwareupdater/updaterexamples/-/raw/self-update-installers/");//"https://gitlab.com/desktopsoftwareupdater/updaterexamples/-/raw/master/");
#if _MSC_FULL_VER > 0
const std::string infoUrl(baseUrl + "appUpdateSample_Win.xml");
#elif defined(__APPLE__)
const std::string infoUrl(baseUrl + "appUpdateSample_MacOS.xml");
#elif defined(__linux__)
const std::string infoUrl(baseUrl + "appUpdateSample_Linux.xml");
#endif

void onResultEvent(UPDATER_PTR updater, OperationType o, Result r, const ExtraInfo& i)
{
    std::cout << __FUNCTION__ << " [" << o << "][" << r << "]" << std::endl;

    if (o == TYPE_DOWNLOAD_INFO && r == RESULT_SUCCESS)
    {
        std::cout << "Downloading Version Info completed." << std::endl;
        std::cout << "Finding next version ..." << std::endl;
        findNextVersion(updater);
    }

    if (o == TYPE_CHECK_UPDATE_VERSION && r == RESULT_SUCCESS)
    {
        std::cout << "Checking Update version completed." << std::endl;
        std::cout << "Get next app version" << std::endl;
        char updVersionAttribute[300];
        memset(updVersionAttribute, 0, 300);
        getNextVersionAttribute(updater, updVersionAttribute, 300, "id", 2);
        std::cout << "Get next app version: " << updVersionAttribute << std::endl;

        std::cout << "Downloading Update version ..." << std::endl;
        downloadUpdate(updater);
    }

    if (o == TYPE_DOWNLOAD_UPDATE && r == RESULT_INPROGRESS)
    {
        if (i.infoLength == sizeof(DownloadBytesInfo))
        {
            DownloadBytesInfo progressInfo;
            memcpy(&progressInfo, i.info, i.infoLength);
            std::cout << "Downloading update progress: "
                      << progressInfo.current << " / " << progressInfo.total << std::endl;
        }
    }

    if (o == TYPE_DOWNLOAD_UPDATE && r == RESULT_SUCCESS)
    {
        std::cout << "Downloading Update version completed" << std::endl;
        checkSignature(updater);
    }

    if (o == TYPE_CHECK_SIGNATURE && (r == RESULT_SUCCESS || r == RESULT_FAILED))
    {
        printf("Checking signature result: %d\n", r == RESULT_SUCCESS ? 1 : 0);
    }

    if (o == TYPE_CHECK_SIGNATURE && r == RESULT_SUCCESS)
    {
        std::cout << "Executing Update version installer" << std::endl;
        runInstaller(updater);
    }

    if ((o == TYPE_RUN_INSTALLER && r == RESULT_SUCCESS) || (r == RESULT_FAILED || r == RESULT_CANCELED))
    {
        if (i.infoLength > 0)
        {
            std::cout << i.info << std::endl;
        }
        stopOperation(updater);
    }
}

int main(int argc, char const* argv[])
{
    if (argc > 1)
        std::cout << "Just imagine I'm the installer" << std::endl;

    std::cout << "Test of the appUpdater library" << std::endl;

    auto updater = getUpdaterInstance();
    std::string version("0.0.0.1");
    std::cout << "Set current app version: " << version << std::endl;
    setCurrentVersion(updater, version.c_str(), version.length());

    char retVersion[30];
    memset(retVersion, 0, 30);
    getCurrentVersion(updater, retVersion, 30);
    std::cout << "Get current app version: " << retVersion << std::endl;

    setOperationResultEvent(updater, [updater](OperationType o, Result r, const ExtraInfo & i)
    {
        onResultEvent(updater, o, r, i);
    });

    std::cout << "Set infoUrl: " << infoUrl << std::endl;
    setInfoUrl(updater, infoUrl.c_str(), infoUrl.length());

    char infoRet[128];
    memset(&infoRet, 0, 128);
    getInfoUrl(updater, infoRet, 128);
    std::cout << "Get infoUrl: " << infoRet << std::endl;
    setChannel(updater, "alpha", 5);

    char channel[30];
    memset(channel, 0, 30);
    getChannel(updater, channel, 30);
    std::cout << "Get update app channel: " << channel << std::endl;

    std::cout << "Downloading Version Info ..." << std::endl;
    downloadInfo(updater);

    while (getCurrentState(updater) != STATE_READY)
    {
        std::this_thread::yield();
    }

    std::cout << __FUNCTION__ << " :: END";
    return 0;
}
