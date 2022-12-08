#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include <QQmlContext>
#include <QQuickStyle>

#include "mastercontroller.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));

    QQuickStyle::setStyle("Material");

    MasterController masterController;

    qmlRegisterType<MasterController>("org.activitytracker.controllers", 1, 0, "MasterController");
    qmlRegisterUncreatableType<DeviceController>("org.activitytracker.controllers", 1, 0, "DeviceController", "Uncreatable type from QML.");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("masterController", &masterController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
