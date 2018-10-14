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

    MouseArea {
        anchors.fill: parent
        onClicked: frameImage.imageClick()
    }

    Image {
        signal imageClick()
        //anchors.fill: parent

        width: 200
        height: 100

        id: frameImage
        objectName: "frameImage"
    }
}
