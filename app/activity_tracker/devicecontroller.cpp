#include "devicecontroller.h"

DeviceController::DeviceController(QObject *parent)
    : QObject{parent}
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    discoveryAgent->setLowEnergyDiscoveryTimeout(10000);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &DeviceController::addDevice);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &DeviceController::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &DeviceController::deviceScanFinished);

}

/**
 * @brief start bluetooth device discovery
 */
void DeviceController::startDeviceDiscovery()
{
    deviceList.clear();

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

/**
 * @brief bluetooth device found
 * @param blueooth information
 */
void DeviceController::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "Device Found: " + info.name() + " Address: " + info.address().toString() ;
}

/**
 * @brief device scan finished
 */
void DeviceController::deviceScanFinished()
{
    deviceList.clear();

    deviceList = discoveryAgent->discoveredDevices();

    if (deviceList.isEmpty())
        qDebug() << "Bluetooth Devices Not Found!";
    else
        qDebug() << "Device Scan Finished!";

//    emit devicesUpdated();
}

/**
 * @brief DeviceController::connectToDevice
 * @param address
 */
void DeviceController::connectToDevice(const QString &address)
{
    for (auto d: qAsConst(devices)) {
        if (auto device = qobject_cast<DeviceInfo *>(d)) {
            if (device->getAddress() == address ) {
                currentDevice.setDevice(device->getDevice());
                break;
            }
        }
    }

    if (!currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device - " << address;
        return;
    }

    /* Meglio se questa parte viene spostata nella classe DeviceInfo in modo
     * da poter gestire tutta la comunicazione con il dispositivo specifico
     * nella classe che rappresenta tale dispositivo. */
    if (controller && m_previousAddress != currentDevice.getAddress()) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    if (!controller) {
        controller = QLowEnergyController::createCentral(QBluetoothAddress("CF:76:C9:5D:7E:92"), localDevice.address());
        connect(controller, &QLowEnergyController::connected,
                this, &DeviceController::deviceConnected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
                this, &DeviceController::errorReceived);
        connect(controller, &QLowEnergyController::disconnected,
                this, &DeviceController::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &DeviceController::addLowEnergyService);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &DeviceController::serviceScanDone);
    }

    controller->connectToDevice();

    m_previousAddress = currentDevice.getAddress();
}
