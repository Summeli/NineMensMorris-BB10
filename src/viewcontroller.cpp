/* AntSnes
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "viewcontroller.h"

#include <QtGui/QApplication>
#include <QtDeclarative>

ViewController::ViewController(QDeclarativeView* gameView, QDeclarativeView* aboutView)
{
	m_gameView = gameView;
	m_aboutView = aboutView;

	m_gameView->rootContext()->setContextProperty("viewcontroller", this);
	m_aboutView->rootContext()->setContextProperty("viewcontroller", this);
}

ViewController::~ViewController()
{

}


void ViewController::showAboutDialog()
{
	m_gameView->hide();
	m_aboutView->showFullScreen();
}
void ViewController::closeAboutDialog()
{
	m_aboutView->hide();
	m_gameView->showFullScreen();
}
