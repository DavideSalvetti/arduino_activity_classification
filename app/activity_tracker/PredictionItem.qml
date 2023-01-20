import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Item {

    property int prediction : 0

    onPredictionChanged: {
        if (prediction === 0) {
            if (state != "cyclette")
                state = "cyclette"
        } else if (prediction === 1) {
            if (state != "idle")
                state = "idle"
        } else if (prediction === 2) {
            if (state != "jump")
                state = "jump"
        }else if (prediction === 3) {
            if (state != "walking")
                state = "walking"
        } else {
            if (state != "unknown")
                state = "unknown"
        }
    }

    Rectangle {
        anchors.fill: parent

        color: "transparent"

        Label {
            id: classId
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                margins: 5
            }

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:  Text.AlignVCenter

            font.bold: true

            text: "Anomaly"
        }

        Rectangle {
            id: predictionItemId
            anchors {
                centerIn: parent
            }

            width: parent.height/2
            height: parent.height/2


            color: "green"
            radius: 100


            ConicalGradient {
                source: predictionItemId
                anchors.fill: parent
                angle: 0.0
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.7; color: "transparent" }
                    GradientStop { position: 1.0; color: "black" }
                }
            }

            NumberAnimation {
                target: predictionItemId
                property: "rotation"
                loops: Animation.Infinite
                from: 0
                to: 360
                duration: 1500
                easing.type: Easing.Linear
                running: true
            }
        }

        Rectangle {
            id: rect
            anchors {
                centerIn: predictionItemId
            }

            radius:  100
            color: Material.backgroundColor

            width: predictionItemId.height - 20
            height: predictionItemId.height - 20

            Image {
                id: imagePredictionId
                anchors.centerIn: parent
                width: parent.height - 20
                height: parent.height - 20

                source: "qrc:/img/question-mark.png"
            }
        }
    }

    states: [
        State {
            name: "unknown"
            PropertyChanges {
                target: imagePredictionId
                source: "qrc:/img/question-mark.png"
            }
            PropertyChanges {
                target: predictionItemId
                color: "red"
            }
            PropertyChanges {
                target: classId
                text: "Unknown"
            }
        },
        State {
            name: "cyclette"
            PropertyChanges {
                target: imagePredictionId
                source: "qrc:/img/bicycle.png"
            }
            PropertyChanges {
                target: predictionItemId
                color: "green"
            }
            PropertyChanges {
                target: classId
                text: "Cyclette"
            }
        },
        State {
            name: "idle"
            PropertyChanges {
                target: imagePredictionId
                source: "qrc:/img/standing-up-man.png"
            }
            PropertyChanges {
                target: predictionItemId
                color: "green"
            }
            PropertyChanges {
                target: classId
                text: "Idle"
            }
        },
        State {
            name: "walking"
            PropertyChanges {
                target: imagePredictionId
                source: "qrc:/img/walk.png"
            }
            PropertyChanges {
                target: predictionItemId
                color: "green"
            }
            PropertyChanges {
                target: classId
                text: "Walking"
            }
        },
        State {
            name: "jump"
            PropertyChanges {
                target: imagePredictionId
                source: "qrc:/img/jumping-rope.png"
            }
            PropertyChanges {
                target: predictionItemId
                color: "green"
            }
            PropertyChanges {
                target: classId
                text: "Jumping"
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            ColorAnimation {
                target: predictionItemId
                duration: 500
            }
        }
    ]


    Component.onCompleted: {
        state = "unknown"
    }
}
