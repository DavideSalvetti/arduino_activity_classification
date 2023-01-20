#include "chartviewcontroller.h"

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

ChartViewController::ChartViewController(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    timerUpdate.setInterval(15);
    timerUpdate.setSingleShot(false);
    connect(&timerUpdate, &QTimer::timeout, this, &ChartViewController::updateValue);

    //timerUpdate.start();

    Channel *newChannel = new Channel("acc X", false, QColor(255, 0, 0), this);
    channels.append(newChannel);

    newChannel = new Channel("acc Y", false, QColor(0, 255, 0), this);
    channels.append(newChannel);

    newChannel = new Channel("acc Z", false, QColor(0, 50, 200), this);
    channels.append(newChannel);

    newChannel = new Channel("gyro X", false, QColor(150, 255, 0), this);
    channels.append(newChannel);

    newChannel = new Channel("gyro Y", false, QColor(125, 0, 255), this);
    channels.append(newChannel);

    newChannel = new Channel("gyro Z", false, QColor(255, 0, 125), this);
    channels.append(newChannel);

}

double ChartViewController::getXMin() const
{
    return xMin;
}

double ChartViewController::getXMax() const
{
    return xMax;
}

QQmlListProperty<Channel> ChartViewController::getChannels()
{
    return QQmlListProperty<Channel>(this, &channels);
}

void ChartViewController::onCharacteristicUpdated(int task, QString uuid, QByteArray newValue)
{
    Q_UNUSED(uuid)

    if (task != 1)
        return;

    DataParser parser(newValue);

    double ms = timestamp/1000.;

    getChannelWithName("acc X")->append(QPointF(ms, parser.getAccX()));
    getChannelWithName("acc Y")->append(QPointF(ms, parser.getAccY()));
    getChannelWithName("acc Z")->append(QPointF(ms, parser.getAccZ()));
    getChannelWithName("gyro X")->append(QPointF(ms, parser.getGyroX()));
    getChannelWithName("gyro Y")->append(QPointF(ms, parser.getGyroY()));
    getChannelWithName("gyro Z")->append(QPointF(ms, parser.getGyroZ()));

    timestamp += 15;

    xMax = ms;
    xMin = xMax - 15;

    if (xMin < 0) xMin = 0;
    if (xMax < 15) xMax = 15;

    emit xMinChanged();
    emit xMaxChanged();

    /* remove the first value so that we always have a 15 sec window */
    if (ms > 15000) {
        for (Channel *channel: qAsConst(channels)) {
            channel->removeFirst();
        }
    }

}

void ChartViewController::update(QAbstractSeries *series)
{
    if (series) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        Channel *channel = getChannelWithName(xySeries->name());

        if (channel) {
            xySeries->replace(channel->getValues());
            xySeries->setColor(channel->getColor());
            xySeries->setVisible(channel->getVisible());
        }

    }
}

void ChartViewController::resetChart()
{
    xMin = 0;
    xMax = 15;
    timestamp = 0;

    for (Channel *channel: qAsConst(channels))
        channel->clearValues();
}

void ChartViewController::updateValue()
{
    getChannelWithName("acc X")->append(QPointF(timestamp/1000., fRand(-2, 2)));
    getChannelWithName("acc Y")->append(QPointF(timestamp/1000., fRand(-3, 1)));
    getChannelWithName("acc Z")->append(QPointF(timestamp/1000., fRand(-1, 3)));
    getChannelWithName("gyro X")->append(QPointF(timestamp/1000., fRand(-80, 20)));
    getChannelWithName("gyro Y")->append(QPointF(timestamp/1000., fRand(-20, 80)));
    getChannelWithName("gyro Z")->append(QPointF(timestamp/1000., fRand(-100, 100)));
    timestamp += 15;

    xMax = timestamp/1000.;
    xMin = xMax - 15;

    if (xMin < 0) xMin = 0;
    if (xMax < 15) xMax = 15;

    emit xMinChanged();
    emit xMaxChanged();
}

double ChartViewController::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Channel * ChartViewController::getChannelWithName(QString name)
{
    for (Channel *channel: channels) {
        if (channel->getChannelName() == name)
            return channel;
    }

    return nullptr;
}



