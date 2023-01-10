#include "devicecontroller.h"

#include <QMetaEnum>

DeviceController::DeviceController(QObject *parent)
    : QObject{parent}
{
    deviceStatus = DISCONNECTED;

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
    //    deviceList.clear();

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

/**
 * @brief error while scanning bluetooth devices
 * @param error
 */
void DeviceController::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug() << "The Bluetooth adaptor is powered off, power it on before doing discovery.";
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qDebug() << "Writing or reading from the device resulted in an error.";
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
            discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        qDebug() << "Error: " + QLatin1String(qme.valueToKey(error));
    }

    deviceStatus = DISCONNECTED;
    emit deviceStatusChanged(deviceStatus);
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
    //    deviceList.clear();

    //    deviceList = discoveryAgent->discoveredDevices();

    //    if (deviceList.isEmpty())
    //        qDebug() << "Bluetooth Devices Not Found!";
    //    else
    //        qDebug() << "Device Scan Finished!";

    //    emit devicesUpdated();
}

/**
 * @brief DeviceController::connectToDevice
 * @param address
 */
void DeviceController::connectToDevice(const QString &address)
{

    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    if (!controller) {
        controller = QLowEnergyController::createCentral(QBluetoothAddress(address), localDevice.address());
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

    deviceStatus = CONNECTING;
    emit deviceStatusChanged(deviceStatus);
}

void DeviceController::disconnectFromDevice()
{
    controller->disconnectFromDevice();
}


/**
 * @brief getter of @param deviceStatus
 * @return
 */
int DeviceController::getDeviceStatus() const
{
    int value = 0;
    if (deviceStatus == CONNECTED)
        value = 2;
    else if (deviceStatus == DISCONNECTED)
        value = 0;
    else if (deviceStatus == CONNECTING)
        value = 1;

    return value;
}

int DeviceController::getActivityPrediction() const
{
    return activityPrediction;
}

void DeviceController::deviceConnected()
{
    qDebug() << "Device Connected";

    deviceStatus = CONNECTED;
    emit deviceStatusChanged(deviceStatus);

    controller->discoverServices();

    deviceService = controller->createServiceObject(QBluetoothUuid(QString("{e2e65ffc-5687-4cbe-8f2d-db76265f269f}")));

    if (!deviceService) {
        qWarning() << "The service UUID is not correct!";
        return;
    }

    qDebug() << deviceService->state();


    if (deviceService->state() == QLowEnergyService::DiscoveryRequired) {
        connect(deviceService, &QLowEnergyService::stateChanged,
                this, &DeviceController::serviceDetailsDiscovered);
        connect(deviceService, QOverload<QLowEnergyService::ServiceError>::of(&QLowEnergyService::error),
                [=](QLowEnergyService::ServiceError newError){
                    qDebug() << "communication error" << newError << deviceService->error();
                });

        //        connect(deviceService, &QLowEnergyService::characteristicWritten,
        //                this, &DeviceController::characteristicHasBeenWritten);
        connect(deviceService, &QLowEnergyService::characteristicRead,
                this, &DeviceController::characteristicHasBeenRead);
        connect(deviceService, &QLowEnergyService::descriptorWritten,
                this, &DeviceController::descriptorHasBeenWritten);
        deviceService->discoverDetails();
        return;
    }

    //discovery already done but it should never end here
    const QList<QLowEnergyCharacteristic> chars = deviceService->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars) {

        qDebug() << "Characterisic found: " + ch.uuid().toString();
        readCharacteristic = ch;

        QLowEnergyDescriptor notification = readCharacteristic.descriptor(
            QBluetoothUuid::ClientCharacteristicConfiguration);
        if (!notification.isValid())
            return;

        connect(deviceService, &QLowEnergyService::characteristicChanged,
                this, &DeviceController::characteristicHasChanged);

        deviceService->writeDescriptor(notification, QByteArray::fromHex("0100"));
    }


}

void DeviceController::deviceDisconnected()
{
    qWarning() << "Disconnected from device";

    deviceStatus = DISCONNECTED;
    emit deviceStatusChanged(deviceStatus);
}

void DeviceController::errorReceived(QLowEnergyController::Error error)
{
    qWarning() << "Error: " << controller->errorString();
    switch (error) {

        // ---- UNKNOWN ERRORS ----
    case QLowEnergyController::NoError:
    case QLowEnergyController::UnknownError:

        break;

        // ---- UNKNOWN REMOVE DEVICE ERROR ----
    case QLowEnergyController::UnknownRemoteDeviceError:

        break;

        // ---- CONNECTION RELATED ERROR ----
    case QLowEnergyController::InvalidBluetoothAdapterError:
    case QLowEnergyController::ConnectionError:
    case QLowEnergyController::AuthorizationError:

        break;

        // ---- COMMUNICATION RELATED ERROR ----
    case QLowEnergyController::AdvertisingError:
    case QLowEnergyController::NetworkError:
    case QLowEnergyController::RemoteHostClosedError:

        break;

    default:
        break;
    }

    deviceStatus = DISCONNECTED;
    emit deviceStatusChanged(deviceStatus);
}

void DeviceController::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }

    QBluetoothUuid uuid = service->serviceUuid();
    qDebug() << "Service Found: " + uuid.toString();

    if (uuid.toString() == "{e2e65ffc-5687-4cbe-8f2d-db76265f269f}" ||
        uuid.toString() == "{e2e65ffc-5687-4cbe-8f2d-db76265f269a}") {

        if (uuid.toString() == "{e2e65ffc-5687-4cbe-8f2d-db76265f269f}") {
            qDebug() << "Data Aquisition Application";
        } else
            qDebug() << "Inference Application";

        deviceService = service;
        qDebug() << "Communication Service Found! State:" << deviceService->state();

        if (deviceService->state() == QLowEnergyService::DiscoveryRequired) {
            connect(deviceService, &QLowEnergyService::stateChanged,
                    this, &DeviceController::serviceDetailsDiscovered);
            connect(deviceService, QOverload<QLowEnergyService::ServiceError>::of(&QLowEnergyService::error),
                    [=](QLowEnergyService::ServiceError newError){
                        qDebug() << "communication error" << newError << deviceService->error();
                    });

            //        connect(deviceService, &QLowEnergyService::characteristicWritten,
            //                this, &DeviceController::characteristicHasBeenWritten);
            connect(deviceService, &QLowEnergyService::characteristicRead,
                    this, &DeviceController::characteristicHasBeenRead);
            connect(deviceService, &QLowEnergyService::descriptorWritten,
                    this, &DeviceController::descriptorHasBeenWritten);

            deviceService->discoverDetails();
            return;
        }

        //discovery already done but it should never end here
        const QList<QLowEnergyCharacteristic> chars = deviceService->characteristics();
        for (const QLowEnergyCharacteristic &ch : chars) {

            qDebug() << "Characterisic found: " + ch.uuid().toString();
            readCharacteristic = ch;

            QLowEnergyDescriptor notification = readCharacteristic.descriptor(
                QBluetoothUuid::ClientCharacteristicConfiguration);
            if (!notification.isValid())
                return;

            connect(deviceService, &QLowEnergyService::characteristicChanged,
                    this, &DeviceController::characteristicHasChanged);

            deviceService->writeDescriptor(notification, QByteArray::fromHex("0100"));
        }
    }
}

void DeviceController::serviceScanDone()
{
    qDebug() << "Service Scan Finished";
}

void DeviceController::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        if (newState != QLowEnergyService::DiscoveringServices) {
            QMetaObject::invokeMethod(this, "characteristicsUpdated",
                                      Qt::QueuedConnection);
        }
        return;
    }

    auto service = qobject_cast<QLowEnergyService *>(sender());
    if (!service)
        return;

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars) {

        qDebug() << "Characterisic found: " + ch.uuid().toString();

        if (ch.uuid().toString() == "{00003000-0000-1000-8000-00805f9b34fb}") {

            readCharacteristic = ch;

            QLowEnergyDescriptor notification = readCharacteristic.descriptor(
                QBluetoothUuid::ClientCharacteristicConfiguration);
            if (!notification.isValid())
                return;

            connect(deviceService, &QLowEnergyService::characteristicChanged,
                    this, &DeviceController::characteristicHasChanged);

            deviceService->writeDescriptor(notification, QByteArray::fromHex("0100"));

        } else if (ch.uuid().toString() == "") {

        } else if (ch.uuid().toString() == "") {

        }
    }
}

void DeviceController::characteristicHasChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
{
    qDebug() << "CharacteristicChanged - " + characteristic.uuid().toString() + " new value: " + newValue + " newValueLenght:" + newValue.length();

    if (0) {

        activityPrediction = newValue.toInt();

        emit activityPredictionChanged(activityPrediction);
    } else {
        emit characteristicChanged(characteristic.uuid().toString(), newValue);
    }
}

void DeviceController::characteristicHasBeenWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
{
    qDebug() << "CharacteristicWritten - " + characteristic.uuid().toString() + " new value: " + newValue;
}

void DeviceController::characteristicHasBeenRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << "CharacteristicRead - " + characteristic.uuid().toString() + " new value: " + value;
}

void DeviceController::descriptorHasBeenWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue)
{
    qDebug() << "DescriptorWritten - " + descriptor.uuid().toString() + " new value: " + newValue;
}

