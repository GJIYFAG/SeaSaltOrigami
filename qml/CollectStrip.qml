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

        Text { Layout.alignment: Qt.AlignHCenter; text: "收集牌"; color: "#6a5b52"; font.pixelSize: 20; font.bold: true }

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: collectFlow.width
            contentHeight: collectFlow.height

            Flow {
                id: collectFlow
                width: parent.width
                spacing: 12
                Repeater {
                    model: game.collectCardLines
                    delegate: GameCard {
                        cardName: String(modelData).split("\n")[0]
                        footerText: String(modelData).split("\n").length > 1 ? String(modelData).split("\n")[1] : ""
                        compact: true
                    }
                }
            }
        }
    }
}
