import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: win
    width: 1536
    height: 960
    visible: true
    title: game.windowTitleText
    color: "#e8f6f5"

    property bool settingsOpen: false
    property bool tutorialOpen: false
    property bool pauseOpen: false

    Component.onCompleted: forceActiveFocus()
    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_Escape) {
            if (settingsOpen) settingsOpen = false
            else if (tutorialOpen) tutorialOpen = false
            else if (game.gameActive) pauseOpen = !pauseOpen
            event.accepted = true
        }
    }

    MainMenu {
        visible: !game.gameActive
        onSettings: settingsOpen = true
        onTutorial: tutorialOpen = true
    }

    GamePlay {
        visible: game.gameActive
        onOpenScore: game.showScoreDetail()
        onOpenSettings: settingsOpen = true
        onOpenTutorial: tutorialOpen = true
        onOpenPause: pauseOpen = true
    }

    Rectangle {
        id: pauseOverlay
        visible: game.gameActive && pauseOpen
        anchors.fill: parent
        color: "#33000000"
        z: 90

        Rectangle {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 34
            anchors.topMargin: 118
            width: 300
            height: 405
            radius: 12
            color: "#17333d"
            opacity: 0.96

            Column {
                anchors.fill: parent
                anchors.margins: 22
                spacing: 14

                Row {
                    width: parent.width
                    Text { text: "游戏已暂停"; color: "white"; font.pixelSize: 18; font.bold: true; width: parent.width - 32; horizontalAlignment: Text.AlignHCenter }
                    Text { text: "×"; color: "white"; font.pixelSize: 30; MouseArea { anchors.fill: parent; onClicked: pauseOpen = false } }
                }

                GameTextButton { width: parent.width; text: "▥   得分详情"; onClicked: { pauseOpen = false; game.showScoreDetail() } }
                GameTextButton { width: parent.width; text: "⚙   游戏设置"; onClicked: { pauseOpen = false; settingsOpen = true } }
                GameTextButton { width: parent.width; text: "▤   游戏教程"; onClicked: { pauseOpen = false; tutorialOpen = true } }
                GameTextButton { width: parent.width; text: "⌂   返回主界面"; danger: true; onClicked: { pauseOpen = false; game.returnToMenu() } }

                Text { width: parent.width; horizontalAlignment: Text.AlignHCenter; text: "按 ESC 键返回游戏"; color: "#b9c9cb"; font.pixelSize: 14 }
            }
        }
    }

    SettingsScreen {
        visible: settingsOpen
        z: 100
        onClose: settingsOpen = false
    }

    TutorialScreen {
        visible: tutorialOpen
        z: 100
        onClose: tutorialOpen = false
    }

    Rectangle {
        visible: game.toastVisible
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 28
        width: Math.min(parent.width - 80, Math.max(360, toastText.implicitWidth + 60))
        height: 54
        radius: 27
        color: "#17333d"
        opacity: 0.94
        z: 120
        Text { id: toastText; anchors.centerIn: parent; text: game.toastText; color: "white"; font.pixelSize: 16; font.bold: true }
        MouseArea { anchors.fill: parent; onClicked: game.dismissToast() }
    }
}
