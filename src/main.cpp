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

#include <QtGui/QApplication>
#include <QtDeclarative>
#ifdef MEEGO_EDITION_HARMATTAN
#include <MDeclarativeCache>
#include <gconf/gconf-client.h>
#endif

#include <QLocale>
#include <QTranslator>
#include <QDebug>

#include "game.h"
#include "settings.h"
#include "boardiconprovider.h"
#include "historymodel.h"
#include "viewcontroller.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef MEEGO_EDITION_HARMATTAN
    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
#else
    QApplication *app = new QApplication(argc, argv);
    QDeclarativeView *view = new QDeclarativeView();
    QDeclarativeView *aboutview = new QDeclarativeView();
#endif
#ifdef Q_WS_MAEMO_5
    view->setAttribute(Qt::WA_Maemo5LandscapeOrientation);
    view->setAttribute(Qt::WA_Maemo5NonComposited);
    view->connect(view->engine(), SIGNAL(quit()), SLOT(close()));
#endif

    // For QSettings
    QCoreApplication::setOrganizationName("AnttiPohjola");
    QCoreApplication::setApplicationName("Nine Men's Morris");
/*
#ifndef Q_WS_MAEMO_5
    // Qt 4.7.0 doesn't have qsTrId() support in QML
    GConfClient *gconf = gconf_client_get_default();
    QString locale = gconf_client_get_string(gconf, "/meegotouch/i18n/region", NULL);
    QTranslator translator;
    if (translator.load("qmuehle_" + locale, "/usr/share/l10n/meegotouch"))
        app->installTranslator(&translator);
    else
        qDebug() << "Failed to load translation" <<  ("/usr/share/l10n/meegotouch/qmuehle_" + locale +".qm");
#endif
*/
    // Export the Game and Settings classes through the Muehle 1.0 QML module
    qmlRegisterType<Game>("Muehle", 1, 0, "Game");
    qmlRegisterType<HistoryModel>("Muehle", 1, 0, "HistoryModel");
    qmlRegisterType<Settings>("Muehle", 1, 0, "Settings");

    view->setSource(QUrl("qrc:/qml/main.qml"));
    aboutview->setSource(QUrl("qrc:/qml/AboutPage.qml"));

    ViewController* controller = new ViewController(view, aboutview);

    //"image://myimageprovider/image.png"
    view->engine()->addImageProvider("boardicons", new BoardIconProvider());

    // FIXME - for now, don't show fullscreen if screen height >= 800 or width >= 1024
/*    QRect screenGeometry=QApplication::desktop()->screenGeometry();
    if((screenGeometry.height()<800) && (screenGeometry.width()<1024))
         view->showFullScreen();
    else view->show();
*/
    view->showFullScreen();
    return app->exec();
}
