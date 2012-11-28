import QtQuick 1.0
import Muehle 1.0

import "board.js" as JS

Item {
    id: board

    property string error
    property string prompt
    property bool playerBlack
    property int ply
    property bool computerBlack: parent.settings.computerBlack
    property bool computerWhite: parent.settings.computerWhite
    property alias game: game
    property bool busy: false

    signal gameFinished()

    /*
    Rectangle {
        id: blackTray
        color: "#8080807f"
        width: 480
        height: 80
        y: 0
    }
    */
    Item {
        id: boardImage
        anchors.verticalCenter: parent.verticalCenter
        width: 768
        height: 768
        //width: 480
        //height: 480
    }
    /*
    Image {
        id: boardImage
        anchors.verticalCenter: parent.verticalCenter
        width: 480
        height: 480
        source: "../images/board.png"
    }
    */
    /*
    Rectangle {
        id: whiteTray
        color: "#2020207f"
        width: 480
        height: 80
        y: board.height - height
    }
    */

    SequentialAnimation {
        id: moveAndTake
        ParallelAnimation {
            id: move
            NumberAnimation {
                id: movex
                property: "x"
                duration: 400
                easing.type: Easing.InOutExpo
            }
            NumberAnimation {
                id: movey
                property: "y"
                duration: 400
                easing.type: Easing.InOutExpo
            }
        }
        ParallelAnimation {
            id: take
            NumberAnimation {
                id: takex
                property: "x"
                duration: 500
                easing.type: Easing.InOutExpo
            }
            NumberAnimation {
                id: takey
                property: "y"
                duration: 500
                easing.type: Easing.InOutExpo
            }
        }
        onRunningChanged: {
            if (!running) {
                if (takey.target != null && takey.target.__remove)
                    takey.target.destroy()
                if (JS.mode == JS.FINISHED)
                    gameFinished()
            }
        }
    }

    function movePiecesTo(piece, x, y, piece2, x2, y2) {
        piece.snap.complete()
        moveAndTake.complete()
        movex.target = piece
        movey.target = piece
        movex.to = x
        movey.to = y
        if (!(piece2===undefined)) {
            piece2.snap.complete()
            takex.target = piece2
            takey.target = piece2
            takex.to = x2
            takey.to = y2
        } else {
            takex.target = null
            takey.target = null
        }
        moveAndTake.running = true
    }

    function movePieceToPosition(piece, target) {
        var x = getX(target)
        var y = getY(target)

        movePiecesTo(piece, x - piece.width/2, y - piece.height/2)
        piece.lastPosition = target
    }

    function moveAndTakePieces(piece, target, piece2) {
        var x = getX(target)
        var y = getY(target)
        var x2 = getX(-1)
        var y2 = getY(-1)

        // Mark for removal
        piece2.__remove = true

        movePiecesTo(piece, x - piece.width/2, y - piece.height/2, piece2, x2 - piece2.width/2, y2 - piece2.height/2)
        piece.lastPosition = target
    }

    Timer {
        id: startTimer
        interval: 500
        repeat: false
        onTriggered: startNewGame()
    }

    Game {
        id: game
        difficulty: settings.difficulty
        onMoveFound: {
            if ((playerBlack && computerBlack) || (!playerBlack && computerWhite)) {
                maybeExecuteMove(from, to, take)
            }
            board.busy = false
            // Enable the board for player input
            if ((playerBlack && !computerBlack) || (!playerBlack && !computerWhite))
                if (JS.mode != JS.FINISHED)
                    board.enabled = true
        }
    }

    function maybeExecuteMove(from, to, take) {
        if (to < 0) {
            console.debug("Invalid move.")
            return
        }
        var piece
        if (from >= 0) {
            // Move a piece on the board
            piece = JS.gameBoard[from]
        } else {
            // Place a piece from the tray
            if (playerBlack) {
                for (var i = 0; i < 9 && JS.blackTray[i] != null; i++)
                    piece = JS.blackTray[i]
            } else {
                for (var i = 0; i < 9 && JS.whiteTray[i] != null; i++)
                    piece = JS.whiteTray[i]
            }
        }
        movePiece(piece, from, to)

        if (take == -1) {
            movePieceToPosition(piece, to)
            return
        }

        if (JS.mode != JS.REMOVE) {
            console.debug("Invalid try to remove." + take + " " + JS.mode)
            return
        }

        var piece2 = JS.gameBoard[take]
        // Start the animation before doing the actual move to get the correct out of board position
        moveAndTakePieces(piece, to, piece2)
        movePiece(piece2, take, -1)
    }

    Component.onCompleted: startTimer.start()

    function nearestPosition(_x, _y) {
        if (_y < boardImage.y || _y > (boardImage.y + boardImage.height))
            return -1
        var positions = JS.positions
        var distance2_min = 768 * 768
        var nearest = 0
        var i = 0
        for (; i < positions.length; i++) {
            var distance2 = Math.pow(_x - positions[i][0], 2) + Math.pow(_y - boardImage.y - positions[i][1], 2)
            if (distance2 < distance2_min) {
                distance2_min = distance2
                nearest = i
            }
        }
        return nearest
    }

    function getX(index) {
        if (index == -1)
            return 240
        return JS.positions[index][0] + Math.round(2 * Math.random() - 1)
    }

    function getY(index) {
        if (index == -1) {
            // Move out of view
            if (playerBlack)
                return -120
            else
                return board.height + 220
        }
        return JS.positions[index][1] + boardImage.y + Math.round(2 * Math.random() - 1)
    }

    function startNewGame() {
        JS.startNewGame()
        playerBlack = (JS.player == JS.black)
        ply = JS.counter
        maybeComputeMove()
    }

    function movePiece(piece, lastPosition, target) {
        error = ""
        var valid = JS.movePiece(piece, lastPosition, target)
        if (!valid)
            error = JS.error

        playerBlack = (JS.player == JS.BLACK)
        ply = JS.counter

        updatePrompt()

        if (JS.mode == JS.FINISHED) {
            // If the last player was a human, finish immediately
            if ((!playerBlack && !computerBlack) || (playerBlack && !computerWhite))
                gameFinished()
        } else
            maybeComputeMove()

        return valid
    }

    function updatePrompt() {
        prompt = JS.getPrompt()
        console.log(prompt)
    }

    function maybeComputeMove() {
        if (JS.mode == JS.PLACE || JS.mode == JS.MOVE) {
            if ((playerBlack && computerBlack) || (!playerBlack && computerWhite)) {
                board.busy = true
                board.enabled = false
                game.computeMove()
            } else {
                // This takes care of enabling the board when called from startNewGame
                board.enabled = true
            }
        }
    }

    function clear() {
        for (var i = 0; i < 3 * 8; i++) {
            var piece = JS.gameBoard[i];
            if (piece != null)
                piece.destroy()
        }
    }
}
