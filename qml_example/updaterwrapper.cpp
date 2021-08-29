#include <QThread>

#include "updaterwrapper.h"


const QString baseUrl("https://gitlab.com/desktopsoftwareupdater/updaterexamples/-/raw/self-update-installers/");//"https://gitlab.com/desktopsoftwareupdater/updaterexamples/-/raw/master/");
#if _MSC_FULL_VER > 0
const QString infoUrl(baseUrl + "appUpdateSample_Win.xml");
#elif defined(__APPLE__)
const QString infoUrl(baseUrl + "appUpdateSample_MacOS.xml");
#elif defined(__linux__)
const QString infoUrl(baseUrl + "appUpdateSample_Linux.xml");
#endif

float last_percentage = 0.0;
void UpdaterWrapper::onResultEvent(UPDATER_PTR updater, OperationType o, Result r, const ExtraInfo &i)
{
    switch (o)
    {
    case TYPE_DOWNLOAD_INFO:
    {
        emit sigStepChanged(o, r, "");
        qDebug("TYPE_DOWNLOAD_INFO : %d", r);
        break;
    }
    case TYPE_CHECK_UPDATE_VERSION:
    {
        emit sigStepChanged(o, r, "");
        qDebug("TYPE_CHECK_UPDATE_VERSION : %d", r);
        break;
    }
    case TYPE_DOWNLOAD_UPDATE:
    {
        if(i.infoLength == sizeof(DownloadBytesInfo))
        {
            DownloadBytesInfo progress;
            memcpy(&progress, i.info, i.infoLength);

            auto current_percentage = (float)progress.current/(float)progress.total;
            if(current_percentage - last_percentage >= 0.01)
            {
                last_percentage = current_percentage;
                emit sigStepChanged(o, r, "");
                Q_EMIT sigDownloadingProgress(last_percentage);
            }
        }

        qDebug("TYPE_DOWNLOAD_UPDATE : %d", r);
        break;
    }
    case TYPE_CHECK_SIGNATURE:
    {
        emit sigStepChanged(o, r, "");
        qDebug("TYPE_CHECK_SIGNATURE : %d", r);
        break;
    }
    case TYPE_RUN_INSTALLER:
    {
        emit sigStepChanged(o, r, "");
        qDebug("TYPE_RUN_INSTALLER : %d", r);
        break;
    }
    }

    if ((o == TYPE_RUN_INSTALLER && r == RESULT_SUCCESS) || (r == RESULT_FAILED || r == RESULT_CANCELED))
    {
        emit sigStepChanged(-1, -1, "END_UPDATE");
        qDebug(r == RESULT_SUCCESS ? "SUCCESS" : "RESULT_FAILED || RESULT_CANCELED");
        if(r != RESULT_SUCCESS)
        {
            emit sigStepChanged(-1, -1, QString::fromStdString(std::string(i.info, i.infoLength)));
        }
        stopOperation(updater);
    }
}


UpdaterWrapper::UpdaterWrapper(QObject *parent) : QObject(parent)
{
    m_updater = getUpdaterInstance();

    setOperationResultEvent(m_updater, [this](OperationType o, Result r, const ExtraInfo &i) {
        this->onResultEvent(m_updater, o, r, i);
    });

    std::string version("0.0.0.1");
    setCurrentVersion(m_updater, version.c_str(), version.length());
    setInfoUrl(m_updater, infoUrl.toStdString().c_str(), infoUrl.length());
    setChannel(m_updater, "alpha", 5);
}

void UpdaterWrapper::update()
{
    last_percentage = 0.0;
    emit sigStepChanged(-1, 0, infoUrl);
    emit sigStepChanged(-1, 0, "BEGIN_UPDATE");
    checkAndUpdateAsync(m_updater);
}
