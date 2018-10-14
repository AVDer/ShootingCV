import QtQuick 2.10
import QtQuick.Window 2.10

Rectangle {
    //anchors.fill: parent

    width: 200
    height: 100

    Text {
        id: someText
        text: qsTr("Hello from QML")
    }

    Image {
        anchors.fill: parent
        id: frameImage
        objectName: "frameImage"
    }
}
