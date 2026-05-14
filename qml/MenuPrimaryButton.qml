import QtQuick

Rectangle {
    id: root
    property alias text: label.text
    property bool secondary: false
    signal clicked()

    width: 300
    height: 66
    radius: 20
    color: secondary ? "#fffaf0" : "#43aaa8"
    border.color: secondary ? "#d7c6a5" : "#2e908f"
    border.width: 2

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 24
        font.bold: true
        color: secondary ? "#526469" : "white"
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
        onPressed: root.scale = 0.98
        onReleased: root.scale = 1.0
        onCanceled: root.scale = 1.0
    }
}
