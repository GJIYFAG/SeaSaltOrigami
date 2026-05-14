import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    radius: 24
    color: "#fbfbf6"
    border.color: "#d9dfd8"
    border.width: 2

    property alias mainSpotItem: mainSpot
    property alias discard1SpotItem: discard1Spot
    property alias discard2SpotItem: discard2Spot

    function line(text, idx) {
        var a = String(text).split("\n")
        return a.length > idx ? a[idx] : ""
    }
    function countFrom(text, fallback) {
        var m = String(text).match(/([0-9]+)张|([0-9]+)/)
        if (!m) return fallback
        return parseInt(m[1] || m[2])
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 8

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "牌区"
            color: "#45666c"
            font.pixelSize: 22
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 28

            PileSpot {
                id: mainSpot
                title: "抽牌堆"
                cardName: "背面"
                count: countFrom(game.mainPileText, 0)
                back: true
                highlighted: game.pendingDrawActive
                onClicked: game.drawMain()
            }

            PileSpot {
                id: discard1Spot
                title: "弃牌堆1"
                cardName: line(game.discard1Text, 2) === "" ? "空" : line(game.discard1Text, 2)
                count: countFrom(line(game.discard1Text, 1), 0)
                highlighted: game.pendingDrawActive && game.pendingKeepIndex >= 0
                onClicked: game.pendingDrawActive ? game.finishPendingMainDraw(1) : game.drawDiscard1()
            }

            PileSpot {
                id: discard2Spot
                title: "弃牌堆2"
                cardName: line(game.discard2Text, 2) === "" ? "空" : line(game.discard2Text, 2)
                count: countFrom(line(game.discard2Text, 1), 0)
                highlighted: game.pendingDrawActive && game.pendingKeepIndex >= 0
                onClicked: game.pendingDrawActive ? game.finishPendingMainDraw(2) : game.drawDiscard2()
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: game.pendingDrawActive ? "选中保留牌后，点击弃牌堆1/2完成弃置" : "点击抽牌堆执行抽2弃1；也可以直接抽取弃牌堆顶部牌"
            color: "#819198"
            font.pixelSize: 15
        }
    }
}
