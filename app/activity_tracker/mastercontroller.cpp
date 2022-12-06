#include "mastercontroller.h"

MasterController::MasterController(QObject *parent)
    : QObject{parent}
{
    deviceController = new DeviceController(this);
}

DeviceController *MasterController::getDeviceController()
{
    return deviceController;
}
