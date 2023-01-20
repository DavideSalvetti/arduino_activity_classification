#include "dataparser.h"
#include <QDebug>

DataParser::DataParser(QByteArray data, QObject *parent)
    : QObject{parent}
{
    QList<QByteArray> values = data.split(0X2C);

    if (values.size()) {
        for (int i = 0; i < values.size(); i++) {
            if (i == 0)
                accX = values.at(i).toDouble();
            if (i == 1)
                accY = values.at(i).toDouble();
            if (i == 2)
                accZ = values.at(i).toDouble();
            if (i == 3)
                gyroX = values.at(i).toDouble();
            if (i == 4)
                gyroY = values.at(i).toDouble();
            if (i == 5)
                gyroZ = values.at(i).toDouble();
            if (i == 6)
                temp = values.at(i).toDouble();
            if (i == 7)
                humidity = values.at(i).toDouble();
            if (i == 8)
                timestamp = values.at(i).toLong();
        }
    }
}

double DataParser::getAccX() const
{
    return accX;
}

double DataParser::getAccY() const
{
    return accY;
}

double DataParser::getAccZ() const
{
    return accZ;
}

double DataParser::getGyroY() const
{
    return gyroY;
}

double DataParser::getGyroX() const
{
    return gyroX;
}

double DataParser::getGyroZ() const
{
    return gyroZ;
}

double DataParser::getTemp() const
{
    return temp;
}

double DataParser::getHumidity() const
{
    return humidity;
}

long DataParser::getTimestamp() const
{
    return timestamp;
}
