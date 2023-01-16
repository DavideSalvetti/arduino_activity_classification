#include "activityobserver.h"

ActivityObserver::ActivityObserver(QObject *parent)
    : QObject{parent},
    activityPrediction(5)
{

}

int ActivityObserver::getActivityPrediction() const
{
    return activityPrediction;
}

QVariantList ActivityObserver::getPreviousPrediction()
{
    return previousPrediction;
}

void ActivityObserver::onCharacteristicUpdated(int task, QString uuid, QByteArray newValue)
{
    Q_UNUSED(uuid)

    if (task != 2)
        return;

   // previousPrediction.append(activityPrediction);

    activityPrediction = newValue.at(0);

    emit activityPredictionChanged(activityPrediction);
}
