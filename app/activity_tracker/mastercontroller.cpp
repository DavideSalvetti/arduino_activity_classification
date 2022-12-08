#include "mastercontroller.h"

MasterController::MasterController(QObject *parent)
    : QObject{parent}
{
    deviceController = new DeviceController(this);
    fileManager = new FileManager(this);

    connect(deviceController, &DeviceController::characteristicChanged,
            fileManager, &FileManager::onCharacteristicUpdated);
}

DeviceController *MasterController::getDeviceController()
{
    return deviceController;
}

FileManager *MasterController::getFileManager()
{
    return fileManager;
}
