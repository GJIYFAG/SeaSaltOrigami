import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal settings()
    signal tutorial()
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#e7f7f4" }
        GradientStop { position: 0.58; color: "#fbf0da" }
        GradientStop { position: 1.0; color: "#d8ece8" }
    }

    Canvas {
        anchors.fill: parent
        opacity: 0.34
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.strokeStyle = "#5fb6b1"
            ctx.lineWidth = 2
            for (var y = 70; y < height; y += 58) {
                ctx.beginPath()
                for (var x = -30; x <= width + 30; x += 18) {
                    var py = y + Math.sin((x + y) / 42) * 7
                    if (x === -30) ctx.moveTo(x, py)
                    else ctx.lineTo(x, py)
                }
                ctx.stroke()
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 560
        height: 520
        radius: 8
        color: "#fffaf0"
        border.color: "#91c6c3"
        border.width: 2

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 9
            radius: 8
            color: "#df8f73"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 44
            spacing: 18

            Text { Layout.alignment: Qt.AlignHCenter; text: "海盐折纸"; color: "#2f8587"; font.pixelSize: 44; font.bold: true }
            Text { Layout.alignment: Qt.AlignHCenter; text: "Sea Salt Origami"; color: "#71888a"; font.pixelSize: 18 }
            Item { Layout.preferredHeight: 14 }
            MenuPrimaryButton { Layout.alignment: Qt.AlignHCenter; text: "单人模式"; onClicked: game.startGame(true) }
            MenuPrimaryButton { Layout.alignment: Qt.AlignHCenter; text: "双人本地"; secondary: true; onClicked: game.startGame(false) }
            Item { Layout.fillHeight: true }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 18
                MenuPrimaryButton { width: 180; text: "教程"; secondary: true; onClicked: root.tutorial() }
                MenuPrimaryButton { width: 180; text: "设置"; secondary: true; onClicked: root.settings() }
            }
        }
    }
}
