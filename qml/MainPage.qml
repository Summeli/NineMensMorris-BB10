import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI
import Muehle 1.0

Page {
    id: mainPage
    orientationLock: PageOrientation.LockPortrait

    // Export the settings for the appWindow, SettingsPage and Board to use
    property alias settings: settings

    property bool faceToFace: settings.faceToFace && !(settings.computerBlack || settings.computerWhite)

    Image {
        id: background
        source: "../images/background.png"
    }

    Rectangle {
        id: header
        height: UI.HEADER_DEFAULT_HEIGHT_PORTRAIT
        anchors { left: parent.left; right: parent.right }
        color: "#0036880b"

        Label {
            id: status
            x: UI.DEFAULT_MARGIN
            y: UI.HEADER_DEFAULT_TOP_SPACING_PORTRAIT
            font.pixelSize: UI.FONT_XLARGE
            font.weight: Font.Normal
            color: "black"
            //% "Nine Men's Morris"
            text: qsTrId("qtn_game_appname_qmuehle")
        }
        // A busy indicator to be shown when the toolbar is hidden
        BusyIndicator {
            platformStyle: BusyIndicatorStyle { size: "small" }
            anchors { left: status.right; verticalCenter: status.verticalCenter; leftMargin: UI.DEFAULT_MARGIN }
            visible: !showToolBar && board.busy && settings.difficulty >= 3
            running: visible
        }

        Rectangle {
            id: errorHeader
            height: UI.HEADER_DEFAULT_HEIGHT_PORTRAIT
            y: -82
            anchors { left: parent.left; right: parent.right }
            color: "#f91d00"

            Label {
                id: errorLabel
                x: UI.DEFAULT_MARGIN
                y: UI.HEADER_DEFAULT_TOP_SPACING_PORTRAIT
                font.pixelSize: UI.FONT_XLARGE
                color: "#ffffff"
                text: ""
            }
        }

        Image {
            source: "image://theme/statusbar-dropshadow"
            anchors { top: errorHeader.bottom; left: parent.left; right: parent.right }
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
                //% "Black wins"
                winDialog.titleText = qsTrId("win_black")
                //% "The black player wins in %d moves. "
                winDialog.message = qsTrId("win_black_moves").replace("%d", board.ply) +
                                    //% "Do you want to start a new game?"
                                    qsTrId("ask_start_new")
            } else {
                //% "White wins"
                winDialog.titleText = qsTrId("win_white")
                //% "The white player wins in %d moves. "
                winDialog.message = qsTrId("win_white_moves").replace("%d", board.ply) +
                                    //% "Do you want to start a new game?"
                                    qsTrId("ask_start_new")
            }
            winDialog.open()
        }
    }

    QueryDialog {
        id: winDialog
        //% "Start new game"
        acceptButtonText: qsTrId("btn_start_new")
        onAccepted: board.startNewGame()
    }

    Rectangle {
        id: footer
        height: UI.HEADER_DEFAULT_HEIGHT_PORTRAIT
        y: parent.height
        z: 1 // To get the errorFooter above the view menu tool icon
        visible: board.playerBlack && faceToFace && header.visible
        width: parent.width
        rotation: 180
        color: "#0036880b"
        Label {
            id: status2
            x: UI.DEFAULT_MARGIN + (fullScreenToolIcon.visible ? 64 : 0)
            y: UI.HEADER_DEFAULT_TOP_SPACING_PORTRAIT
            font.pixelSize: UI.FONT_XLARGE
            color: "black"
            //% "Place a black piece"
            text: qsTrId("place_black")
        }

        Rectangle {
            id: errorFooter
            height: UI.HEADER_DEFAULT_HEIGHT_PORTRAIT
            y: -82
            anchors { left: parent.left; right: parent.right }
            color: "#f91d00"

            Label {
                id: errorLabel2
                x: UI.DEFAULT_MARGIN
                y: UI.HEADER_DEFAULT_TOP_SPACING_PORTRAIT
                font.pixelSize: UI.FONT_XLARGE
                color: "white"
                //% "Invalid move!"
                text: qsTrId("invalid_move")
            }
        }
    }

    tools: ToolBarLayout {
        BusyIndicator {
            platformStyle: BusyIndicatorStyle { size: "small" }
            anchors.horizontalCenter: parent.horizontalCenter
            visible: board.busy && settings.difficulty >= 3
            running: visible
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu";
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    // A view menu tool icon to be shown when the toolbar is hidden
    ToolIcon {
        id: fullScreenToolIcon
        platformIconId: "toolbar-view-menu";
        visible: !showToolBar
        opacity: 0.8
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                //% "New game"
                text: qsTrId("menu_new_game")
                onClicked: startNewGame()
            }
            MenuItem {
                //% "History
                text: qsTrId("menu_history")
                onClicked: pageStack.push(Qt.createComponent("HistoryPage.qml"));
            }
            MenuItem {
                //% "Settings"
                text: qsTrId("menu_settings")
                onClicked: pageStack.push(Qt.createComponent("SettingsPage.qml"));
            }
            MenuItem {
                //% "About"
                text: qsTrId("menu_about")
                onClicked: pageStack.push(Qt.createComponent("AboutPage.qml"));
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

    function updateActive() {
        header.visible = platformWindow.active || !settings.fullScreen
        if (!platformWindow.active) {
            settings.save()
            // TODO - save game state
            // board.save()
        }
    }

    Connections {
        target: platformWindow
        //onViewModeChanged: updateViewMode()
        //onVisibleChanged: updateVisible()
        onActiveChanged: updateActive()
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
