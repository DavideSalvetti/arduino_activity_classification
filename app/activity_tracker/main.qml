import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Connections {
        target: masterController.ui_deviceController

        function onDeviceStatusChanged(status)  {
            console.log("Device Status Changed! " + status)

            if (status === 0)
                commentId.text = "Disconnected"
            else if (status === 1)
                commentId.text = "Connecting"
            else if (status === 2)
                commentId.text = "Connected"

        }

        function onTaskChanged(task) {
            console.log("Task: " + task)

            if (task === 0) {
                title.text = "Arduino Activity Tracker App"
                frameId.state = "idle_mode"
            } else if (task === 1) {
                title.text = "Data Acquisition Mode"
                frameId.state = "acquisition_mode"
            } else if (task === 2) {
                title.text = "Activity Prediction Mode"
                frameId.state = "activity_mode"
            }
        }

        function onActivityPredictionChanged(prediction) {
            predictionItem.prediction = prediction
        }
    }

    Frame {
        id: frameId

        anchors.fill: parent

        Label {
            id: title
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 10
            }

            text: "Arduino Activity Tracker App"
            font.bold: true
            font.pointSize: 16

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:  Text.AlignVCenter
        }

        Column {
            id: devicesAvailable
            anchors {
                top: title.bottom
                left: parent.left
                bottom: connectButton.top
                margins: 10
            }

            width: labelId.implicitWidth

            Label {
                id: labelId
                text: "Choose a device:"
            }

            RadioButton {
                id: radioDavide
                checked: true
                text: qsTr("Davide")
            }

            RadioButton {
                id: radioMatteo
                text: qsTr("Matteo")
            }

            RadioButton {
                id: radioMartina
                text: qsTr("Martina")
            }
        }


        Label {
            id: secondsRecordedId
            anchors {
                right: parent.right
                top: title.bottom
                margins: 10
            }
            opacity: 0

            text: qsTr("Seconds recorded:") + " " + masterController.ui_fileManager.seconds

        }

        PredictionItem {
            id: predictionItem

            anchors {
                top: title.bottom
                left: parent.left
                right: parent.right
                bottom: connectButton.top
                margins: 10
            }

            opacity: 0

        }


        Button {
            id: connectButton

            x: parent.width/2 - width/2
            y: parent.height - height - 10

            highlighted: true


            text: masterController.ui_deviceController.deviceStatus === 0 ? "Connect" : "Disconnect"

            enabled: true

            onClicked: {

                if (connectButton.text == "Connect") {
                    if (radioDavide.checked)
                        masterController.ui_deviceController.connectToDevice("C3:4C:C0:10:83:DC")
                    else if (radioMatteo.checked)
                        masterController.ui_deviceController.connectToDevice("CF:76:C9:5D:7E:92")
                    else
                        masterController.ui_deviceController.connectToDevice("C0:8E:37:3D:2F:FD")
                } else {
                    masterController.ui_deviceController.disconnectFromDevice()
                }
            }
        }

        Label {
            id: commentId

            anchors {
                bottom: parent.bottom
                left: connectButton.right
                right: recordButton.left
                margins: 10
            }

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:  Text.AlignVCenter

            height: connectButton.height
            text: "Disconnected"

        }

        Button {
            id: recordButton
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 10
            }

            opacity: 0

            highlighted: true

            text: masterController.ui_fileManager.recording == false ? "Record" : "Stop Record"

            enabled: true

            onClicked: {
                if (masterController.ui_fileManager.recording)
                    masterController.ui_fileManager.stopRecording()
                else
                    masterController.ui_fileManager.startRecording()
            }
        }


        states: [
            State {
                name: "idle_mode"
                PropertyChanges {
                    target: devicesAvailable
                    opacity: 1
                }
                PropertyChanges {
                    target: connectButton
                    x: parent.width/2 - connectButton.width/2
                }
                PropertyChanges {
                    target: recordButton
                    opacity: 0
                }
                PropertyChanges {
                    target: secondsRecordedId
                    opacity: 0
                }
                PropertyChanges {
                    target: predictionItem
                    opacity: 0
                }
            },
            State {
                name: "acquisition_mode"
                PropertyChanges {
                    target: devicesAvailable
                    opacity: 0
                }
                PropertyChanges {
                    target: connectButton
                    x: 10
                }
                PropertyChanges {
                    target: recordButton
                    opacity: 1
                }
                PropertyChanges {
                    target: secondsRecordedId
                    opacity: 1
                }
            },
            State {
                name: "activity_mode"
                PropertyChanges {
                    target: devicesAvailable
                    opacity: 0
                }
                PropertyChanges {
                    target: predictionItem
                    opacity: 1
                }
            }
        ]

        transitions: [
            Transition {
                from: "idle_mode"
                to: "acquisition_mode"
                reversible: true


                NumberAnimation {
                    target: connectButton
                    property: "x"
                    duration: 500
                    easing.type: Easing.Linear
                }

                NumberAnimation {
                    target: recordButton
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.Linear
                }

                NumberAnimation {
                    target: secondsRecordedId
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.Linear
                }

                NumberAnimation {
                    target: devicesAvailable
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.Linear
                }

            },
            Transition {
                from: "idle_mode"
                to: "activity_mdoe"
                reversible: true

                NumberAnimation {
                    target: predictionItem
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.Linear
                }

                NumberAnimation {
                    target: devicesAvailable
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.Linear
                }

            }

        ]
    }

    Component.onCompleted: {
        frameId.state = "idle_mode"
    }

}
