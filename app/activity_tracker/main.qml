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

    Material.theme: Material.Dark
    Material.accent: "white"
    Material.background: Material.Grey
    Material.primary: Material.Purple

    Connections {
        target: masterController.ui_deviceController

        function onDeviceStatusChanged(status)  {
            console.log("Device Status Changed! " + status)

            if (status == 0)
                commentId.text = "Disconnected"
            else if (status == 1)
                commentId.text = "Connecting"
            else if (status == 2)
                commentId.text = "Connected"

        }
    }

    Rectangle {

        anchors.fill: parent

        color: "#323232"

        Column {
            anchors {
                top: parent.top
                left: parent.left
                bottom: connectButton.top
                margins: 10
            }

            width: labelId.implicitWidth

            Label {
                id: labelId
                text: "Which device do you want to connect?"
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

        Button {
            id: connectButton
            anchors {
                bottom: parent.bottom
                left: parent.left
                margins: 10
            }


            text: "Connect"

            enabled: masterController.ui_deviceController.deviceStatus === 0 ? true : false

            onClicked: {

                if (radioDavide.checked)
                    masterController.ui_deviceController.connectToDevice("C3:4C:C0:10:83:DC")
                else if (radioMatteo.checked)
                    masterController.ui_deviceController.connectToDevice("CF:76:C9:5D:7E:92")
                else
                    masterController.ui_deviceController.connectToDevice("C0:8E:37:3D:2F:FD")
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

            height: connectButton.height
            text: "Disconnected"

            horizontalAlignment: parent.horizontalCenter

        }

        Button {
            id: recordButton
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 10
            }

            text: masterController.ui_fileManager.recording == false ? "Record" : "Stop Record"

            enabled: true

            onClicked: {
                if (masterController.ui_fileManager.recording)
                    masterController.ui_fileManager.stopRecording()
                else
                    masterController.ui_fileManager.startRecording()
            }
        }
    }

}
