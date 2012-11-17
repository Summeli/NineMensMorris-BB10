/*
 *   QMühLe
 *   Written by Philipp Zabel <philipp.zabel@gmail.com>
 *
 *   Alpha beta pruning search algorithm based on Gnome Nine Mens Morris,
 *   written by Dirk Farin <dirk.farin@gmx.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <QDebug>

#include "board.h"

#define assert(x) Q_ASSERT(x)

/*
  0 ------------ 1 ------------ 2
  |              |              |
  |    3 ------- 4 ------- 5    |
  |    |         |         |    |
  |    |    6 -- 7 -- 8    |    |
  |    |    |         |    |    |
  9 --10 --11        12 --13 --14
  |    |    |         |    |    |
  |    |   15 --16 --17    |    |
  |    |         |         |    |
  |   18 -------19 ------ 20    |
  |              |              |
 21 ------------22 ------------23
*/
static int mills[16][3] = {
    // horizontal
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23},
    // vertical
    {0, 9, 21}, {3, 10, 18}, {6, 11, 15}, {1, 4, 7}, {16, 19, 22}, {8, 12, 17}, {5, 13, 20}, {2, 14, 23}
};

static int neighbors[8 * 3][4] = {
        {1, 9, -1, -1}, {0, 2, 4, -1}, {1, 14, -1, -1},
        {4, 10, -1, -1}, {1, 3, 5, 7}, {4, 13, -1, -1},
        {7, 11, -1, -1}, {4, 6, 8, -1}, {7, 12, -1, -1},
        {0, 10, 21, -1}, {3, 9, 11, 18}, {6, 10, 15, -1}, {8, 13, 17, -1}, {5, 12, 14, 20}, {2, 13, 23, -1},
        {11, 16, -1, -1}, {15, 17, 19, -1}, {12, 16, -1, -1},
        {10, 19, -1, -1}, {16, 18, 20, 22}, {13, 19, -1, -1},
        {9, 22, -1, -1}, {19, 21, 23, -1}, {14, 22, -1, -1}
};

Board::Board()
{
}

Board::Board(qlonglong init)
{
    // Setup board
    m_whitePieces = 0;
    m_blackPieces = 0;
    for (int i = 0; i < 3*8; i++) {
        m_board[i] = (init & 0x3) - 1;
        if (m_board[i] == WHITE)
            m_whitePieces++;
        else if (m_board[i] == BLACK)
            m_blackPieces++;
        init >>= 2;
    }

    m_player = (init & 0x3) - 1;
    init >>= 2;

    m_ply = init;
}

qlonglong Board::toLongLong() const
{
    qlonglong result = 0;
    for (int i = 8 * 3 - 1; i >= 0; i--) {
        result <<= 2;
        result |= m_board[i] + 1;
    }

    result |= (qlonglong)(m_player + 1) << 48;
    result |= (qlonglong)m_ply << 50;
    return result;
}

int Board::phase() const
{
    if (m_ply <= 18)
        return 0; // Placing
    else if (m_whitePieces > 3 && m_blackPieces > 3)
        return 1; // Moving
    else
        return 2; // Jumping
}

void Board::init()
{
    // Clear board
    for (int i = 0; i < 3*8; i++)
        m_board[i] = EMPTY;

    // No pieces on the board
    m_whitePieces = 0;
    m_blackPieces = 0;

    // White player begins
    m_player = WHITE;
    m_ply = 0;
}

void Board::move(int from, int to)
{
    if (to >= 0) {
        assert(m_board[to] == EMPTY);

        if (from >= 0)
            m_board[to] = m_board[from];
        else {
            m_board[to] = m_player;
            if (m_player == WHITE)
                m_whitePieces++;
            else
                m_blackPieces++;
        }
    }
    if (from >= 0) {
        if (to == -1) {
            if (m_board[from] == WHITE)
                m_whitePieces--;
            if (m_board[from] == BLACK)
                m_blackPieces--;
        }
        m_board[from] = EMPTY;
    }

    // Allow player to remove opponent's piece when a mill is closed
    if (!isPartOfMill(to) || to == -1) {
        m_player = -m_player;
        m_ply++;
    }

    // Validate the board: count pieces
    int whiteCount = 0;
    int blackCount = 0;
    for (int i = 0; i < 3 * 8; i++) {
        if (m_board[i] == WHITE)
            whiteCount++;
        else if (m_board[i] == BLACK)
            blackCount++;
    }
    if (whiteCount != m_whitePieces)
        qCritical() << "White piece count does not match number of pieces on board! (" << from << "-" << "to" << ")";
    if (blackCount != m_blackPieces)
        qCritical() << "Black piece count does not match number of pieces on board! (" << from << "-" << "to" << ")";
}

void Board::doMove(Move& m)
{
    move(m.m_from, m.m_to);
    if (m.m_take >= 0)
        move(m.m_take, -1);
}

bool Board::playerHasLost() const
{
    if (m_ply < 2 * 9)
        return false;

    if ((m_player == WHITE ? m_whitePieces : m_blackPieces) < 3)
        return true;

    int freedom = 0;
    for (int i = 0 ; i < 3 * 8; i++) {
        if (m_board[i] == m_player)
            freedom += pieceFreedom(i);
    }
    if (freedom == 0 && ((m_player == WHITE ? m_whitePieces : m_blackPieces) > 3))
        return true;

    return false;
}

bool Board::opponentHasLost() const
{
    if (m_ply < 2 * 9)
        return false;

    return (m_player == WHITE ? m_blackPieces : m_whitePieces) < 3;
}

int Board::generateMoves(Move* moves, int maximumMoves) const
{
    int number = 0;

    if (m_ply < 2 * 9) {
        // Placement phase
        //qDebug() << "placement phase";

        for (int i = 0; i < 3 * 8; i++)
            if (m_board[i] == EMPTY) {
                assert(number < maximumMoves);
                moves[number].m_from = -1;
                moves[number].m_to = i;
                if (wouldCloseMill(-1, i)) {
                    assert(number < maximumMoves - 9);
                    number += generateTakes(&moves[number]);
                } else {
                    moves[number].m_take = -1;
                    number++;
                }
            }
    } else {
        // Movement phase
        //qDebug() << "movement phase, no jumps";

        bool mayJump = false;

        if (m_player == WHITE && m_whitePieces == 3)
            mayJump = true;
        if (m_player == BLACK && m_blackPieces == 3)
            mayJump = true;
        if (m_ply < 2 * 9)
            mayJump = false;

        if (!mayJump) {
            // Only movement to neighbor fields
            for (int i = 0; i < 3 * 8; i++)
                if (m_board[i] == m_player) {
                    assert(number < maximumMoves);

                    for (int j = 0; j < 4 && neighbors[i][j] >= 0; j++)
                        if (m_board[neighbors[i][j]] == EMPTY) {
                            int to = neighbors[i][j];

                            moves[number].m_from = i;
                            moves[number].m_to = to;

                            if (wouldCloseMill(i, to)) {
                                assert(number < maximumMoves - 9);
                                number += generateTakes(&moves[number]);
                            } else {
                                moves[number].m_take = -1;
                                number++;
                            }
                        }
                }
        } else {
            // Jumping is allowed
            //qDebug() << "placement phase, jumps allowed";

            for (int i = 0; i < 3 * 8; i++)
                if (m_board[i] == m_player) {
                    assert(number < maximumMoves);

                    for (int j = 0; j < 3 * 8; j++)
                        if (m_board[j] == EMPTY) {
                            moves[number].m_from = i;
                            moves[number].m_to = j;

                            if (wouldCloseMill(i, j)) {
                                assert(number < maximumMoves - 9);
                                number += generateTakes(&moves[number]);
                            } else {
                                moves[number].m_take = -1;
                                number++;
                            }
                        }
                }
        }
    }

    return number;
}

int Board::generateTakes(Move* moves) const
{
    int number = 0;

    // Only allow taking opponent pieces that are not in mills
    for (int i = 0; i < 3 * 8; i++)
        if (m_board[i] == -m_player && !isPartOfMill(i)) {
            moves[number] = moves[0];
            moves[number].m_take = i;
            number++;
        }

    if (number == 0) {
        // But if all opponent pieces are in mills, allow taking any
        for (int i = 0; i < 3 * 8; i++)
            if (m_board[i] == -m_player) {
                moves[number] = moves[0];
                moves[number].m_take = i;
                number++;
            }
    }

    assert(number > 0);

    return number;
}

double Board::evaluate() const
{
    int material = m_whitePieces - m_blackPieces;

    double freedom = 0;
    bool whiteNeedsFreedom = m_ply < 2 * 9 || m_whitePieces > 3;
    bool blackNeedsFreedom = m_ply < 2 * 9 || m_blackPieces > 3;
    for (int i = 0 ; i < 3 * 8; i++) {
        if (m_board[i] == WHITE && whiteNeedsFreedom)
            freedom += pieceFreedom(i);
        if (m_board[i] == BLACK && blackNeedsFreedom)
            freedom -= pieceFreedom(i);
    }

    double centers = 0;
    centers += m_board[4] + m_board[10] + m_board[13] + m_board[19];

    double closedMills = 0;
    for (int i = 0; i < 16; i++) {
        int type = m_board[mills[i][0]];
        if (type == m_board[mills[i][1]] &&
            type == m_board[mills[i][2]])
            closedMills += type;
    }

    return material + 0.3 * freedom + 0.2 * centers + 0.4 * closedMills;
}

int Board::pieceFreedom(int pos) const
{
    int freedom = 0;

    for (int i = 0; i < 4 && neighbors[pos][i] >= 0; i++)
        if (m_board[neighbors[pos][i]] == EMPTY)
            freedom++;

    return freedom;
}

static char piece_displ[3] = { '#','.','O' };
static char* pc = &piece_displ[1];
void Board::debugPrint() const {
    #define p(n) (pc[m_board[n]])
    qDebug() << p(0) << "------------" << p(1) << "------------" << p(2);
    qDebug() << "|   " << p(3) << "-------" << p(4) << "-------" << p(5) << "   |  ply:" << m_ply;
    qDebug() << "|    |   " << p(6) << "--" << p(7) << "--" << p(8) << "   |    |  white (O):" << m_whitePieces;
    qDebug() << p(9) << "--" << p(10) << "--" << p(11) << "       " << p(12) << "--" << p(13) << "--" << p(14) << " black (#):" << m_blackPieces;
    qDebug() << "|    |   " << p(15) << "--" << p(16) << "--" << p(17) << "   |    |  next:" << ((m_player == WHITE) ? "white" : "black");
    qDebug() << "|   " << p(18) << "-------" << p(19) << "-------" << p(20) << "   |";
    qDebug() << p(12) << "------------" << p(22) << "------------" << p(23);
    #undef p
}

bool Board::isPartOfMill(int target) const
{
    if (m_board[target] == EMPTY)
        return false;

    int type = m_board[target];
    for (int i = 0; i < 16; i++) {
        int pos1 = -1;
        int pos2 = -1;
        if (mills[i][0] == target) {
            pos1 = mills[i][1];
            pos2 = mills[i][2];
        } else if (mills[i][1] == target) {
            pos1 = mills[i][0];
            pos2 = mills[i][2];
        } else if (mills[i][2] == target) {
            pos1 = mills[i][0];
            pos2 = mills[i][1];
        } else {
            continue;
        }
        if (m_board[pos1] == type && m_board[pos2] == type) {
            //qDebug() << "isPartOfMill:" << target << "," << pos1 << "," << pos2;
            return true;
        }
    }
    return false;
}

bool Board::wouldCloseMill(int from, int target) const
{
    if (m_board[target] != EMPTY)
        return false;

    int type;
    if (from == -1)
        type = m_player;
    else
        type = m_board[from];
    for (int i = 0; i < 16; i++) {
        int pos1 = -1;
        int pos2 = -1;
        if (mills[i][0] == target) {
            pos1 = mills[i][1];
            pos2 = mills[i][2];
        } else if (mills[i][1] == target) {
            pos1 = mills[i][0];
            pos2 = mills[i][2];
        } else if (mills[i][2] == target) {
            pos1 = mills[i][0];
            pos2 = mills[i][1];
        } else {
            continue;
        }
        if (pos1 != from && pos2 != from && m_board[pos1] == type && m_board[pos2] == type) {
            //qDebug() << "wouldCloseMill:" << from << " --> " << target << "," << pos1 << "," << pos2;
            return true;
        }
    }
    return false;
}

/*
bool Board::WouldCloseMill(int s,int n) const
{
  assert(board[n]==EMPTY);

  if (board[milltab[n][0][0]] == next &&
      board[milltab[n][0][1]] == next &&
      milltab[n][0][0] != s &&
      milltab[n][0][1] != s) return true;

  if (board[milltab[n][1][0]] == next &&
      board[milltab[n][1][1]] == next &&
      milltab[n][1][0] != s &&
      milltab[n][1][1] != s) return true;

  return false;
}
*/
