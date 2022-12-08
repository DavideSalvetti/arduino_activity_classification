#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>

#define DEVICE_ADDRESS QString("C3:4C:C0:10:83:DC")

#define SERVICE_UUID QString("e2e65ffc-5687-4cbe-8f2d-db76265f269f")
#define CHARACTERISTIC_UUID QString("478e24cf-5e0c-451d-b220-02aacd43f0c1f")

class DeviceController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int deviceStatus READ getDeviceStatus NOTIFY deviceStatusChanged)
public:
    explicit DeviceController(QObject *parent = nullptr);

    Q_INVOKABLE void startDeviceDiscovery();
    Q_INVOKABLE void connectToDevice(const QString &address);


    int getDeviceStatus() const;

    enum DeviceStatus {
        DISCONNECTED = 0,
        CONNECTING = 1,
        CONNECTED = 2
    };


private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
    void deviceScanFinished();

    void deviceConnected();
    void deviceDisconnected();
    void errorReceived(QLowEnergyController::Error);

    void addLowEnergyService(const QBluetoothUuid &uuid);
    void serviceScanDone();

    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void characteristicHasChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);

    void characteristicHasBeenWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
    void characteristicHasBeenRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void descriptorHasBeenWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);

signals:
    void deviceStatusChanged();


private:
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent {nullptr};
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *deviceService {nullptr};
    QLowEnergyCharacteristic readCharacteristic;

    DeviceStatus deviceStatus;

};

#endif // DEVICECONTROLLER_H
