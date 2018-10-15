import QtQuick 2.10
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

RowLayout {
    anchors.fill: parent
    ColumnLayout {

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            MouseArea {
                anchors.fill: parent
                onClicked: frameImage.imageClick()
            }

            Timer {
                objectName: "imageTimer"
                repeat: true
                running: true
                onTriggered: {
                    frameImage.source = ""
                    frameImage.source = frameImage.currentImage
                }
            }

            Image {
                signal imageClick()

                anchors.fill: parent
                cache: false
                id: frameImage
                objectName: "frameImage"

                property string currentImage
            }
        }

        Row {

            id: logosRow
            readonly property int kLogoSize: 50

            Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                //anchors.bottom: parent.bottom
                source: "qrc:/logos/logo_c++.png"
            }
            Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                source: "qrc:/logos/logo_qt.png"
            }
            Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                source: "qrc:/logos/logo_qml.png"
            }
            Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                source: "qrc:/logos/logo_opencv.png"
            }
            Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                source: "qrc:/logos/logo_js.png"
            }
        }

    }

    GroupBox {
        title: "Frame source"

        ColumnLayout {
            ExclusiveGroup { id: frameSourceGroup }
            RadioButton {
                text: "Off"
                checked: true
                exclusiveGroup: frameSourceGroup
                onClicked:{
                    if(checked){
                        frameImage.currentImage = ""
                    }
                }
            }
            RadioButton {
                text: "Sample"
                exclusiveGroup: frameSourceGroup
                onClicked:{
                    if(checked){
                        frameImage.currentImage = "image://target/sample"
                    }
                }
            }
            RadioButton {
                text: "Original"
                exclusiveGroup: frameSourceGroup
                onClicked:{
                    if(checked){
                        frameImage.currentImage = "image://target/target"
                    }
                }
            }
            RadioButton {
                text: "Grey"
                exclusiveGroup: frameSourceGroup
            }
        }
    }

}
