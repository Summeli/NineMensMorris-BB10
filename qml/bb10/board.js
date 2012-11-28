/*
var blockSize = 40
var maxColumn = 10
var maxRow = 15
var maxIndex = maxColumn * maxRow
*/
var gameBoard = new Array(3*8)
var whiteTray = new Array(9)
var blackTray = new Array(9)
var component

var pieces = new Array(2)

var player
var BLACK = 0
var WHITE = 1

var mode
var PLACE = 0
var MOVE = 1
var REMOVE = 2
var JUMP = 3
var FINISHED = 4

var counter = 0
var error
/*
var positions = [
    [62, 62], [242, 62], [420, 62],
    [118, 118], [242, 118],[362, 118],
    [176,176], [242, 176], [306,176],
    [62, 240], [118, 240], [176, 240], [306, 240], [362, 240], [420, 240],
    [176, 304], [242, 304], [306,304],
    [118, 362], [242, 362],[362, 362],
    [62, 420], [242, 420], [420, 420],
]*/

var positions = [
                 [100, 90], [385, 90], [670, 90],
                 [190, 185], [385, 185],[580, 185],
                 [285,276], [385, 276], [490,276],
                 [100, 380], [190, 380], [285, 380], [490, 380], [580, 380], [670, 380],
                 [285, 480], [385, 480], [490,480],
                 [190, 575], [385, 575],[580, 575],
                 [100, 670], [385, 670], [670, 670],
             ]
/*
  0--------1--------2
  |        |        |
  |  3-----4-----5  |
  |  |     |     |  |
  |  |  6--7--8  |  |
  |  |  |     |  |  |
  9-10-11    12-13-14
  |  |  |     |  |  |
  |  | 15-16-17  |  |
  |  |     |     |  |
  | 18----19----20  |
  |        |        |
 21-------22-------23
*/

var neighbors = [
        [1, 9], [0, 2, 4], [1, 14],
        [4, 10], [1, 3, 5, 7], [4, 13],
        [7, 11], [4, 6, 8], [7, 12],
        [0, 10, 21], [3, 9, 11, 18], [6, 10, 15], [8, 13, 17], [5, 12, 14, 20], [2, 13, 23],
        [11, 16], [15, 17, 19], [12, 16],
        [10, 19], [16, 18, 20, 22], [13, 19],
        [9, 22], [19, 21, 23], [14, 22]
]

function startNewGame()
{
    // Clear board
    for (var i = 0; i < 3 * 8; i++) {
        if (gameBoard[i] != null) {
            gameBoard[i].destroy()
            gameBoard[i] = null
        }
    }

    // Clear trays
    for (var i = 0; i < 9; i++) {
        if (whiteTray[i] != null) {
            whiteTray[i].destroy()
            whiteTray[i] = null
        }
        if (blackTray[i] != null) {
            blackTray[i].destroy()
            blackTray[i] = null
        }
    }

    // Create pieces
    for (var i = 0; i < 9; i++) {
        var piece = createPiece(0, -60, BLACK)
        blackTray[i] = piece
        piece.moveTo(86 * i - 2, 100)
    }
    for (var i = 0; i < 9; i++) {
        var piece = createPiece(768 - piece.width, board.height, WHITE)
        whiteTray[i] = piece
        piece.moveTo(86 * i - 2, board.height - 100 - piece.height)
    }

    // No pieces on the board
    pieces[BLACK] = 0
    pieces[WHITE] = 0

    // White player begins
    player = WHITE
    mode = PLACE
    counter = 0

    game.init()
}

function createPiece(x, y, type) {
    if (component == null)
        component = Qt.createComponent("Piece.qml")

    // Piece.qml is a local file, so we don't need to wait for statusChanged()
    if (component.status == Component.Ready) {
        var dynamicObject = component.createObject(board)
        if (dynamicObject == null) {
            console.log("error creating piece")
            console.log(component.errorString())
            return null
        }
        dynamicObject.x = x
        dynamicObject.y = y
        dynamicObject.type = type
//      board[index(column, row)] = dynamicObject
    } else {
        console.log("error loading piece component")
        console.log(component.errorString())
        return null
    }
    return dynamicObject
}

function movePiece(piece, lastPosition, target)
{
    error = null

    // Ignore non-moves
    if (lastPosition == target)
        return true

    // Trying to remove a piece?
    if (target == -1) {
        // Only opponent's pieces, if allowed by game state
        if (mode != REMOVE) {
            error = "You can't remove pieces"
            return false
        }

        if (piece.type == player) {
            error = "That is your own piece"
            return false
        }

        // Only remove pieces that are not part of a mill
        if (isPartOfMill(lastPosition)) {
            // Can we find any enemy piece that is not part of a mill?
            for (var i = 0; i < 3 * 8; i++) {
                if (gameBoard[i] != null && gameBoard[i].type != player && !isPartOfMill(i)) {
                    error = "This piece is part of a mill"
                    return false
                }
            }
            // If all enemy pieces are in mills, allow removal
        }

        // Remove the piece
        game.tryMove(lastPosition, target)
        if (lastPosition != -1)
            gameBoard[lastPosition] = null
        player = 1 - player
        pieces[player] -= 1
        counter += 1

        if (game.playerHasLost()) {
            mode = FINISHED
        } else if (counter < 2 * 9) {
            mode = PLACE
        } else {
            mode = MOVE
        }
        return true
    }

    // Trying not to remove a piece?
    if (mode == REMOVE && target >= 0) {
        error = "You should remove a piece"
        return false
    }

    // Otherwise, don't allow handling the opponent's pieces
    if (piece.type != player) {
        error = "That piece is not yours"
        return false
    }

    // No board movement during placement phase
    if (mode == PLACE && lastPosition != -1) {
        error = "You should place a piece"
        return false
    }

    // No more than two pieces on one field
    if (gameBoard[target] != null) {
        error = "This field is occupied"
        return false
    }

    // No jumps with more than three pieces!
    if (mode == MOVE && pieces[player] > 3 && !areNeighbors(lastPosition, target)) {
        error = "No jumping allowed"
        return false
    }

    // Move the piece
    game.tryMove(lastPosition, target)
    if (lastPosition != -1)
        gameBoard[lastPosition] = null
    else {
        removeFromTray(piece)
        pieces[player] += 1
    }
    gameBoard[target] = piece

    // Allow player to remove opponent's piece when a mill is closed
    if (isPartOfMill(target)) {
        mode = REMOVE
        return true
    }

    // Otherwise, just change the active player
    player = 1 - player
    counter += 1
    if (game.playerHasLost()) {
        mode = FINISHED
    } else if (counter < 2 * 9) {
        mode = PLACE
    } else {
        mode = MOVE
    }
    return true
}

function removeFromTray(piece) {
    var tray;
    if (piece.type == WHITE)
        tray = whiteTray
    else
        tray = blackTray
    for (var i = 0; i < 9 && tray[i] != null; i++) {
        if (tray[i] == piece) {
            // Remove the piece
            tray[i] = null
            var piecesLeft = 8
            // Shift remaining pieces
            for (var j = i; j < 8; j++) {
                tray[j] = tray[j + 1]
                if (tray[j] == null && j < piecesLeft)
                    piecesLeft = j
            }
            tray[8] = null
            // Reposition tray pieces
            for (var j = 0; j < 9 && tray[j] != null; j++) {
                tray[j].moveTo(86 * j - 2 + (9 - piecesLeft) * 43, tray[j].y)
            }
            return;
        }
    }
}

function areNeighbors(pos1, pos2)
{
    if (pos1 == -1 || pos2 == -1)
        return false
    for (var i = 0; i < neighbors[pos1].length; i++) {
        if (neighbors[pos1][i] == pos2)
            return true
    }
    return false
}

function isPartOfMill(target)
{
    return game.isPartOfMill(target)
}

function getPrompt() {
    if (mode == PLACE)
        return (player == BLACK) ? "Place a black piece"
                                 : "Place a white piece"
    if (mode == MOVE)
        return (player == BLACK) ? "Move a black piece"
                                 : "Move a white piece"
    if (JS.mode == REMOVE)
        return (player == BLACK) ? "Take a white piece"
                                 : "Take a black piece"

    if (mode == FINISHED)
        return ""
}
