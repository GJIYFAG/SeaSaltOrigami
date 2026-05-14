import QtQuick
import QtQuick.Layouts

Item {
    id: root
    property string title: "牌堆"
    property string cardName: "空"
    property int count: 0
    property bool back: false
    property bool highlighted: false
    property bool clickable: true
    property bool hovered: mouse.containsMouse
    signal clicked()

    width: 230
    height: 270

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.title
            color: root.highlighted ? "#2f9290" : "#45666c"
            font.pixelSize: 19
            font.bold: true
            Behavior on color { ColorAnimation { duration: 160 } }
        }

        Rectangle {
            id: well
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 22
            color: root.highlighted ? "#f2fffd" : (root.hovered ? "#fbfffd" : "#f8faf7")
            border.color: root.highlighted ? "#65b5b2" : (root.hovered ? "#9bcecb" : "#dce4df")
            border.width: root.highlighted ? 2 : 1
            scale: root.hovered && root.clickable ? 1.018 : 1.0

            Behavior on color { ColorAnimation { duration: 160 } }
            Behavior on border.color { ColorAnimation { duration: 160 } }
            Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }

            Rectangle {
                anchors.centerIn: parent
                width: 150
                height: 196
                radius: 26
                color: "#57c5c1"
                opacity: root.highlighted ? 0.10 : 0
                scale: pulse.running ? 1.08 : 0.98
                Behavior on opacity { NumberAnimation { duration: 160 } }

                SequentialAnimation on scale {
                    id: pulse
                    running: root.highlighted
                    loops: Animation.Infinite
                    NumberAnimation { to: 1.08; duration: 680; easing.type: Easing.InOutSine }
                    NumberAnimation { to: 0.98; duration: 680; easing.type: Easing.InOutSine }
                }
            }

            GameCard {
                id: card
                anchors.centerIn: parent
                cardName: root.cardName
                back: root.back
                large: true
                hovered: root.hovered && root.clickable
                pressed: mouse.pressed
                footerText: root.back ? "" : (root.count + "张")
            }

            Rectangle {
                anchors.horizontalCenter: card.horizontalCenter
                anchors.top: card.bottom
                anchors.topMargin: -10
                width: root.back ? 72 : 58
                height: 26
                radius: 13
                color: "#58b8b3"
                visible: root.back || root.count > 0
                scale: root.hovered ? 1.05 : 1.0
                Behavior on scale { NumberAnimation { duration: 140 } }
                Text { anchors.centerIn: parent; text: root.back ? root.count : (root.count + "张"); color: "white"; font.bold: true; font.pixelSize: 15 }
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                enabled: root.clickable
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.clicked()
            }
        }
    }
}
