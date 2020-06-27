#include <QThread>

#include "updaterwrapper.h"

const QString baseUrl("https://gitlab.com/ronme/updateexample/-/raw/master/");
#if _MSC_FULL_VER > 0
const QString infoUrl(baseUrl + "appUpdateSample_Win.xml");
#elif defined(__APPLE__)
const QString infoUrl(baseUrl + "appUpdateSample_MacOS.xml");
#elif defined(__linux__)
const QString infoUrl(baseUrl + "appUpdateSample_Linux.xml");
#endif

void UpdaterWrapper::onResultEvent(UPDATER_PTR updater, OperationType o, Result r, const ExtraInfo &i)
{
    switch (o)
    {
    case TYPE_DOWNLOAD_INFO:
    {
        qDebug("TYPE_DOWNLOAD_INFO : %d", r);
        break;
    }
    case TYPE_CHECK_UPDATE_VERSION:
    {
        qDebug("TYPE_CHECK_UPDATE_VERSION : %d", r);
        break;
    }
    case TYPE_DOWNLOAD_UPDATE:
    {
        if(i.infoLength == sizeof(DownloadBytesInfo))
        {
            DownloadBytesInfo progress;
            memcpy(&progress, i.info, i.infoLength);
            Q_EMIT sigDownloadingProgress((progress.total/100) * progress.current);
        }

        qDebug("TYPE_DOWNLOAD_UPDATE : %d", r);
        break;
    }
    case TYPE_CHECK_SIGNATURE:
    {
        qDebug("TYPE_CHECK_SIGNATURE : %d", r);
        break;
    }
    case TYPE_RUN_INSTALLER:
    {
        qDebug("TYPE_RUN_INSTALLER : %d", r);
        break;
    }
    }

    if ((o == TYPE_RUN_INSTALLER && r == RESULT_SUCCESS) || (r == RESULT_FAILED || r == RESULT_CANCELED))
    {
        qDebug(r == RESULT_SUCCESS ? "SUCCESS" : "RESULT_FAILED || RESULT_CANCELED");
        stopOperation(updater);
    }
}


UpdaterWrapper::UpdaterWrapper(QObject *parent) : QObject(parent)
{
    m_updater = getUpdaterInstance();

    setOperationResultEvent(m_updater, [this](OperationType o, Result r, const ExtraInfo &i) {
        sigStepChanged(o, r, "");
        this->onResultEvent(m_updater, o, r, i);
    });

    std::string version("0.0.0.1");
    setCurrentVersion(m_updater, version.c_str(), version.length());
    setInfoUrl(m_updater, infoUrl.toStdString().c_str(), infoUrl.length());
    setChannel(m_updater, "alpha", 5);
}

void UpdaterWrapper::update()
{
    sigStepChanged(-1, 0, "BEGIN_UPDATE");
    checkAndUpdate(m_updater);
    sigStepChanged(-1, 0, "END_UPDATE");
}
