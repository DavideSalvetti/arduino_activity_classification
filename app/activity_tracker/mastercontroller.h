#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>

#include "devicecontroller.h"
#include "filemanager.h"
#include "activityobserver.h"
#include "chartviewcontroller.h"

class MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( DeviceController* ui_deviceController READ getDeviceController CONSTANT )
    Q_PROPERTY( FileManager* ui_fileManager READ getFileManager CONSTANT )
    Q_PROPERTY( ActivityObserver *ui_activityObserver READ getActivityObserver CONSTANT )
    Q_PROPERTY( ChartViewController *ui_chartViewController READ getChartViewController CONSTANT)
public:
    explicit MasterController(QObject *parent = nullptr);


    DeviceController* getDeviceController();
    FileManager* getFileManager();
    ActivityObserver *getActivityObserver();
    ChartViewController *getChartViewController();

signals:


private:
    DeviceController *deviceController {nullptr};
    FileManager *fileManager {nullptr};
    ActivityObserver *activityObserver {nullptr};
    ChartViewController *chartViewController {nullptr};

};

#endif // MASTERCONTROLLER_H
