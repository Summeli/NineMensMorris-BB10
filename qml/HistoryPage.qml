import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "UIConstants.js" as UI

Page {
    id: settingsPage
    orientationLock: PageOrientation.LockPortrait
    anchors.margins: UI.DEFAULT_MARGIN

    ListView {
        id: moveList
        width: parent.width
        height: parent.height

        section.property: "section"

        model: board.game.history
        delegate: ListDelegate { }
    }

    FastScroll {
        id: scrolldecorator
        listView: moveList
    }

    Label {
        anchors.centerIn: parent
        color: UI.COLOR_SECONDARY_FOREGROUND
        font.family: "Nokia Pure Text Light"
        font.pixelSize: 64
        visible: moveList.count == 0
        //% "No moves yet"
        text: qsTrId("hist_empty")
    }

    tools: ToolBarLayout {
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back";
            onClicked: pageStack.pop()
        }
    }
}
