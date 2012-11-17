#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include <QThread>

#include "board.h"

class ComputerPlayer : public QThread
{
    Q_OBJECT
public:
    explicit ComputerPlayer(QObject *parent = 0);
    void setNodes(int nodes);
    void setBoard(Board& board);
    const Move& move() const;
    void run();

signals:

public slots:

private:
    int m_nodes;
    int m_nodesVisited;
    Board m_board;
    Move m_move;

    double alphaBetaRecurse(const Board& board, double alpha, double beta, int levelsToGo, Move* move);
};

#endif // COMPUTERPLAYER_H
