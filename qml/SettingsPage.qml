import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Page {
    id: settingsPage
    orientationLock: PageOrientation.LockPortrait
    anchors.margins: UI.DEFAULT_MARGIN

    Column {
        spacing: UI.DEFAULT_MARGIN
        anchors.fill: parent

        Item {
            anchors { left: parent.left; right: parent.right }
            height: UI.LIST_ITEM_HEIGHT_DEFAULT

            Label {
                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                //% "White player: "
                text: qsTrId("sett_player_white") +
                      //% "Computer"
                      (player1AI.checked ? qsTrId("computer")
                                           //% "Human"
                                         : qsTrId("human"))
            }
            Switch {
                id: player1AI
                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                onCheckedChanged: {
                    if (checked && player2AI.checked)
                        player2AI.checked = false
                }
            }
        }

        Item {
            anchors { left: parent.left; right: parent.right }
            height: UI.LIST_ITEM_HEIGHT_DEFAULT

            Label {
                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                //% "Black player: "
                text: qsTrId("sett_player_black") +
                      //% "Computer"
                      (player2AI.checked ? qsTrId("computer")
                                           //% "Human"
                                         : qsTrId("human"))
            }
            Switch {
                id: player2AI
                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                onCheckedChanged: {
                    if (checked && player1AI.checked)
                        player1AI.checked = false
                }
            }
        }

        Item {
            anchors { left: parent.left; right: parent.right }
            height: UI.LIST_ITEM_HEIGHT_DEFAULT

            CheckBox {
                id: faceToFace
                anchors.verticalCenter: parent.verticalCenter
                enabled: !(player1AI.checked || player2AI.checked)
                //% "Play face to face"
                text: qsTrId("sett_face_to_face")
            }
        }


        Item {
            anchors { left: parent.left; right: parent.right }
            height: UI.LIST_ITEM_HEIGHT_DEFAULT

            Label {
                id: difficultyLabel
                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                //% "Difficulty"
                text: qsTrId("sett_difficulty")
            }
            Slider {
                id: difficultySlider
                anchors { left: difficultyLabel.right; right: parent.right; verticalCenter: parent.verticalCenter }
                enabled: player1AI.checked || player2AI.checked
                valueIndicatorVisible: true
                valueIndicatorText: value
                minimumValue: 1
                maximumValue: 5
                stepSize: 1
            }
        }

        Item {
            anchors { left: parent.left; right: parent.right }
            height: UI.LIST_ITEM_HEIGHT_DEFAULT

            CheckBox {
                id: fullScreen
                anchors.verticalCenter: parent.verticalCenter
                height: UI.LIST_ITEM_HEIGHT_DEFAULT
                //% "Fullscreen mode"
                text: qsTrId("sett_fullscreen")
                onCheckedChanged: appWindow.settings.fullScreen = checked
            }
        }
    }

    tools: ToolBarLayout {
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back";
            onClicked: {
                appWindow.settings.computerWhite = player1AI.checked
                appWindow.settings.computerBlack = player2AI.checked
                appWindow.settings.faceToFace = faceToFace.checked
                appWindow.settings.difficulty = difficultySlider.value
                pageStack.pop()
                board.maybeComputeMove()
            }
        }
    }

    Component.onCompleted: {
        player1AI.checked = appWindow.settings.computerWhite
        player2AI.checked = appWindow.settings.computerBlack
        faceToFace.checked = appWindow.settings.faceToFace
        difficultySlider.value = appWindow.settings.difficulty
        fullScreen.checked = appWindow.settings.fullScreen
    }
}
