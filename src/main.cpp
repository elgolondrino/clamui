/******************************************************************************
**
** Copyright(C) by Georg Lassnig
**
** Contact e-mail     : g.lassnig@e67-its.de
** Program URL        : http://www.e67-its.de/
**
** Developed with     : Qt Creator
** Programing Language: C++
** Depend on          : Qt4/Qt5
**
** License Information:
**
** $QT_BEGIN_LICENSE:GPL$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; WITHOUT even the implied WARRANTY OF
** MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE. See the
** GNU General PUBLIC License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/> .
**
** $QT_END_LICENSE$
**
*******************************************************************************/

/* KDE Headers */

/* Qt Headers */
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QSettings>
#include <QLocale>

#include "clamui.h"
#include "definitionen.h"

int main(int argc, char *argv[]) {
    QDir clamDir;

    QApplication app(argc, argv);
    QTranslator myappTranslator;

    QString locale = QLocale::system().name();
    QString defaultLanguage = QString(APP_NAME) + "_" + locale + ".qm";
    QString currLanguage, gamePath;
    bool languageSet;

    // Falls die Sprache manuell gesetzt wurde.
    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_COMPANY_NAME, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    languageSet = clamui_conf.value("Set_Language", false).toBool();
    currLanguage = clamui_conf.value(
                     "languageFileName", defaultLanguage).toString();
    clamui_conf.endGroup();

    // Systemsprache aus der Umgebungsvariable des Systems lesen.
    // Read the system language from the system environment.
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    if (languageSet == false) {
        myappTranslator.load(LANG_PATH + APP_NAME + "_"  + locale + ".qm");
        app.installTranslator(&myappTranslator);
      } else {
        myappTranslator.load(LANG_PATH + currLanguage);
        app.installTranslator(&myappTranslator);
      }

    if (!clamDir.exists(CLAMAV_PATH)) {
        clamDir.mkdir(CLAMAV_PATH);
    }

        ClamUI start;
        start.show();
        return app.exec();
  } // main end

//--- main.cpp end ---
