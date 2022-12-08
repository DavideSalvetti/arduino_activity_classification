import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.15


ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Material.theme: Material.Dark
    Material.accent: Material.Purple
    Material.background: Material.Grey

    Rectangle {

        anchors.fill: parent

        color: "#323232"

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
                masterController.ui_deviceController.connectToDevice("Ciao")
            }
        }

        Button {
            anchors {
                bottom: parent.bottom
                left: connectButton.right
                margins: 10
            }

            text: "Record"

            enabled: masterController.ui_deviceController.deviceStatus === 2 ? true : false

            onClicked: {
                masterController.ui_deviceController.connectToDevice("Ciao")
            }
        }
    }

}
