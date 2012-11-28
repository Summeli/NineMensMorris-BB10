import QtQuick 1.1

Item {
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
    height: 1200
    width: 768
    Rectangle {
        id: rectForPage
        anchors.fill: parent
        anchors {
            topMargin: 10
            leftMargin: 16
            rightMargin: 16
        }
        
	    MouseArea {
	        id: buttonArea
	        anchors.fill: parent
	        onClicked: viewcontroller.closeAboutDialog()
	    }
        Column {
            id: column
            anchors {
                margins: 20
                left: parent.left
                right: parent.right
            }
            spacing: 14

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../images/icon-m-muehle.png"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 16
                //% "Nine Men's Morris"
                text: "Nine Men's Morris" + " 1.0.0"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Copyright (c) 2011 Philipp Zabel"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "<a href='mailto:philipp.zabel@gmail.com'>philipp.zabel@gmail.com</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }
            
          Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "BlackBerry Port Antti Pohjola"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "<a href='mailto:summeli@summeli.fi'>summeli@summeli.fi</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                font.pixelSize: 14
                font.family: "Arial"
                width: parent.width
                wrapMode: Text.WordWrap
                text: license
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Text {
                font.pixelSize: 14
                font.family: "Arial"
                width: parent.width
                wrapMode: Text.WordWrap
                text: "The source code of the origal program can be downloaded from Gitorious: " +
                      "<a href='https://gitorious.org/qmuehle'>https://gitorious.org/qmuehle</a><br /><br />"
                onLinkActivated: Qt.openUrlExternally(link)
            }
           Text {
                font.pixelSize: 14
                font.family: "Arial"
                width: parent.width
                wrapMode: Text.WordWrap
                text: "The source code of the Blackberry port can be downloaded from the GitHub " +
                      "<a href='https://github.com/Summeli/NineMensMorris'>https://github.com/Summeli/NineMensMorris</a><br /><br />"
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

}
