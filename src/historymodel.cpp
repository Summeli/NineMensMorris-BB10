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

#include <QStringBuilder>

#include "historymodel.h"
#include "board.h"

HistoryElement::HistoryElement(int player, int from, int to, qlonglong board, int phase)
    : m_player(player), m_from(from), m_to(to), m_board(board), m_phase(phase)
{
}

QString HistoryElement::title() const
{
    QString title;
    int ply = m_board >> 50;

    if (m_from == -1) {
        if (m_player == WHITE)
            //% "%1: White places"
            title = qtTrId("hist_white_place_%1").arg(ply);
        else
            //% "%1: Black places"
            title = qtTrId("hist_black_place_%1").arg(ply);
    } else {
        if (m_to == -1) {
            if (m_player == WHITE)
                //% "%1: White takes"
                title = qtTrId("hist_white_takes_%1").arg(ply);
            else
                //% "%1: Black takes"
                title = qtTrId("hist_black_takes_%1").arg(ply);
        } else {
            if (m_player == WHITE)
                //% "%1: White moves"
                title = qtTrId("hist_white_move_%1").arg(ply);
            else
                //% "%1: Black moves"
                title = qtTrId("hist_black_move_%1").arg(ply);
        }
    }

    return title;
}

QString HistoryElement::subtitle() const
{
    return QString("");
}

QString HistoryElement::sourceIcon() const
{
    QVariant tmp(m_board);
    return "image://boardicons/" % tmp.toString();
}

QString HistoryElement::section() const
{
    switch (m_phase) {
    case 0:
        //% "Placing"
        return qtTrId("hist_sect_place");
    case 1:
        //% "Moving"
        return qtTrId("hist_sect_move");
    case 2:
        //% "Jumping"
        return qtTrId("hist_sect_jump");
    }

    return QString("");
}

HistoryModel::HistoryModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[SubtitleRole] = "subtitle";
    roles[IconSourceRole] = "iconSource";
    roles[SectionRole] = "section";
    setRoleNames(roles);
}

void HistoryModel::addMove(const HistoryElement &move)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_moves << move;
    endInsertRows();
}

void HistoryModel::clear()
{
    m_moves.clear();
}

int HistoryModel::rowCount(const QModelIndex & parent) const {
    return m_moves.count();
}

QVariant HistoryModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_moves.count())
        return QVariant();

    const HistoryElement &move = m_moves[index.row()];
    if (role == TitleRole)
        return move.title();
    else if (role == SubtitleRole)
        return move.subtitle();
    else if (role == IconSourceRole)
        return move.sourceIcon();
    else if (role == SectionRole)
        return move.section();
    return QVariant();
}

int HistoryModel::count() const
{
    return m_moves.count();
}

QVariantMap HistoryModel::get(int index) const
{
    QVariantMap listElement;

    if (index < 0 || index > m_moves.count())
        return listElement;

    const HistoryElement &move = m_moves[index];
    listElement["section"] = move.section();

    return listElement;
}
