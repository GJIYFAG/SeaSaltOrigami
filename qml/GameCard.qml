import QtQuick

Rectangle {
    id: root
    property string cardName: "空"
    property bool selected: false
    property bool hovered: false
    property bool pressed: false
    property bool back: false
    property bool large: false
    property bool compact: false
    property string footerText: ""
    readonly property bool isSeahorse: cardName.indexOf("海马") >= 0

    width: compact ? 76 : (large ? 126 : 108)
    height: compact ? 104 : (large ? 174 : 150)
    radius: 14
    color: back ? "#65c0c5" : "#fffdf5"
    scale: pressed ? 0.985 : (selected ? 1.075 : (hovered ? 1.035 : 1.0))
    border.width: selected ? 4 : (hovered ? 3 : 2)
    border.color: selected ? "#e4bd67" : (hovered ? "#74c9c4" : colorForName(cardName))

    Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }
    Behavior on border.color { ColorAnimation { duration: 150 } }
    Behavior on opacity { NumberAnimation { duration: 140 } }

    function colorForName(n) {
        if (n.indexOf("螃蟹") >= 0 || n.indexOf("章鱼") >= 0 || n.indexOf("龙虾") >= 0) return "#d9a097"
        if (n.indexOf("贝壳") >= 0 || n.indexOf("美人鱼") >= 0) return "#e0c478"
        if (n.indexOf("海草") >= 0 || n.indexOf("海马") >= 0 || n.indexOf("企鹅") >= 0) return "#a9c891"
        if (n.indexOf("鱼") >= 0 || n.indexOf("鲨鱼") >= 0 || n.indexOf("海龟") >= 0 || n.indexOf("船锚") >= 0) return "#93c3ce"
        if (n.indexOf("船") >= 0) return "#9fb99d"
        return "#d7cbbb"
    }

    function iconForName(n) {
        if (back) return "🐚"
        if (n.indexOf("美人鱼") >= 0) return "🧜"
        if (n.indexOf("章鱼") >= 0) return "🐙"
        if (n.indexOf("螃蟹") >= 0) return "🦀"
        if (n.indexOf("龙虾") >= 0) return "🦞"
        if (n.indexOf("海马") >= 0) return ""
        if (n.indexOf("企鹅") >= 0) return "🐧"
        if (n.indexOf("船锚") >= 0 || n.indexOf("锚") >= 0) return "⚓"
        if (n.indexOf("贝壳") >= 0) return "🐚"
        if (n.indexOf("鲨鱼") >= 0) return "🦈"
        if (n.indexOf("海龟") >= 0) return "🐢"
        if (n.indexOf("水母") >= 0) return "🪼"
        if (n.indexOf("游泳") >= 0) return "🏊"
        if (n.indexOf("鱼") >= 0) return "🐟"
        if (n.indexOf("船") >= 0) return "⛵"
        if (n.indexOf("问号") >= 0) return "?"
        if (n.indexOf("群") >= 0 || n.indexOf("队") >= 0 || n.indexOf("堆") >= 0) return "✦"
        return "✧"
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 7
        radius: root.radius - 5
        color: back ? "transparent" : "#fffdf7"
        border.color: back ? "#d9ffff" : "#eee2cf"
        border.width: 1
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 5
        radius: root.radius - 4
        color: selected ? "#ffe9a8" : "#8bd8d5"
        opacity: selected ? 0.13 : (hovered ? 0.10 : 0.0)
        visible: opacity > 0
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 20
        radius: root.radius
        color: "#000000"
        opacity: hovered || selected ? 0.055 : 0
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    Rectangle {
        id: hoverGlow
        anchors.fill: parent
        anchors.margins: 4
        radius: root.radius - 3
        color: "transparent"
        border.color: "#f5dfad"
        border.width: 2
        opacity: hovered && !selected ? 0.18 : 0
        Behavior on opacity { NumberAnimation { duration: 180 } }
    }

    Rectangle {
        id: shimmer
        width: parent.width * 0.34
        height: parent.height * 1.25
        rotation: 18
        x: hovered || selected ? parent.width + 8 : -width - 20
        y: -12
        color: "#ffffff"
        opacity: hovered || selected ? 0.18 : 0
        Behavior on x { NumberAnimation { duration: 620; easing.type: Easing.InOutCubic } }
        Behavior on opacity { NumberAnimation { duration: 160 } }
    }

    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: compact ? 7 : 10
        text: back ? "" : cardName.match(/[0-9]+/) ? cardName.match(/[0-9]+/)[0] : ""
        font.pixelSize: compact ? 16 : 24
        font.bold: true
        color: root.colorForName(cardName)
    }

    Text {
        id: icon
        visible: !root.isSeahorse
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: compact ? -9 : -14
        text: root.iconForName(cardName)
        font.pixelSize: compact ? 30 : (large ? 54 : 44)
    }

    Canvas {
        id: seahorseCanvas
        visible: root.isSeahorse && !back
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: compact ? -10 : -16
        width: compact ? 34 : (large ? 62 : 52)
        height: compact ? 44 : (large ? 78 : 66)
        opacity: 0.92
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.lineCap = "round"
            ctx.lineJoin = "round"
            ctx.strokeStyle = "#6aaeb5"
            ctx.fillStyle = "#cfecef"
            ctx.lineWidth = Math.max(2, width * 0.075)

            // 身体曲线
            ctx.beginPath()
            ctx.moveTo(width * 0.58, height * 0.16)
            ctx.bezierCurveTo(width * 0.33, height * 0.16, width * 0.30, height * 0.42, width * 0.48, height * 0.50)
            ctx.bezierCurveTo(width * 0.70, height * 0.60, width * 0.62, height * 0.82, width * 0.38, height * 0.78)
            ctx.bezierCurveTo(width * 0.20, height * 0.75, width * 0.22, height * 0.58, width * 0.43, height * 0.62)
            ctx.stroke()

            // 头部
            ctx.save()
            ctx.translate(width * 0.56, height * 0.19)
            ctx.rotate(-0.25)
            ctx.scale(1.2, 0.9)
            ctx.beginPath()
            ctx.arc(0, 0, width * 0.13, 0, Math.PI * 2)
            ctx.fill()
            ctx.stroke()
            ctx.restore()

            // 嘴部
            ctx.beginPath()
            ctx.moveTo(width * 0.67, height * 0.18)
            ctx.lineTo(width * 0.91, height * 0.15)
            ctx.lineTo(width * 0.68, height * 0.24)
            ctx.stroke()

            // 背鳍
            ctx.beginPath()
            ctx.moveTo(width * 0.43, height * 0.37)
            ctx.lineTo(width * 0.20, height * 0.30)
            ctx.lineTo(width * 0.35, height * 0.48)
            ctx.closePath()
            ctx.fillStyle = "#e8d6a6"
            ctx.fill()
            ctx.stroke()

            // 冠刺
            ctx.strokeStyle = "#d6b973"
            ctx.lineWidth = Math.max(1.5, width * 0.045)
            for (var i = 0; i < 4; i++) {
                var px = width * (0.44 + i * 0.05)
                ctx.beginPath()
                ctx.moveTo(px, height * 0.08)
                ctx.lineTo(px + width * 0.02, height * 0.015)
                ctx.stroke()
            }

            // 眼睛与腹纹
            ctx.fillStyle = "#476e75"
            ctx.beginPath()
            ctx.arc(width * 0.60, height * 0.16, Math.max(1.2, width * 0.025), 0, Math.PI * 2)
            ctx.fill()
            ctx.strokeStyle = "#8ec9cd"
            ctx.lineWidth = Math.max(1, width * 0.03)
            for (var j = 0; j < 4; j++) {
                ctx.beginPath()
                ctx.moveTo(width * (0.47 + j * 0.025), height * (0.33 + j * 0.07))
                ctx.lineTo(width * (0.58 + j * 0.015), height * (0.36 + j * 0.07))
                ctx.stroke()
            }
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: compact ? 14 : 24
        width: parent.width - 14
        horizontalAlignment: Text.AlignHCenter
        text: back ? "" : cardName.replace(/[0-9]+$/, "")
        font.pixelSize: compact ? 12 : 16
        font.bold: true
        color: "#4f656b"
        elide: Text.ElideRight
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7
        text: footerText
        visible: footerText.length > 0 && !back
        font.pixelSize: compact ? 10 : 12
        color: "#7a8b8d"
    }

    Rectangle {
        visible: selected
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -10
        width: 54
        height: 18
        radius: 9
        color: "#58b8b3"
        Text { anchors.centerIn: parent; text: "已选"; color: "white"; font.pixelSize: 11; font.bold: true }
    }
}
