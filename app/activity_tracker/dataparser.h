#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>

class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QByteArray data, QObject *parent = nullptr);

    double getAccX() const;
    double getAccY() const;
    double getAccZ() const;
    double getGyroY() const;
    double getGyroX() const;
    double getGyroZ() const;
    double getTemp() const;
    double getHumidity() const;

    long getTimestamp() const;

private:
    double accX;
    double accY;
    double accZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    double temp;
    double humidity;
    long timestamp;

};

#endif // DATAPARSER_H
