import QtQuick 1.0
import Muehle 1.0

Item {
    id: mainPage

    // Export the settings for the appWindow, SettingsPage and Board to use
    property alias settings: settings

    property bool faceToFace: settings.faceToFace && !(settings.computerBlack || settings.computerWhite)

    Image {
        id: background
        source: "../images/background.png"
    }

    Rectangle {
        id: header
        height: 70
        anchors { left: parent.left; right: parent.right }
        color: "#0036880b"

        Text {
            id: status
            x: 16
            y: 16
            font.pixelSize: 32
            font.weight: Font.Normal
            color: "black"
            text: "Nine Men's Morris"
        }

        Rectangle {
            id: errorHeader
            height: 70
            y: -82
            opacity: 0
            width: 480
            anchors { left: parent.left; }
            color: "#f91d00"

            Text {
                id: errorLabel
                x: 16
                y: 16
                font.pixelSize: 32
                color: "#ffffff"
                text: ""
            }
        }

        Rectangle {
            opacity: 0
            anchors { top: errorHeader.bottom; left: parent.left; }
            width: 480
            height: 12
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#7f000000" }
                GradientStop { position: 1.0; color: "#00000000" }
            }
        }
    }

    Board {
        id: board
        // Those are fixed to align with the background image
        y: 70
        height: 606

        onPromptChanged: { status.text = prompt ; status2.text = prompt }
        onErrorChanged: showError(error)
        onPlayerBlackChanged: hideError()
        onGameFinished: {
            winDialog.icon = playerBlack ? "../images/piece-white.png"
                                       : "../images/piece-black.png"
            if (!playerBlack) {
                winDialog.titleText = "Black wins"
                winDialog.message = "The black player wins in %d moves. ".replace("%d", board.ply) +
                                    "Do you want to start a new game?"
            } else {
                winDialog.titleText = "White wins"
                winDialog.message = "The white player wins in %d moves. ".replace("%d", board.ply) +
                                    "Do you want to start a new game?"
            }
            winDialog.open()
        }
    }

    QueryDialog {
        id: winDialog
        acceptButtonText: "Start new game"
        onAccepted: board.startNewGame()
        onRejected: startNewRect.visible = true
    }

    Rectangle {
        id: startNewRect
        anchors { left: parent.left; right: parent.right; bottom: board.bottom }
        height: 70
        color: "black"
        visible: false

        Button {
            anchors.centerIn: parent
            text: "Start new game"
            onClicked: { board.startNewGame(); startNewRect.visible = false }
        }
    }
        Rectangle {
            id: aboutButton
            anchors { left: parent.left; top: background.bottom }
            width: 480
            height: 70
            color: "#333"
            Button {
                anchors.centerIn: parent
                text: "About"
                onClicked: { viewcontroller.showAboutDialog(); }
            }
        }

    Rectangle {
        id: footer
        height: 70
        y: parent.height
        z: 1 // To get the errorFooter above the view menu tool icon
        visible: board.playerBlack && faceToFace && header.visible
        width: parent.width
        rotation: 180
        color: "#0036880b"
        Text {
            id: status2
            x: 16
            y: 16
            font.pixelSize: 32
            color: "black"
            text: "Place a black piece"
        }

        Rectangle {
            id: errorFooter
            height: 70
            y: -70
            anchors { left: parent.left; right: parent.right }
            color: "#f91d00"

            Text {
                id: errorLabel2
                x: 16
                y: 16
                font.pixelSize: 32
                color: "white"
                text: "Invalid move!"
            }
        }
    }

    NumberAnimation {
        id: errorAnimation
        targets: [errorHeader, errorFooter]
        property: "y"
        duration: 250
        easing.type: Easing.InOutExpo
    }

    Timer {
        id: errorTimer
        interval: 2000
        repeat: false
        onTriggered: {
            errorAnimation.to = -82
            errorAnimation.start()
            stop()
        }
    }

    Settings {
        id: settings

        Component.onCompleted: load()
    }

    states: [
        State {
            name: "playerBlack"
            when: footer.visible
            PropertyChanges {
                target: header
                y: -70
            }
            PropertyChanges {
                target: footer
                y: parent.height - height
            }
        }
    ]

    transitions: Transition {
        NumberAnimation {
            properties: "y"
            duration: 250
            easing.type: Easing.InOutExpo
        }
    }

    function startNewGame() {
        hideError()
        board.startNewGame()
    }

    function showError(error) {
        if (error == "")
            return
        console.log("Error: " + error)
        errorLabel.text = error
        errorLabel2.text = error
        errorAnimation.to = 0
        errorAnimation.start()
        errorTimer.restart()
    }

    function hideError() {
        errorAnimation.stop()
        errorTimer.stop()
        errorFooter.y = -82
        errorHeader.y = -82
    }
}
