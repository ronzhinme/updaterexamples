#ifndef UPDATERWRAPPER_H
#define UPDATERWRAPPER_H

#include <QObject>
#include <updaterInterface.h>

class UpdaterWrapper : public QObject
{
    Q_OBJECT
public:
    explicit UpdaterWrapper(QObject *parent = nullptr);
private:
  UPDATER_PTR m_updater;
  void onResultEvent(UPDATER_PTR updater, OperationType o, Result r, const ExtraInfo &i);
signals:
  void sigStepChanged(int step, int result, QString extraInfo);
  void sigDownloadingProgress(double value);
public slots:
  void update();
};

#endif // UPDATERWRAPPER_H
