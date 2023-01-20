import QtQuick 2.15
import QtCharts 2.15

ChartView {
    id: chartView
    animationOptions: ChartView.SeriesAnimations
    theme: ChartView.ChartThemeLight

    property bool openGL: true
    property bool openGLSupported: true

    legend.visible: false
    antialiasing: true

    //plotArea: Qt.rect(x, y, width, height)


    ValueAxis {
        id: axisY1
        min: -8
        max: 8

        gridVisible: false
    }

    ValueAxis {
        id: axisY2
        min: -800
        max: 800

        gridVisible: false
    }

    ValueAxis {
        id: axisX
        min: masterController.ui_chartViewController.xMin
        max: masterController.ui_chartViewController.xMax
        tickCount: 2
        gridVisible: false
    }

    LineSeries {
        id: lineSeriesAccX
        name: "acc X"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeriesAccY
        name: "acc Y"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeriesAccZ
        name: "acc Z"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeriesGyroX
        name: "gyro X"
        axisX: axisX
        axisY: axisY2
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeriesGyroY
        name: "gyro Y"
        axisX: axisX
        axisY: axisY2
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeriesGyroZ
        name: "gyro Z"
        axisX: axisX
        axisY: axisY2
        useOpenGL: chartView.openGL
    }


    Timer {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            masterController.ui_chartViewController.update(chartView.series(0));
            masterController.ui_chartViewController.update(chartView.series(1));
            masterController.ui_chartViewController.update(chartView.series(2));
            masterController.ui_chartViewController.update(chartView.series(3));
            masterController.ui_chartViewController.update(chartView.series(4));
            masterController.ui_chartViewController.update(chartView.series(5));
        }
    }


    onOpenGLChanged: {
        if (openGLSupported) {
            series("acc X").useOpenGL = openGL;
            series("acc Y").useOpenGL = openGL;
            series("acc Z").useOpenGL = openGL;
            series("gyro X").useOpenGL = openGL;
            series("gyro Y").useOpenGL = openGL;
            series("gyro Z").useOpenGL = openGL;
        }
    }
    Component.onCompleted: {
//        if (!series("accX").useOpenGL) {
//            openGLSupported = false
//            openGL = false
//        }
//        if (!series("accY").useOpenGL) {
//            openGLSupported = false
//            openGL = false
//        }
//        if (!series("accZ").useOpenGL) {
//            openGLSupported = false
//            openGL = false
//        }


    }
}
