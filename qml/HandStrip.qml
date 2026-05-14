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
        anchors.margins: 12
        spacing: 6

        RowLayout {
            Layout.fillWidth: true
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: game.selectedHandCount > 0 ? ("手牌（已选 " + game.selectedHandCount + "/2，拖拽调整顺序）") : "手牌（拖拽调整顺序）"
                color: "#6a5b52"
                font.pixelSize: 19
                font.bold: true
            }
        }

        Flickable {
            id: flick
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            interactive: !dragLock.active
            contentWidth: handRow.width + 24
            boundsBehavior: Flickable.StopAtBounds

            QtObject { id: dragLock; property bool active: false }

            Row {
                id: handRow
                x: 12
                y: 10
                spacing: 14

                Repeater {
                    model: game.handCardNames
                    delegate: Item {
                        id: holder
                        width: 124
                        height: 182
                        property real homeX: (width - dragShell.width) / 2
                        property real homeY: game.handSelected[index] === true ? -10 : (mouse.containsMouse ? -6 : 0)
                        property bool wasDragged: false
                        property real pressX: 0
                        property int slotStep: width + handRow.spacing

                        Item {
                            id: dragShell
                            width: 108
                            height: 150
                            x: holder.homeX
                            y: holder.homeY
                            z: mouse.drag.active ? 20 : (game.handSelected[index] === true ? 8 : (mouse.containsMouse ? 6 : 1))

                            Behavior on x { enabled: !mouse.drag.active; NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                            Behavior on y { enabled: !mouse.drag.active; NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }

                            GameCard {
                                id: card
                                anchors.centerIn: parent
                                cardName: modelData
                                selected: game.handSelected[index] === true
                                hovered: mouse.containsMouse && !mouse.pressed && !mouse.drag.active
                                pressed: mouse.pressed && !mouse.drag.active
                            }
                        }

                        MouseArea {
                            id: mouse
                            anchors.fill: dragShell
                            enabled: !game.computerTurn && !game.pendingDrawActive
                            hoverEnabled: true
                            preventStealing: true
                            cursorShape: Qt.PointingHandCursor
                            drag.target: dragShell
                            drag.axis: Drag.XAxis
                            drag.threshold: 8

                            onPressed: {
                                holder.wasDragged = false
                                holder.pressX = dragShell.x
                                dragLock.active = true
                            }

                            onPositionChanged: {
                                if (Math.abs(dragShell.x - holder.pressX) > 10) {
                                    holder.wasDragged = true
                                }
                            }

                            onReleased: {
                                dragLock.active = false
                                if (holder.wasDragged) {
                                    var deltaSlots = Math.round((dragShell.x - holder.homeX) / holder.slotStep)
                                    var target = index + deltaSlots
                                    target = Math.max(0, Math.min(game.handCardNames.length - 1, target))
                                    if (target !== index) {
                                        game.moveHandCard(index, target)
                                    } else {
                                        dragShell.x = holder.homeX
                                        dragShell.y = holder.homeY
                                    }
                                } else {
                                    game.toggleHandCard(index)
                                    dragShell.x = holder.homeX
                                    dragShell.y = holder.homeY
                                }
                            }

                            onCanceled: {
                                dragLock.active = false
                                dragShell.x = holder.homeX
                                dragShell.y = holder.homeY
                            }

                            onExited: {
                                if (!mouse.pressed) {
                                    dragShell.y = holder.homeY
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
