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

#include "game.h"
#include "board.h"
#include "historymodel.h"

#define assert(x) Q_ASSERT(x)

Game::Game(QObject *parent) :
    QObject(parent),
    m_difficulty(1)
{
    QObject::connect(&m_computerPlayer, SIGNAL(finished()),
                     this, SLOT(onComputerPlayerFinished()));
}

void Game::init()
{
    m_board.init();
    m_history.clear();
}

bool Game::tryMove(int from, int to)
{
    qDebug() << "Move from " << from << " to " << to;

    if (to >= 0) {
        if (m_board[to] != 0) {
            qDebug() << "Target field occupied";
            return false;
        }
    }

    int player = m_board.player();
    m_board.move(from, to);

    m_history.addMove(HistoryElement(player, from, to, m_board.toLongLong(), m_board.phase()));

    m_board.debugPrint();

    return true;
}

void Game::computeMove()
{
    static int nodes[] = { 0, 500, 15000, 100000, 300000, 1000000 };

    m_computerPlayer.setNodes(nodes[m_difficulty]);
    m_computerPlayer.setBoard(m_board);
    m_computerPlayer.start();
}

bool Game::isPartOfMill(int target) const
{
    return m_board.isPartOfMill(target);
}

bool Game::playerHasLost() const
{
    return m_board.playerHasLost();
}

int Game::difficulty() const
{
    return m_difficulty;
}

void Game::setDifficulty(int difficulty)
{
    if (difficulty < 1 || difficulty > 5)
        return;
    m_difficulty = difficulty;
}

void Game::onComputerPlayerFinished()
{
    const Move move = m_computerPlayer.move();

    moveFound(move.m_from, move.m_to, move.m_take);
}

