#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>

#include "devicecontroller.h"

class MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( DeviceController* ui_deviceController READ getDeviceController CONSTANT )
public:
    explicit MasterController(QObject *parent = nullptr);


    DeviceController* getDeviceController();

signals:


private:
    DeviceController *deviceController {nullptr};

};

#endif // MASTERCONTROLLER_H
