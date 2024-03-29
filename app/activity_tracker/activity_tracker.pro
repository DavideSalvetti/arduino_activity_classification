QT += charts qml quick bluetooth quickcontrols2
android: QT += androidextras
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        activityobserver.cpp \
        channel.cpp \
        chartviewcontroller.cpp \
        dataparser.cpp \
        devicecontroller.cpp \
        filemanager.cpp \
        main.cpp \
        mastercontroller.cpp

RESOURCES += qml.qrc \
    img.qrc \
    qtquickcontrols2.conf

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    activityobserver.h \
    channel.h \
    chartviewcontroller.h \
    dataparser.h \
    devicecontroller.h \
    filemanager.h \
    mastercontroller.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
