#include <QDebug>

#include "computerplayer.h"

#define assert(x) Q_ASSERT(x)

ComputerPlayer::ComputerPlayer(QObject *parent) :
    QThread(parent),
    m_nodes(500)
{
}

const Move& ComputerPlayer::move() const {
    return m_move;
}

void ComputerPlayer::setNodes(int nodes) {
    m_nodes = nodes;
}

void ComputerPlayer::setBoard(Board& board) {
    m_board = board;
}

void ComputerPlayer::run()
{
    double eval;
    m_move.m_from = -1;
    m_move.m_to = -1;
    m_move.m_take = -1;

    m_nodesVisited = 0;
    for (int depth = 2; m_nodesVisited < m_nodes; depth++) {
        eval = alphaBetaRecurse(m_board, -99999.9, 99999.9, depth, &m_move);
        if (abs((int)eval) > 100)
            break;
        //qDebug() << depth;
    }
    //qDebug() << "eval: " << eval << " move: " << m_move.m_from << "-" << m_move.m_to << "x" << m_move.m_take;
}

double ComputerPlayer::alphaBetaRecurse(const Board& board, double alpha, double beta, int levelsToGo, Move* move)
{
    m_nodesVisited++;

    if (levelsToGo == 0)
        return board.evaluate();

    // Check for lost game
    if (board.playerHasLost()) {
        if (board.player() == WHITE)
            return -9999.9 - levelsToGo;
        else
            return +9999.9 + levelsToGo;
    }

#if 0
    if (stop)
      return 0.0;
#endif

    // internal node: expand node

    Board tmp;

    Move moves[200];
    int numMoves = board.generateMoves(moves, 200);
    if (numMoves == 0) {
        double e;
        if (board.player() == WHITE)
            e = -9999.9 - levelsToGo;
        else
            e = +9999.9 + levelsToGo;
        assert(!move);
        return e;
    }
    assert(numMoves < 200);

    //qDebug() << "generated" << numMoves << "moves";

    double bestEval;
    int    bestIndex = 0;  // = 0 just in case that there is no valid move
    bool   moveFound = false;

    if (board.player() == WHITE) {
        bestEval = -99999.9;

        for (int i = 0; i < numMoves; i++) {
            if (bestEval > beta)
                return bestEval;

            tmp = board;
            tmp.doMove(moves[i]);

            double eval = alphaBetaRecurse(tmp, alpha, beta, levelsToGo - 1, NULL);
            if (eval > alpha)
                alpha = eval;

            if (eval > bestEval || (eval == bestEval && (rand()%2))) {
                bestIndex = i;
                bestEval = eval;
                moveFound = true;
            }
        }
    } else { // board.player() == BLACK
        bestEval = 99999.9;

        for (int i = 0; i < numMoves; i++) {
            if (bestEval < alpha)
                return bestEval;

            tmp = board;
            tmp.doMove(moves[i]);

            double eval = alphaBetaRecurse(tmp, alpha, beta, levelsToGo - 1, NULL);
            if (eval < beta)
                beta = eval;

            if (eval < bestEval || (eval == bestEval && (rand()%2))) {
                bestIndex = i;
                bestEval = eval;
                moveFound = true;
            }
        }
    }

    if (!moveFound)
        qCritical() << "NO MOVE FOUND!";
    assert(moveFound);

    if (move)
        *move = moves[bestIndex];

    return bestEval;
}
