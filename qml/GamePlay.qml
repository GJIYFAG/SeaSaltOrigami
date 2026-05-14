import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal openScore()
    signal openSettings()
    signal openTutorial()
    signal openPause()

    anchors.fill: parent
    color: "#e8f6f5"

    function itemCenter(item) {
        if (!item) return Qt.point(width / 2, height / 2)
        return item.mapToItem(root, item.width / 2, item.height / 2)
    }

    function pileItem(name) {
        if (name === "main") return piles.mainSpotItem
        if (name === "discard1") return piles.discard1SpotItem
        if (name === "discard2") return piles.discard2SpotItem
        if (name === "pending") return pendingBox
        return piles.mainSpotItem
    }

    function isCollectCardName(cardName) {
        return cardName.indexOf("贝壳") >= 0 || cardName.indexOf("章鱼") >= 0 || cardName.indexOf("企鹅") >= 0
               || cardName.indexOf("船锚") >= 0 || cardName.indexOf("海马") >= 0 || cardName.indexOf("美人鱼") >= 0
    }

    function isBuffCardName(cardName) {
        return cardName.indexOf("群牌") >= 0 || cardName.indexOf("队牌") >= 0 || cardName.indexOf("堆牌") >= 0
    }

    function targetZoneForCard(cardName) {
        if (isBuffCardName(cardName)) return "buff"
        if (isCollectCardName(cardName)) return "collect"
        return "hand"
    }

    function targetItem(name) {
        if (name === "hand") return handArea
        if (name === "collect") return collectArea
        if (name === "buff") return buffArea
        if (name === "played") return playedArea
        if (name === "discard1") return piles.discard1SpotItem
        if (name === "discard2") return piles.discard2SpotItem
        if (name === "pending") return pendingBox
        return handArea
    }

    function sourceItem(name) {
        if (name === "main" || name === "discard1" || name === "discard2" || name === "pending") return pileItem(name)
        return targetItem(name)
    }

    function flyCard(fromName, toName, cardName, delayMs) {
        var src = itemCenter(sourceItem(fromName))
        var dst = itemCenter(targetItem(toName))
        var obj = flyingCardComponent.createObject(root, {
            "cardName": cardName,
            "z": 220,
            "visible": true
        })
        if (obj) obj.launch(src.x, src.y, dst.x, dst.y, delayMs || 0)
    }

    Connections {
        target: game
        function onDrawAnimationRequested(sourcePile, cardName) {
            root.flyCard(sourcePile, root.targetZoneForCard(cardName), cardName, 0)
        }
        function onDiscardAnimationRequested(targetPile, cardName) {
            root.flyCard("pending", targetPile, cardName, 130)
        }
        function onPendingDrawAnimationRequested(firstCardName, secondCardName) {
            root.flyCard("main", "pending", firstCardName, 0)
            root.flyCard("main", "pending", secondCardName, 140)
        }
        function onZoneAnimationRequested(sourceArea, targetArea, cardName) {
            root.flyCard(sourceArea, targetArea, cardName, 0)
        }
    }

    Component {
        id: flyingCardComponent
        GameCard {
            id: fly
            property real toX: 0
            property real toY: 0
            property int delayBeforeStart: 0
            property real lift: -28
            property real startRotation: -6 + Math.random() * 12
            opacity: 0
            scale: 0.78
            hovered: true
            rotation: startRotation

            Rectangle {
                anchors.fill: parent
                anchors.margins: -6
                radius: parent.radius + 8
                color: "#f7e6b2"
                opacity: fly.opacity > 0 ? 0.20 : 0
                z: -1
                Behavior on opacity { NumberAnimation { duration: 180 } }
            }

            function launch(sx, sy, tx, ty, delayMs) {
                x = sx - width / 2
                y = sy - height / 2
                toX = tx - width / 2
                toY = ty - height / 2
                delayBeforeStart = delayMs
                flyAnim.start()
            }

            SequentialAnimation {
                id: flyAnim
                PauseAnimation { duration: fly.delayBeforeStart }
                ParallelAnimation {
                    NumberAnimation { target: fly; property: "opacity"; from: 0; to: 1; duration: 90; easing.type: Easing.OutCubic }
                    SequentialAnimation {
                        NumberAnimation { target: fly; property: "y"; to: Math.min(fly.y, fly.toY) + fly.lift; duration: 210; easing.type: Easing.OutCubic }
                        NumberAnimation { target: fly; property: "y"; to: fly.toY + 8; duration: 360; easing.type: Easing.InOutCubic }
                        NumberAnimation { target: fly; property: "y"; to: fly.toY; duration: 150; easing.type: Easing.OutBack }
                    }
                    SequentialAnimation {
                        PauseAnimation { duration: 55 }
                        NumberAnimation { target: fly; property: "x"; to: fly.toX; duration: 650; easing.type: Easing.InOutCubic }
                    }
                    SequentialAnimation {
                        NumberAnimation { target: fly; property: "scale"; to: 1.13; duration: 200; easing.type: Easing.OutBack }
                        NumberAnimation { target: fly; property: "scale"; to: 0.96; duration: 330; easing.type: Easing.InOutCubic }
                        NumberAnimation { target: fly; property: "scale"; to: 0.86; duration: 120; easing.type: Easing.OutCubic }
                    }
                    SequentialAnimation {
                        NumberAnimation { target: fly; property: "rotation"; to: -fly.startRotation * 0.4; duration: 240; easing.type: Easing.OutCubic }
                        NumberAnimation { target: fly; property: "rotation"; to: 0; duration: 360; easing.type: Easing.OutBack }
                    }
                    SequentialAnimation {
                        PauseAnimation { duration: 560 }
                        NumberAnimation { target: fly; property: "opacity"; to: 0; duration: 170; easing.type: Easing.InCubic }
                    }
                }
                ScriptAction { script: fly.destroy() }
            }
        }
    }

    GameHeader {
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        onOpenScore: root.openScore()
        onOpenSettings: root.openSettings()
        onOpenTutorial: root.openTutorial()
        onOpenPause: root.openPause()
    }

    Rectangle {
        id: table
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.margins: 14
        radius: 12
        color: "#def4f4"
        border.color: "#b9d8d7"
        border.width: 2

        Text {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            text: "🐚  🐟  ⚓"
            color: "#d4ebeb"
            font.pixelSize: 80
            opacity: 0.55
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 28

            PlayedPairStrip { id: playedArea; Layout.preferredWidth: 260; Layout.fillHeight: true }

            PileStrip {
                id: piles
                Layout.fillWidth: true
                Layout.preferredWidth: 820
                Layout.maximumWidth: 860
                Layout.fillHeight: true
            }

            GameActionBar {
                Layout.preferredWidth: 270
                Layout.alignment: Qt.AlignVCenter
            }
        }
    }

    Rectangle {
        id: footer
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 250
        color: "transparent"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 14

            BuffStrip { id: buffArea; Layout.preferredWidth: 330; Layout.fillHeight: true }
            HandStrip { id: handArea; Layout.fillWidth: true; Layout.fillHeight: true }
            CollectStrip { id: collectArea; Layout.preferredWidth: 470; Layout.fillHeight: true }
        }
    }

    Rectangle {
        id: pendingBox
        visible: game.pendingDrawActive
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: footer.top
        anchors.bottomMargin: 16
        width: 540
        height: 230
        radius: 22
        color: "#fffaf2"
        border.color: "#e0c478"
        border.width: 2
        opacity: visible ? 1 : 0
        scale: visible ? 1 : 0.96
        z: 20

        Behavior on opacity { NumberAnimation { duration: 180 } }
        Behavior on scale { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 10
            Text { Layout.alignment: Qt.AlignHCenter; text: "抽2弃1：选择保留牌"; font.pixelSize: 20; font.bold: true; color: "#5a6d70" }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 24
                Repeater {
                    model: game.pendingDrawCardNames
                    delegate: Item {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 160
                        opacity: 0
                        scale: 0.88

                        SequentialAnimation on opacity {
                            running: true
                            PauseAnimation { duration: index * 90 }
                            NumberAnimation { to: 1; duration: 170; easing.type: Easing.OutCubic }
                        }
                        SequentialAnimation on scale {
                            running: true
                            PauseAnimation { duration: index * 90 }
                            NumberAnimation { to: 1; duration: 210; easing.type: Easing.OutBack }
                        }

                        GameCard {
                            id: pendingCard
                            anchors.centerIn: parent
                            cardName: modelData
                            selected: index === game.pendingKeepIndex
                            hovered: pendingMouse.containsMouse
                            pressed: pendingMouse.pressed
                        }
                        MouseArea {
                            id: pendingMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: game.selectPendingKeep(index)
                        }
                    }
                }
            }
            Text { Layout.alignment: Qt.AlignHCenter; text: "选中保留牌后，点击弃牌堆1/2完成弃置"; color: "#7b8b8d"; font.pixelSize: 14 }
        }
    }

    Rectangle {
        visible: game.computerTurn
        anchors.top: table.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 14
        width: 270
        height: 48
        radius: 24
        color: "#4b8587"
        opacity: 0.92
        z: 30
        Text { anchors.centerIn: parent; text: "电脑回合中…"; color: "white"; font.pixelSize: 18; font.bold: true }
    }

}