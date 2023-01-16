#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool recording READ getRecording NOTIFY recordingChanged)
    Q_PROPERTY(int seconds READ getSeconds NOTIFY secondsChanged)
public:
    explicit FileManager(QObject *parent = nullptr);

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();

    bool getRecording() const;
    int getSeconds() const;

public slots:
    void onCharacteristicUpdated(int task, QString uuid, QByteArray newValue);

signals:
    void recordingChanged();
    void secondsChanged();

private:
    bool recording {false};
    QByteArray dataCaptured;
    int seconds;
    int millis;

};

#endif // FILEMANAGER_H
