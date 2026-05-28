import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    property alias text: label.text
    property bool primary: false
    property bool danger: false
    property bool muted: false
    property bool enabledButton: true
    signal clicked()

    radius: height / 2
    width: 220
    height: 58
    opacity: enabledButton ? 1.0 : 0.48
    scale: mouse.pressed ? 0.98 : (mouse.containsMouse && enabledButton ? 1.018 : 1.0)
    color: danger ? "#fff2ef" : (primary ? "#42aaa8" : (muted ? "#f6f5ee" : "#fbfaf4"))
    border.width: primary ? 2 : 1
    border.color: danger ? "#ef9d95" : (primary ? "#2a918f" : (mouse.containsMouse && enabledButton ? "#93c7c4" : "#cfd9d5"))

    Behavior on scale { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }
    Behavior on opacity { NumberAnimation { duration: 120 } }
    Behavior on color { ColorAnimation { duration: 140 } }
    Behavior on border.color { ColorAnimation { duration: 140 } }

    layer.enabled: true
    layer.samples: 4

    Text {
        id: label
        anchors.centerIn: parent
        text: "按钮"
        color: danger ? "#b95d58" : (primary ? "white" : "#40545a")
        font.pixelSize: 21
        font.bold: true
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        enabled: root.enabledButton
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
