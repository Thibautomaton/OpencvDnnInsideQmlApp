import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

ApplicationWindow {
    width: Screen.width

    height:Screen.height
    visible: true
    title: qsTr("OpencvDNN Example")

    Rectangle {
        id: root
        anchors.fill: parent
        color: "black"

            Rectangle {
                id: panel1
                width: 640
                height: 480
                color: "white"

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 10
                }

                Image {
                    id: opencvImage1
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    property bool counter: false
                    visible: false
                    source: "image://live1/image"
                    asynchronous: false
                    cache: false

                    function reload(){
                        counter = !counter
                        source = "image://live1/image?id=" + counter
                    }
                }

                Button {
                   id: activateC1
                   text: "Detection C1"
                   anchors {
                       top: panel1.bottom
                       horizontalCenter: parent.horizontalCenter
                       margins: 5
                   }
                   onClicked: {
                       opencvImage1.visible = !opencvImage1.visible
                       VideoStreamer1.openVideoCamera(0)
                   }
                }
            }

            Rectangle  {
                id: panel2
                width: 640
                height: 480
                color: "white"

                anchors {
                    left: panel1.right
                    verticalCenter: parent.verticalCenter
                    margins: 10
                }

                Image {
                    id: opencvImage2
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    property bool counter: false
                    source: "image://live2/image"
                    visible: false
                    asynchronous: false
                    cache: false
                    function reload()
                    {
                        counter = !counter
                        source = "image://live2/image?id=" + counter
                    }
                }

                Button {
                    id: activateC2
                    text: "Detection C2"
                    anchors {
                        top: panel2.bottom
                        horizontalCenter: parent.horizontalCenter
                        margins: 5
                    }
                    onClicked: {
                        VideoStreamer2.openVideoCamera(1)
                        opencvImage2.visible = !opencvImage2.visible
                    }
                }
            }
    }
    Connections {
        target: liveImageProvider1

        function onImageChanged()
        {
            opencvImage1.reload();
        }
    }
    Connections {
        target: liveImageProvider2

        function onImageChanged()
        {
            opencvImage2.reload()
        }
    }
}
