#ifndef CHARTVIEWCONTROLLER_H
#define CHARTVIEWCONTROLLER_H

#include <QObject>
#include <QtCharts/QAbstractSeries>
#include <QTimer>
#include <QQmlListProperty>

#include "dataparser.h"
#include "channel.h"

QT_CHARTS_USE_NAMESPACE

class ChartViewController : public QObject
{
    Q_OBJECT
    Q_PROPERTY (double xMin READ getXMin NOTIFY xMinChanged )
    Q_PROPERTY (double xMax READ getXMax NOTIFY xMaxChanged )
    Q_PROPERTY (QQmlListProperty<Channel> channels READ getChannels NOTIFY channelsChanged)
public:
    explicit ChartViewController(QObject *parent = nullptr);

    double getXMin() const;
    double getXMax() const;
    QQmlListProperty<Channel> getChannels();


public slots:
    void onCharacteristicUpdated(int task, QString uuid, QByteArray newValue);

signals:
    void xMinChanged();
    void xMaxChanged();
    void channelsChanged();

public slots:
    void update(QAbstractSeries *series);
    void resetChart();

private slots:
    void updateValue();


private:
    QList<Channel*> channels;

    long timestamp = 0;
    QTimer timerUpdate;

    double xMin;
    double xMax;


    double fRand(double fMin, double fMax);
    Channel *getChannelWithName(QString name);
};

#endif // CHARTVIEWCONTROLLER_H
