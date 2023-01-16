#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include <QQmlContext>
#include <QQuickStyle>

#include "mastercontroller.h"


#if defined (Q_OS_ANDROID)
#include <QtAndroid>
const QVector<QString> permissions({"android.permission.ACCESS_COARSE_LOCATION",
                                    "android.permission.BLUETOOTH",
                                    "android.permission.BLUETOOTH_ADMIN",
                                    "android.permission.WRITE_EXTERNAL_STORAGE",
                                    "android.permission.READ_EXTERNAL_STORAGE"});
#endif


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));


    QQuickStyle::setStyle("Material");

#if defined (Q_OS_ANDROID)
    //Request requiered permissions at runtime
    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return 0;
        }
    }
#endif

    MasterController masterController;

    qmlRegisterType<MasterController>("org.activitytracker.controllers", 1, 0, "MasterController");
    qmlRegisterUncreatableType<DeviceController>("org.activitytracker.controllers", 1, 0, "DeviceController", "Uncreatable type from QML.");
    qmlRegisterUncreatableType<FileManager>("org.activitytracker.controllers", 1, 0, "FileManager", "Uncreatable type from QML.");
    qmlRegisterUncreatableType<ActivityObserver>("org.activitytracker.controllers", 1, 0, "ActivityObserver", "Uncreatable type from QML.");

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
