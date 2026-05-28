import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal openScore()
    signal openSettings()
    signal openTutorial()
    signal openPause()

    height: 152
    color: "#fbf7ed"

    function rawLine(i, fallback) {
        var lines = String(game.scoreText).split("\n")
        return lines.length > i ? lines[i] : fallback
    }
    function cleanLine(s) {
        return String(s).replace(/🏆|🔥|🎨/g, "").replace(/^-\s*/, "").trim()
    }
    function scoreValue(i, fallback) {
        var s = cleanLine(rawLine(i, fallback))
        var parts = s.split("：")
        return parts.length > 1 ? parts.slice(1).join("：") : s
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 36
        anchors.rightMargin: 26
        anchors.topMargin: 12
        anchors.bottomMargin: 10
        spacing: 22

        Rectangle {
            Layout.preferredWidth: 390
            Layout.fillHeight: true
            radius: 18
            color: "#fffdf7"
            border.color: "#eadcc6"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    ScoreTile { title: "总得分"; value: scoreValue(0, "总分：0") }
                    ScoreTile { title: "本轮得分"; value: scoreValue(1, "本轮分：0 / 10"); highlight: true }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 2
                    DetailLine { text: cleanLine(rawLine(2, "对牌得分：0分")) }
                    DetailLine { text: cleanLine(rawLine(3, "事件牌得分：0分")) }
                    DetailLine { text: cleanLine(rawLine(4, "收集牌总分：0分")) }
                    DetailLine { text: cleanLine(rawLine(5, "增益牌：0分")) }
                    DetailLine { text: cleanLine(rawLine(6, "美人鱼：0分")) }
                    DetailLine { text: cleanLine(rawLine(7, "颜色分：轮次后结算")) }
                }
            }
        }

        Item { Layout.fillWidth: true }

        Rectangle {
            Layout.preferredWidth: 420
            Layout.preferredHeight: 92
            radius: 28
            color: "#fffaf0"
            border.color: "#8abfbd"
            border.width: 2

            Column {
                anchors.centerIn: parent
                spacing: 4
                Text { anchors.horizontalCenter: parent.horizontalCenter; text: game.playerText; color: "#3c8e91"; font.pixelSize: 28; font.bold: true }
                Text { anchors.horizontalCenter: parent.horizontalCenter; text: game.allInPending ? "放手一搏最终回合" : "海盐折纸"; color: "#6b8587"; font.pixelSize: 16; font.bold: true }
            }
        }

        Item { Layout.fillWidth: true }

        RowLayout {
            Layout.preferredWidth: 455
            spacing: 18
            HeaderIconButton { label: "得分详情"; icon: "▥"; onClicked: root.openScore() }
            HeaderIconButton { label: "游戏设置"; icon: "⚙"; onClicked: root.openSettings() }
            HeaderIconButton { label: "游戏教程"; icon: "▤"; onClicked: root.openTutorial() }
            HeaderIconButton { label: "展开菜单"; icon: "☰"; onClicked: root.openPause() }
        }
    }

    component ScoreTile: Rectangle {
        property string title: ""
        property string value: "0"
        property bool highlight: false
        Layout.fillWidth: true
        Layout.preferredHeight: 44
        radius: 14
        color: highlight ? "#eefbfa" : "#fff8ea"
        border.color: highlight ? "#8bcac7" : "#ead7b5"
        border.width: 1
        Column {
            anchors.centerIn: parent
            spacing: 0
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: title; color: "#6f7e80"; font.pixelSize: 11; font.bold: true }
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: value; color: highlight ? "#2f9290" : "#765e4d"; font.pixelSize: 20; font.bold: true }
        }
    }

    component DetailLine: Text {
        Layout.fillWidth: true
        color: "#5d6c70"
        font.pixelSize: 13
        font.bold: true
        elide: Text.ElideRight
    }

    component HeaderIconButton: Item {
        id: hb
        property string label: ""
        property string icon: ""
        signal clicked()
        Layout.preferredWidth: 86
        Layout.preferredHeight: 112
        Column {
            anchors.centerIn: parent
            spacing: 7
            Rectangle {
                width: 68
                height: 68
                radius: 34
                color: mouse.containsMouse ? "#eefafa" : "#f7f6ef"
                border.color: mouse.containsMouse ? "#8bcac7" : "#d5ded7"
                scale: mouse.containsMouse ? 1.04 : 1.0
                Behavior on scale { NumberAnimation { duration: 140; easing.type: Easing.OutCubic } }
                Behavior on color { ColorAnimation { duration: 140 } }
                Text { anchors.centerIn: parent; text: hb.icon; font.pixelSize: 31; color: "#6c8183"; font.bold: true }
                MouseArea { id: mouse; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor; onClicked: hb.clicked() }
            }
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: hb.label; color: "#62777a"; font.pixelSize: 13; font.bold: true }
        }
    }
}
