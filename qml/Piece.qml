import QtQuick 1.0
//import QtQuick 1.1
//import QtMobility.feedback 1.1

Item {
    id: piece
    property int type: 0
    property int lastPosition: -1
    property int lastX
    property int lastY
    property bool __remove: false

    property alias snap: snap

    width: 60
    height: 60

    Image {
        id: shadow
        source: "../images/piece-shadow.png"
        width: 60
        height: 60
        y: 3

        states: [
            State {
                name: "raised"
                when: area.pressed
                PropertyChanges {
                    target: shadow
                    x: -30
                    y: -23
                    width: 120
                    height: 120
                }
                PropertyChanges {
                    target: piece
                    z: 1
                }
            }
        ]

        transitions: Transition {
            NumberAnimation {
                properties: "x,y,z,width,height"
                duration: 50
                easing.type: Easing.InOutQuad
            }
        }
    }

    Image {
        id: image
        source: piece.type == 0 ? "../images/piece-black.png" :"../images/piece-white.png"
        x: 5
        y: 5
        width: 50
        height: 50

        states: [
            State {
                name: "raised"
                when: area.pressed
                PropertyChanges {
                    target: image
                    x: -20
                    y: -20
                    width: 100
                    height: 100
                }
            }
        ]

        transitions: Transition {
            NumberAnimation {
                properties: "x,y,width,height"
                duration: 50
                easing.type: Easing.InOutQuad
            }
        }
    }

    ParallelAnimation {
        id: snap
        NumberAnimation {
            id: snapx
            target: piece
            property: "x"
            duration: 300
            easing.type: Easing.OutExpo
        }
        NumberAnimation {
            id: snapy
            target: piece
            property: "y"
            duration: 300
            easing.type: Easing.OutExpo
            onRunningChanged: {
                if (!running && __remove)
                    piece.destroy()
            }
        }
    }

    MouseArea {
        id: area

        anchors.fill: parent
        drag.target: piece

        onPressed: {
            piece.lastX = piece.x
            piece.lastY = piece.y
            //piecePickEffect.play()
        }

        onReleased: {
            var target = board.nearestPosition(piece.x + piece.width/2, piece.y + piece.height/2)
            var targetX = board.getX(target)
            var targetY = board.getY(target)

            snap.complete()
            if (target != lastPosition &&
                board.movePiece(piece, lastPosition, target)) {
                // Possibly mark for removal
                if (target == -1)
                    __remove = true
                snapx.to = targetX - piece.width/2
                snapy.to = targetY - piece.height/2
                piece.lastPosition = target
                //pieceDropEffect.play()
            } else {
                snapx.to = piece.lastX
                snapy.to = piece.lastY
                //moveFailedEffect.play()
            }
            snap.running = true
            mouse.accepted = true
        }

        /*
        ThemeEffect {
                id: piecePickEffect
                effect: ThemeEffect.ItemPick
        }

        ThemeEffect {
            id: pieceDropEffect
            effect: ThemeEffect.ItemDrop
        }

        ThemeEffect {
            id: moveFailedEffect
            effect: ThemeEffect.NegativeTacticon
        }
        */
    }

    function moveTo(x, y) {
        snap.complete()
        snapx.to = x
        snapy.to = y
        snap.running = true
    }

    function moveToPosition(target) {
        var x = board.getX(target)
        var y = board.getY(target)

        console.debug("Piece::moveToPosition(" + target + ") - moving to " + x + "," + y)

        // Possibly mark for removal
        if (target == -1)
            __remove = true

        snap.complete()

        snapx.to = x - piece.width/2
        snapy.to = y - piece.height/2
        snap.running = true
        lastPosition = target
    }
}
