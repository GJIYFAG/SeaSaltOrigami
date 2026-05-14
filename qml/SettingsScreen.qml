import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal close()
    anchors.fill: parent
    color: "#990f242b"

    Rectangle {
        anchors.centerIn: parent
        width: 520
        height: 420
        radius: 24
        color: "#fffaf2"
        border.color: "#92c7c5"
        border.width: 2

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 34
            spacing: 18
            Text { text: "游戏设置"; font.pixelSize: 30; font.bold: true; color: "#41666b" }
            Text { text: "当前版本保留核心设置入口，界面已改为参考图风格。"; wrapMode: Text.WordWrap; Layout.fillWidth: true; color: "#5f7377"; font.pixelSize: 17 }
            Rectangle { Layout.fillWidth: true; height: 1; color: "#e3d8c6" }
            Text { text: "快捷键：ESC 打开/关闭暂停菜单"; color: "#5f7377"; font.pixelSize: 16 }
            Text { text: "操作：点击抽牌堆执行抽2弃1；点击弃牌堆或右侧按钮拿取/弃置；手牌选满2张后可点击【出牌】。"; wrapMode: Text.WordWrap; Layout.fillWidth: true; color: "#5f7377"; font.pixelSize: 16 }
            Item { Layout.fillHeight: true }
            GameTextButton { Layout.alignment: Qt.AlignRight; width: 160; text: "关闭"; primary: true; onClicked: root.close() }
        }
    }
}
