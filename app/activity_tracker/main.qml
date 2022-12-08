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
                else
                    masterController.ui_deviceController.connectToDevice("CF:76:C9:5D:7E:92")
            }
        }

        Button {

            anchors {
                bottom: parent.bottom
                left: connectButton.right
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
