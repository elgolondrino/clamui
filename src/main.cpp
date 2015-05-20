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
#include <KF5/kdelibs4support_version.h>

/* Qt Headers */
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QSettings>
#include <QLocale>

#include "clamui.h"
#include "definitionen.h"
#include "sqlite_db.h"
#include "clamdprocess.h"
#include "freshclamprocess.h"
#include "firstrunwizard.h"

int main(int argc, char *argv[]) {
    QDir clamDir;
    QFile dbFile, confFile;

    QApplication app(argc, argv);
    QTranslator myappTranslator;

    QString locale = QLocale::system().name();
    QString defaultLanguage = QString(APP_NAME) + "_" + locale + ".qm";

    qDebug() << locale;

    // Falls die Sprache manuell gesetzt wurde.
    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    bool languageSet = clamui_conf.value("Language_Manually", false).toBool();
    QString currLanguage = clamui_conf.value(
                     "languageFileName", defaultLanguage).toString();
    bool hideWindow =clamui_conf.value("Hide_Window", false).toBool();
    clamui_conf.endGroup();

    // Systemsprache aus der Umgebungsvariable des Systems lesen.
    // Read the system language from the system environment.
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    if (languageSet == false) {
        // Langauge from system settings
        myappTranslator.load(LANG_PATH
                             + APP_NAME + "_"
                             + locale + ".qm");
        app.installTranslator(&myappTranslator);
      } else {
        // Language manually.
        myappTranslator.load(LANG_PATH + currLanguage);
        app.installTranslator(&myappTranslator);
      }

    /*
     * Check if there is a ClamAV directory.
     * In this directory are the config files stored.
     */
    if (!clamDir.exists(CLAMAV_PATH))
        clamDir.mkdir(CLAMAV_PATH);

    /*
     * The directory for the virus definitionen stored.
     */
    if (!clamDir.exists(CLAMAV_VDB_PATH))
        clamDir.mkdir(CLAMAV_VDB_PATH);

    /*
     * Check for an existing SQLite3 database.
     */
    if (!dbFile.exists(APP_CONFIG_PATH + SQLITE_DB_NAME)){

        SQLite_DB sqliteDB;
        sqliteDB.connectDB();
    }

    /*
     * Check for an existing config file.
     *
     * If not run the wizard.
     */
    if (!confFile.exists(APP_CONFIG_PATH + APP_NAME + ".conf")){

        FirstRunWizard startWizard;
        startWizard.show();
        return app.exec();

    }

    /*
     * If exists run clamui.
     */
    if (confFile.exists(APP_CONFIG_PATH + APP_NAME + ".conf")){

        ClamUI startClamUI;
        if (hideWindow){

            startClamUI.hide();
            return app.exec();

        } else {

            startClamUI.show();
            return app.exec();
        }
    }

} // main end

//--- main.cpp end ---
