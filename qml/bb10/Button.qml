import QtQuick 1.0

BorderImage {
    id: button

    property alias text: buttonTitle.text

    signal clicked()

    source: buttonArea.pressed ? "../images/piece-white.png": "../images/piece-black.png"
    border { left: 10; right: 10; top: 10; bottom: 10 }
    width: 240
    height: 70

    Text {
        id: buttonTitle
        anchors.centerIn: parent
        color: "white"
        font.pointSize: 18
    }

    MouseArea {
        id: buttonArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}
