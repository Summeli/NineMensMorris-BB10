/*
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

#include "settings.h"

#include <QSettings>
#include <QDebug>

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_computerWhite(false),
    m_computerBlack(true),
    m_faceToFace(true),
    m_difficulty(1),
    m_fullScreen(true)
{
}

void Settings::load()
{
    setComputerWhite(m_settings.value("computerWhite", QVariant(false)).toBool());
    setComputerBlack(m_settings.value("computerBlack", QVariant(true)).toBool());
    setFaceToFace(m_settings.value("faceToFace", QVariant(true)).toBool());
    setDifficulty(m_settings.value("difficulty", QVariant(1)).toInt());
    setFullScreen(m_settings.value("fullScreen", QVariant(true)).toBool());
}

void Settings::save()
{
    m_settings.setValue("computerWhite", m_computerWhite);
    m_settings.setValue("computerBlack", m_computerBlack);
    m_settings.setValue("faceToFace", m_faceToFace);
    m_settings.setValue("difficulty", m_difficulty);
    m_settings.setValue("fullScreen", m_fullScreen);
}

void Settings::setComputerWhite(bool computer)
{
    if (m_computerWhite != computer) {
        m_computerWhite = computer;
        emit computerWhiteChanged();
    }
}

bool Settings::computerWhite() const
{
    return m_computerWhite;
}

void Settings::setComputerBlack(bool computer)
{
    if (m_computerBlack != computer) {
        m_computerBlack = computer;
        emit computerBlackChanged();
    }
}

bool Settings::computerBlack() const
{
    return m_computerBlack;
}

void Settings::setFaceToFace(bool faceToFace)
{
    if (m_faceToFace != faceToFace) {
        m_faceToFace = faceToFace;
        emit faceToFaceChanged();
    }
}

bool Settings::faceToFace() const
{
    return m_faceToFace;
}

void Settings::setDifficulty(int difficulty)
{
    if (m_difficulty != difficulty) {
        m_difficulty = difficulty;
        emit difficultyChanged();
    }
}

int Settings::difficulty() const
{
    return m_difficulty;
}

void Settings::setFullScreen(bool fullScreen)
{
    if (m_fullScreen != fullScreen) {
        m_fullScreen = fullScreen;
        emit fullScreenChanged();
    }
}

bool Settings::fullScreen() const
{
    return m_fullScreen;
}
