#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QPointF>
#include <QColor>

class Channel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString channelName READ getChannelName  CONSTANT)
    Q_PROPERTY(QColor channelColor READ getColor CONSTANT)
    Q_PROPERTY(bool channelVisible READ getVisible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit Channel(QString channelName, bool visible, QColor color, QObject *parent = nullptr);


    const QString &getChannelName() const;
    void setChannelName(const QString &newChannelName);

    bool getVisible() const;
    void setVisible(bool newVisible);

    QColor getColor() const;

    void append(QPointF value);
    void removeFirst();


    const QVector<QPointF> &getValues() const;
    void setValues(const QVector<QPointF> &newValues);
    void clearValues();

signals:
    void visibleChanged();

private:
    QString channelName;
    bool visible;
    QVector<QPointF> values;
    QColor color;

};

#endif // CHANNEL_H
