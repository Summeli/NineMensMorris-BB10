import QtQuick 1.0

Item {
    id: dialog
    anchors.fill: parent
    visible: false

    property string acceptButtonText
    property alias icon: image.source
    property alias titleText: title.text
    property alias message: message.text

    signal accepted
    signal rejected

    Rectangle {
        anchors.fill: parent
        opacity: 0.8
        color: "black"
    }

    Image {
        id: image
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: title.top
    }

    Text {
        id: title
        text: "Title"
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: message.top
        font.pointSize: 26
    }

    Text {
        id: message
        text: "Message"
        color: "white"
        anchors.centerIn: parent
        font.pointSize: 18
    }

    MouseArea {
        anchors.fill: parent
        onClicked: { rejected(); dialog.visible = false }
    }

    Button {
        anchors { horizontalCenter: parent.horizontalCenter; top: message.bottom }
        text: acceptButtonText
        onClicked: { accepted(); dialog.visible = false }
    }

    function open() {
        dialog.visible = true
    }
}
