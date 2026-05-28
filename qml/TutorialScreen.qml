import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal close()
    anchors.fill: parent
    color: "#990f242b"

    Rectangle {
        anchors.centerIn: parent
        width: 720
        height: 560
        radius: 24
        color: "#fffaf2"
        border.color: "#92c7c5"
        border.width: 2

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 34
            spacing: 14
            Text { text: "游戏教程"; font.pixelSize: 30; font.bold: true; color: "#41666b" }
            Text { Layout.fillWidth: true; wrapMode: Text.WordWrap; color: "#5f7377"; font.pixelSize: 17; text: "1. 当前玩家每回合先抽牌：点击抽牌堆抽2弃1，或从弃牌堆1/2拿取顶部牌。" }
            Text { Layout.fillWidth: true; wrapMode: Text.WordWrap; color: "#5f7377"; font.pixelSize: 17; text: "2. 手牌区点击两张可打出的对牌，右侧【出牌】按钮亮起后点击即可触发对牌效果。点击第三张牌会自动替换最早选择的牌；可拖拽手牌调整顺序。" }
            Text { Layout.fillWidth: true; wrapMode: Text.WordWrap; color: "#5f7377"; font.pixelSize: 17; text: "3. 收集牌和增益牌会自动进入下方区域，按规则计算本轮分。" }
            Text { Layout.fillWidth: true; wrapMode: Text.WordWrap; color: "#5f7377"; font.pixelSize: 17; text: "4. 本轮分达到10分后，可选择结束本轮。" }
            Item { Layout.fillHeight: true }
            GameTextButton { Layout.alignment: Qt.AlignRight; width: 160; text: "我知道了"; primary: true; onClicked: root.close() }
        }
    }
}
