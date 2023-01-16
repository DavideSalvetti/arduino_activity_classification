#ifndef CHARTVIEWCONTROLLER_H
#define CHARTVIEWCONTROLLER_H

#include <QObject>



class ChartViewController : public QObject
{
    Q_OBJECT
public:
    explicit ChartViewController(QObject *parent = nullptr);

signals:

};

#endif // CHARTVIEWCONTROLLER_H
