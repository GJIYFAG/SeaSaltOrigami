import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    radius: 18
    color: "#fffdf7"
    border.color: "#ded8c8"
    border.width: 2

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 10

        Text { text: "增益牌"; color: "#4f656b"; font.pixelSize: 20; font.bold: true }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 16
            color: "#fbfaf2"
            border.color: "#e1d8c4"
            border.width: 1

            Text {
                anchors.centerIn: parent
                visible: game.buffCardNames.length === 0
                text: "暂无"
                color: "#d4c9b7"
                font.pixelSize: 20
            }

            Flow {
                anchors.fill: parent
                anchors.margins: 14
                spacing: 10
                Repeater {
                    model: game.buffCardNames
                    delegate: GameCard { cardName: modelData; compact: true }
                }
            }
        }
    }
}
