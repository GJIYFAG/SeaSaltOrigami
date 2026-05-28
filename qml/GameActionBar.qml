import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 14

    GameTextButton {
        Layout.preferredWidth: 250
        text: game.pendingDrawActive ? "弃到堆1  ›" : "抽取堆1  ›"
        primary: true
        enabledButton: !game.computerTurn && (game.pendingDrawActive || game.drawDiscard1Enabled)
        onClicked: game.pendingDrawActive ? game.finishPendingMainDraw(1) : game.drawDiscard1()
    }

    GameTextButton {
        Layout.preferredWidth: 250
        text: game.pendingDrawActive ? "弃到堆2  ›" : "抽取堆2  ›"
        muted: true
        enabledButton: !game.computerTurn && (game.pendingDrawActive || game.drawDiscard2Enabled)
        onClicked: game.pendingDrawActive ? game.finishPendingMainDraw(2) : game.drawDiscard2()
    }

    GameTextButton {
        Layout.preferredWidth: 250
        text: game.selectedHandCount === 2 ? "▶  出牌" : "▶  选择2张后出牌"
        primary: game.selectedHandCount === 2
        muted: game.selectedHandCount !== 2
        enabledButton: !game.computerTurn && !game.pendingDrawActive && game.selectedHandCount === 2
        onClicked: game.play()
    }

    GameTextButton {
        Layout.preferredWidth: 250
        text: "⚑  结束回合"
        muted: true
        enabledButton: !game.computerTurn && !game.pendingDrawActive
        onClicked: game.endTurn()
    }

    GameTextButton {
        Layout.preferredWidth: 250
        visible: game.roundEndVisible
        text: "结束本轮"
        danger: true
        enabledButton: !game.computerTurn && !game.pendingDrawActive
        onClicked: game.endRound()
    }
}
