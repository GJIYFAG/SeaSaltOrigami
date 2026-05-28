import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    radius: 20
    color: "#f9fbf6"
    border.color: "#d7e0d8"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Text { text: "已打出对牌"; color: "#45666c"; font.pixelSize: 17; font.bold: true }
            Item { Layout.fillWidth: true }
            Rectangle {
                width: 48; height: 22; radius: 11
                color: "#eaf6f5"
                Text { anchors.centerIn: parent; text: Math.floor(game.playedPairCardNames.length / 2) + " 对"; color: "#3f8c8f"; font.pixelSize: 12; font.bold: true }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 16
            color: "#fffdf7"
            border.color: "#e5e1d5"
            border.width: 1

            Text {
                anchors.centerIn: parent
                visible: game.playedPairCardNames.length === 0
                text: "本轮暂未出牌"
                color: "#cbbfae"
                font.pixelSize: 15
                font.bold: true
            }

            Flickable {
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                contentWidth: pairColumn.width
                contentHeight: pairColumn.height
                boundsBehavior: Flickable.StopAtBounds

                Column {
                    id: pairColumn
                    width: parent.width
                    spacing: 8

                    Repeater {
                        model: Math.ceil(game.playedPairCardNames.length / 2)
                        delegate: Rectangle {
                            width: pairColumn.width
                            height: 64
                            radius: 12
                            color: "#f8f4ea"
                            border.color: "#eee3d0"

                            Row {
                                anchors.centerIn: parent
                                spacing: 6
                                GameCard { cardName: game.playedPairCardNames[index * 2]; compact: true; scale: 0.58; transformOrigin: Item.Center }
                                Text { text: "+"; color: "#9a8f7f"; font.pixelSize: 18; font.bold: true; anchors.verticalCenter: parent.verticalCenter }
                                GameCard { cardName: game.playedPairCardNames[index * 2 + 1] || "空"; compact: true; scale: 0.58; transformOrigin: Item.Center }
                            }
                        }
                    }
                }
            }
        }
    }
}
