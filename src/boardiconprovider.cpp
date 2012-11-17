#/*
 *   QMühLe
 *   Written by Philipp Zabel <philipp.zabel@gmail.com>
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

#include <QPainter>
#include <QDebug>

#include "boardiconprovider.h"
#include "board.h"

BoardIconProvider::BoardIconProvider()
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
      m_icon("/usr/share/themes/blanco/meegotouch/icons/icon-m-muehle.png")
{
    QPixmap pixmap();
}

QPixmap BoardIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = 64;
    int height = 64;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(m_icon);

    QPainter painter(&pixmap);

    QPen black(QBrush(Qt::black), 5.0);
    QPen white(QBrush(Qt::white), 5.0);

    Board board(id.toLongLong());
    int x, y;
    for (int i = 0; i < 3 * 8; i++) {
        if (board[i] == EMPTY)
            continue;
        x = i % 3;
        y = i / 3;
        if (y == 4)
            x += 4;
        if (y > 3)
            y -= 1;
        switch (abs(y - 3)) {
        case 3:
            x *= 3;
            break;
        case 2:
            x = x * 2 + 1;
            break;
        case 1:
            x += 2;
            break;
        }
        if (board[i] == BLACK)
            painter.setPen(black);
        else if (board[i] == WHITE)
            painter.setPen(white);
        painter.drawPoint(14 + 6 * x, 13 + 6 * y);
    }

    return pixmap;
}
