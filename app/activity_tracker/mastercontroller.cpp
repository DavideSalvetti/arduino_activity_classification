#include "mastercontroller.h"

MasterController::MasterController(QObject *parent)
    : QObject{parent}
{
    deviceController = new DeviceController(this);
    fileManager = new FileManager(this);
    activityObserver = new ActivityObserver(this);
    chartViewController = new ChartViewController(this);

    connect(deviceController, &DeviceController::characteristicChanged,
            fileManager, &FileManager::onCharacteristicUpdated);

    connect(deviceController, &DeviceController::characteristicChanged,
            activityObserver, &ActivityObserver::onCharacteristicUpdated);
}

DeviceController *MasterController::getDeviceController()
{
    return deviceController;
}

FileManager *MasterController::getFileManager()
{
    return fileManager;
}

ActivityObserver *MasterController::getActivityObserver()
{
    return activityObserver;
}

ChartViewController *MasterController::getChartViewController()
{
    return chartViewController;
}
