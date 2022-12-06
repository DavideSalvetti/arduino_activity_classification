#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>

class DeviceController : public QObject
{
    Q_OBJECT
public:
    explicit DeviceController(QObject *parent = nullptr);

    Q_INVOKABLE void startDeviceDiscovery();
    Q_INVOKABLE void connectToDevice(const QString &address);


private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
    void deviceScanFinished();

signals:

private:
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent {nullptr};
    QList<QBluetoothDeviceInfo> deviceList;
    QLowEnergyController *controller = nullptr;

};

#endif // DEVICECONTROLLER_H
