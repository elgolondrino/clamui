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

#ifndef CLAMUI_H
#define CLAMUI_H

/* KF5 Headers */
#include <KStatusNotifierItem>
#include <KNotification>
#include <KDESu/SuProcess>
#include <KF5/Solid/Solid/StorageDrive>
#include <KF5/Solid/Solid/StorageAccess>

/* Qt Headers */
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QTranslator>
#include <QMap>
#include <QTimer>
#include <QMessageBox>

#include "about.h"
#include "settings.h"
#include "tababoutclamav.h"
#include "tabscanscheduling.h"
#include "tabvirusdb.h"
#include "tabvirusquarantine.h"
#include "tabvirusscan.h"
#include "definitionen.h"
#include "languagetools.h"
#include "clamdprocess.h"
#include "freshclamprocess.h"

#include "ui_clamui.h"

class ClamUI : public QMainWindow, private Ui::ClamUI
{
    Q_OBJECT

public:
    explicit ClamUI(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createSlots();
    void settingsRead();
    void createTrayIcon(QString iconSysTray, QString statusMessage);
    void clamDaemon();
    void freshclamDaemon();

    QString virusdbPath;
    QString daemonPath;
    QString configPath;
    QString programPath;
    QString freshclamInterval;

//    bool freshclamStatus;
//    bool clamdStatus;
    bool freshclamAsDaemon;
    bool stopClamdOnQuit;
    bool stopFreshclamOnQuit;
    bool noMessageOnQuit;

    KStatusNotifierItem *statusNotifierItem;
    QMenu *trayIconMenu;
    QTranslator *appTranslator;
    LanguageTools * languages;
    QMap<QString, QString> * fileToNiceName;

    QAction *action_Restore;
    QAction *action_Maximized;
    QAction *action_Minimized;
    QAction *action_Help;

    QCheckBox *checkBoxNoMessage;

private slots:
    void slotQuit();
    void slotAbout();
    void slotSettings();
    void settingsWrite();
    void slotScanFiles();
    void slotDaemonStatus();
    void slotIsolatedFiles();
    void slotUpdateVirusDB();
    void slotWindowState();
};

#endif // CLAMUI_H
