#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>

#include "devicecontroller.h"
#include "filemanager.h"

class MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( DeviceController* ui_deviceController READ getDeviceController CONSTANT )
    Q_PROPERTY( FileManager* ui_fileManager READ getFileManager CONSTANT )
public:
    explicit MasterController(QObject *parent = nullptr);


    DeviceController* getDeviceController();
    FileManager* getFileManager();

signals:


private:
    DeviceController *deviceController {nullptr};
    FileManager *fileManager {nullptr};

};

#endif // MASTERCONTROLLER_H
