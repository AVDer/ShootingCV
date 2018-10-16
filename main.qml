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

        ListView {
            id: logosRow
            readonly property int kLogoSize: 50
            height: kLogoSize
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            model: LogosModel{}
            delegate: Image {
                width: logosRow.kLogoSize
                height: logosRow.kLogoSize
                source: logo
            }
        }

    }

    GroupBox {
        title: "Frame source"
        ExclusiveGroup { id: frameSourceGroup }
        Layout.fillHeight: true

        ListView {
            Layout.fillHeight: true
            orientation: ListView.Vertical
            model: FrameSourceModel{}
            delegate: RadioButton {
                text: sourceName
                exclusiveGroup: frameSourceGroup
                onClicked: frameImage.currentImage = sourceURL
            }
        }

    }

}
