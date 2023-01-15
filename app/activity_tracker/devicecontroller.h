#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>

#define DEVICE_ADDRESS QString("C3:4C:C0:10:83:DC")

#define DATA_SERVICE_UUID QString("{e2e65ffc-5687-4cbe-8f2d-db76265f269f}")
#define CLASSIFICATION_SERVICE_UUID QString("{e2e65ffc-5687-4cbe-8f2d-db76265f269a}");

class DeviceController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int deviceStatus READ getDeviceStatus NOTIFY deviceStatusChanged)
    Q_PROPERTY(int activityPrediction READ getActivityPrediction NOTIFY activityPredictionChanged)
    Q_PROPERTY(int task READ getTask NOTIFY taskChanged)
public:
    explicit DeviceController(QObject *parent = nullptr);

    Q_INVOKABLE void startDeviceDiscovery();
    Q_INVOKABLE void connectToDevice(const QString &address);
    Q_INVOKABLE void disconnectFromDevice();


    int getDeviceStatus() const;
    int getActivityPrediction() const;
    int getTask() const;

    enum DeviceStatus {
        DISCONNECTED = 0,
        CONNECTING = 1,
        CONNECTED = 2
    };

    enum Task {
        NONE = 0,
        DATA_ACQUISITION,
        ACTIVITY_PREDICTION
    };


private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

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
    void deviceStatusChanged(int status);
    void activityPredictionChanged(int activityPrediction);
    void characteristicChanged(QString uuid, QByteArray newValue);
    void taskChanged(int task);


private:
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent {nullptr};
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *deviceService {nullptr};
    QLowEnergyCharacteristic readCharacteristic;

    DeviceStatus deviceStatus;
    int activityPrediction;
    Task task;

};

#endif // DEVICECONTROLLER_H
