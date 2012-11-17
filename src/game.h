#ifndef GAME_H
#define GAME_H

#include <QDeclarativeItem>
#include <QDeclarativeView>

#include "board.h"
#include "historymodel.h"
#include "computerplayer.h"

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)
    Q_PROPERTY(QObject* history READ history NOTIFY historyChanged)
public:
    explicit Game(QObject *parent = 0);

    Q_INVOKABLE void init();
    Q_INVOKABLE bool tryMove(int from, int to);
    Q_INVOKABLE void computeMove();
    Q_INVOKABLE bool isPartOfMill(int target) const;
    Q_INVOKABLE bool playerHasLost() const;

    int difficulty() const;
    void setDifficulty(int difficulty);
    QObject *history() {
        return &m_history;
    }

signals:
    void moveFound(int from, int to, int take);
    void difficultyChanged();
    void historyChanged();

public slots:
    void onComputerPlayerFinished();

private:
    Board m_board;
    int m_difficulty;
    HistoryModel m_history;
    int m_nodesVisited;
    ComputerPlayer m_computerPlayer;

    double alphaBetaRecurse(const Board& b, double alpha, double beta, int levels_to_go, Move* move);
    double evaluateBoard(/*Board*/) const;
    int pieceFreedom(int pos) const;
};

#endif // GAME_H
