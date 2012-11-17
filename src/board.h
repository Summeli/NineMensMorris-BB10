#ifndef BOARD_H
#define BOARD_H

#include <QtCore>

#define BLACK -1
#define EMPTY 0
#define WHITE 1

class Move
{
public:
    signed char m_from;
    signed char m_to;
    signed char m_take;

    Move() {}
    Move(signed char from, signed char to, signed char take)
        : m_from(from), m_to(to), m_take(take) {}
};

class Board
{
public:
    Board();
    Board(qlonglong init);
    void init();

    void move(int from, int to);
    void doMove(Move& move);

    int generateMoves(Move* moves, int maximumMoves) const;
    double evaluate() const;
    int pieceFreedom(int pos) const;
    bool isPartOfMill(int target) const;
    bool wouldCloseMill(int from, int target) const;
    void debugPrint() const;
    bool playerHasLost() const;
    bool opponentHasLost() const;
    qlonglong toLongLong() const;
    int phase() const;

    inline signed char operator[](int index) const {
        return m_board[index];
    }
    inline signed char player() const {
        return m_player;
    }

private:
    signed char m_board[3 * 8];
    int m_player;
    int m_whitePieces;
    int m_blackPieces;
    int m_ply;

    int generateTakes(Move* moves) const;
};

#endif // BOARD_H
