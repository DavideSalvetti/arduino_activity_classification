#include "channel.h"

Channel::Channel(QString channelName, bool visible, QColor color, QObject *parent)
    : QObject{parent},
    channelName(channelName),
    visible(visible),
    color(color)
{

}

const QString &Channel::getChannelName() const
{
    return channelName;
}

void Channel::setChannelName(const QString &newChannelName)
{
    channelName = newChannelName;
}

bool Channel::getVisible() const
{
    return visible;
}

void Channel::setVisible(bool newVisible)
{
    visible = newVisible;

    emit visibleChanged();
}

QColor Channel::getColor() const
{
    return color;
}

void Channel::append(QPointF value)
{
    values.append(value);
}

void Channel::removeFirst()
{
    if (values.size() > 0)
        values.removeFirst();
}

const QVector<QPointF> &Channel::getValues() const
{
    return values;
}

void Channel::setValues(const QVector<QPointF> &newValues)
{
    values = newValues;
}

void Channel::clearValues()
{
    values.clear();
}
