#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool recording READ getRecording NOTIFY recordingChanged)
public:
    explicit FileManager(QObject *parent = nullptr);

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();

    bool getRecording() const;

public slots:
    void onCharacteristicUpdated(QString uuid, QByteArray newValue);

signals:
    void recordingChanged();

private:
    bool recording {false};
    QByteArray dataCaptured;

};

#endif // FILEMANAGER_H
