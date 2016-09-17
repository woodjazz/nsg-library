import QtQuick 2.0
import OpenGLUnderQML 1.0

Item
{
    width: 320
    height: 480
    property alias myQMLItem1: myQMLItem1

    MyQMLItem
    {
        id: myQMLItem1
    }

    Rectangle
    {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text
    {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "The background here is an item rendered with nsgEngine using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
}
