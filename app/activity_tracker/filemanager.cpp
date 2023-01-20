#include "filemanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QDateTime>

FileManager::FileManager(QObject *parent)
    : QObject{parent},
    seconds(0),
    millis(0)
{

    for (int i = 0; i <= static_cast<int>(QStandardPaths::AppConfigLocation); i++)
    {
        const QString tmp = QStandardPaths::writableLocation(static_cast<QStandardPaths::StandardLocation>(i));
        qDebug() << "Path" << i << tmp;
    }

}

/**
 * @brief set a boolean to record
 */
void FileManager::startRecording()
{
    recording = true;

    seconds = 0;
    millis = 0;

    emit recordingChanged();

    dataCaptured.clear();
}

/**
 * @brief set a boolean to stop record
 */
void FileManager::stopRecording()
{
    recording = false;

    emit recordingChanged();

    // devo salvare il file in qualche modo.
    QString documentPath =  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QDateTime date = QDateTime::currentDateTime();
    QString fileName = date.toString("dd_MM_yyyy_hh_mm") + ".csv";

    QByteArray header = "acc x,acc y,acc z,gyro x,gyro y,gyro z,temp,humidity,timestamp";
    header.append(0x0D);

    QFile file(documentPath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly)) {

        file.write(header);

        file.write(dataCaptured);

        file.close();

        qDebug() << "File written, path: " + file.fileName();
    } else
        qDebug() << "Error opening the file, " + file.errorString();

}

bool FileManager::getRecording() const
{
    return recording;
}

int FileManager::getSeconds() const
{
    return seconds;
}

void FileManager::onCharacteristicUpdated(int task, QString uuid, QByteArray newValue)
{
    Q_UNUSED(uuid)

    if (task != 1)
        return;

    if (recording) {

        dataCaptured.append(newValue);
        dataCaptured.append(0x0D);

        millis += 15;
        seconds = millis / 1000;

        emit secondsChanged();
    }
}


