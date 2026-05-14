import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal settings()
    signal tutorial()
    anchors.fill: parent
    color: "#eef8f8"

    Rectangle {
        anchors.centerIn: parent
        width: 560
        height: 520
        radius: 34
        color: "#fffaf0"
        border.color: "#91c6c3"
        border.width: 2

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 44
            spacing: 18

            Text { Layout.alignment: Qt.AlignHCenter; text: "海盐折纸"; color: "#3d8f92"; font.pixelSize: 44; font.bold: true }
            Text { Layout.alignment: Qt.AlignHCenter; text: "Sea Salt Origami"; color: "#71888a"; font.pixelSize: 18 }
            Item { Layout.preferredHeight: 14 }
            MenuPrimaryButton { Layout.alignment: Qt.AlignHCenter; text: "单人模式"; onClicked: game.startGame(true) }
            MenuPrimaryButton { Layout.alignment: Qt.AlignHCenter; text: "双人本地"; secondary: true; onClicked: game.startGame(false) }
            Item { Layout.fillHeight: true }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 18
                MenuPrimaryButton { width: 180; text: "教程"; secondary: true; onClicked: root.tutorial() }
                MenuPrimaryButton { width: 180; text: "设置"; secondary: true; onClicked: root.settings() }
            }
        }
    }
}
