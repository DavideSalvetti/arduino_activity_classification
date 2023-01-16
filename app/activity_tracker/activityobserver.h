#ifndef ACTIVITYOBSERVER_H
#define ACTIVITYOBSERVER_H

#include <QObject>
#include <QQmlListProperty>

class ActivityObserver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int activityPrediction READ getActivityPrediction NOTIFY activityPredictionChanged)
    Q_PROPERTY( QVariantList previousPrediction READ getPreviousPrediction NOTIFY previousPredictionChanged)
public:
    explicit ActivityObserver(QObject *parent = nullptr);

    int getActivityPrediction() const;
    QVariantList getPreviousPrediction();

public slots:
    void onCharacteristicUpdated(int task, QString uuid, QByteArray newValue);


signals:
    void activityPredictionChanged(int activityPrediction);
    void previousPredictionChanged();

private:
    int activityPrediction;
    QVariantList previousPrediction;

};

#endif // ACTIVITYOBSERVER_H
