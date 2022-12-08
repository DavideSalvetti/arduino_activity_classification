#include "filemanager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QDateTime>

FileManager::FileManager(QObject *parent)
    : QObject{parent}
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
    QString fileName = date.toString("dd_MM_yyyy_hh_mm") + "_reg.csv";

    QFile file(documentPath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly)) {

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

void FileManager::onCharacteristicUpdated(QString uuid, QByteArray newValue)
{
    if (recording) {
        qDebug() << "Received Value";

        dataCaptured.append(newValue);
        dataCaptured.append(0x0D);
    }
}


