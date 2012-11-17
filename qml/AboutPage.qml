import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Page {
    property string license: 'This program is free software: you can redistribute it and/or modify ' +
        'it under the terms of the GNU General Public License as published by ' +
        'the Free Software Foundation, either version 3 of the License, or ' +
        '(at your option) any later version.<br /><br />' +

        'This package is distributed in the hope that it will be useful, ' +
        'but WITHOUT ANY WARRANTY; without even the implied warranty of ' +
        'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ' +
        'GNU General Public License for more details.<br /><br />' +

        'You should have received a copy of the GNU General Public License ' +
        'along with this program. If not, see ' +
        '<a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a>'

    id: aboutPage
    orientationLock: PageOrientation.LockPortrait

    Flickable {
        id: flick
        anchors.fill: parent
        anchors {
            topMargin: UI.HEADER_DEFAULT_TOP_SPACING_PORTRAIT
            leftMargin: UI.DEFAULT_MARGIN
            rightMargin: UI.DEFAULT_MARGIN
        }
        contentHeight: column.height

        Column {
            id: column
            anchors {
                margins: UI.DEFAULT_MARGIN
                left: parent.left
                right: parent.right
            }
            spacing: UI.DEFAULT_MARGIN

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "file:///usr/share/icons/hicolor/64x64/apps/qmuehle.png"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: UI.FONT_XLARGE
                //% "Nine Men's Morris"
                text: qsTrId("qtn_game_appname_qmuehle") + " 0.1.2"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Copyright (c) 2011 Philipp Zabel"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "<a href='mailto:philipp.zabel@gmail.com'>philipp.zabel@gmail.com</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label {
                font.pixelSize: UI.FONT_LSMALL
                font.family: "Nokia Pure Text Light"
                width: parent.width
                wrapMode: Text.WordWrap
                text: license
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label {
                font.pixelSize: UI.FONT_LSMALL
                font.family: "Nokia Pure Text Light"
                width: parent.width
                wrapMode: Text.WordWrap
                text: "The source code of this program can be downloaded from Gitorious: " +
                      "<a href='https://gitorious.org/qmuehle'>https://gitorious.org/qmuehle</a><br /><br />"
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    ScrollDecorator {
        flickableItem: flick
        anchors.rightMargin: -UI.DEFAULT_MARGIN
    }

    tools: ToolBarLayout {
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back";
            onClicked: pageStack.pop()
        }
    }
}
