import QtQuick 2.10

Rectangle {
    anchors.fill: parent

    //width: 200
    //height: 100

    MouseArea {
        anchors.fill: parent
        onClicked: frameImage.imageClick()
    }

    Image {
        signal imageClick()
        anchors.fill: parent
        cache: false
        id: frameImage
        objectName: "frameImage"
    }
}
