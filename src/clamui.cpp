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

#include "clamui.h"

ClamUI::ClamUI(QWidget *parent) : QMainWindow(parent){

    setupUi(this);

    setWindowTitle(trUtf8("%1 %2").arg(
                       APP_TITLE).arg(
                       APP_VERSION));
    setWindowIcon(QIcon::fromTheme("clamui"));

    createSlots();
    settingsRead();
    createActions();
    createTrayIcon("clamui", "ClamAV ist derzeit inaktiv.");

    clamDaemon();

    QTimer *daemonStatus = new QTimer(this);
    connect(daemonStatus,
            SIGNAL(timeout()),
            this,
            SLOT(slotDaemonStatus()));
    daemonStatus->start(60000);
}

void ClamUI::slotDaemonStatus() {

    /*
     * If clamd still running don't try to start the daemon.
     */
    if (!QFile::exists(configPath + "clamd.pid")){

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>ClamAV Dämon</b> läuft nicht."),
                    "dialog-warning",
                    10000 );
//        clamdStatus = false;
    } /*else {
        clamdStatus = true;
    }*/

    /*
     * If freshclam still running as daemon don't try to start the program.
     */
    if (!QFile::exists(configPath + "freshclam.pid")) {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Virendefinitionen Update Dämon</b> läuft nicht."),
                    "dialog-warning",
                    10000 );
//        freshclamStatus = false;
    } /*else {
        freshclamStatus = true;
    }*/
}

void ClamUI::changeEvent(QEvent *event){
    QMainWindow::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ClamUI::closeEvent(QCloseEvent *event)
{
    if (statusNotifierItem->standardActionsEnabled() == false) {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Hinweis!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Das Programm läuft im Systemabschnitt "
                           "der Kontrollleiste weiter. "
                           "Um das Programm zu beenden, "
                           "im Kontextmenü auf Beenden klicken."),
                    "dialog-information",
                    10000 );
    }
    hide();
    event->accept();

}

void ClamUI::createSlots(){

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(settingsWrite()));

    connect(pushButton_Close, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(pushButton_Close, SIGNAL(clicked(bool)),
            this, SLOT(close()));

    connect(pushButton_Settings, SIGNAL(clicked(bool)),
            this, SLOT(slotSettings()));

    connect(pushButton_About, SIGNAL(clicked(bool)),
            this ,SLOT(slotAbout()));

    connect(action_AboutClamUI, SIGNAL(triggered(bool)),
            this, SLOT(slotAbout()));

    connect(action_Quit, SIGNAL(triggered(bool)),
            this, SLOT(settingsWrite()));
    connect(action_Quit, SIGNAL(triggered(bool)),
            this, SLOT(slotQuit()));

    connect(action_Close, SIGNAL(triggered(bool)),
            this, SLOT(settingsWrite()));
    connect(action_Close, SIGNAL(triggered(bool)),
            this, SLOT(close()));

    connect(action_Settings, SIGNAL(triggered(bool)),
            this, SLOT(slotSettings()));

    connect(action_Update_VirusDB, SIGNAL(triggered(bool)),
            this, SLOT(slotUpdateVirusDB()));

    connect(action_FilesQuarantyne, SIGNAL(triggered(bool)),
            this, SLOT(slotIsolatedFiles()));

    connect(action_FilesScan, SIGNAL(triggered(bool)),
            this, SLOT(slotScanFiles()));
}

void ClamUI::slotUpdateVirusDB(){

    tabWidget->setCurrentIndex(3);
}

void ClamUI::slotIsolatedFiles(){

    tabWidget->setCurrentIndex(2);
}

void ClamUI::slotScanFiles(){

    tabWidget->setCurrentIndex(1);
}

void ClamUI::slotSettings(){

    Settings *settings = new Settings(this);
    settings->exec();

    settingsRead();

}

void ClamUI::slotAbout(){

    About_App *about = new About_App(this);
    about->exec();
}

void ClamUI::slotQuit(){

    checkBoxNoMessage = new QCheckBox;
    checkBoxNoMessage->setText(trUtf8("Die Meldung nicht mehr anzeigen."));

    ClamdProcess clamd;
    FreshClamProcess freshclam;
    QMessageBox msgBox;

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);

    if (noMessageOnQuit){
        if (stopClamdOnQuit)
            clamd.stopDaemon();

        if (stopFreshclamOnQuit)
            freshclam.stopFreshclam();

        qApp->quit();
    }

    msgBox.setWindowTitle(trUtf8("Beenden bestätigen"));
    msgBox.setText(trUtf8("Möchten Sie <b>") +
                          APP_TITLE + trUtf8("</b> wirklich beenden?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setCheckBox(checkBoxNoMessage);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        if (stopClamdOnQuit)
            clamd.stopDaemon();

        if (stopFreshclamOnQuit)
            freshclam.stopFreshclam();

        clamui_conf.beginGroup("ClamUI");
        clamui_conf.setValue("NoMessage", checkBoxNoMessage->isChecked());
        clamui_conf.endGroup();

        qApp->quit();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}

void ClamUI::settingsWrite(){
    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    clamui_conf.setValue("Currend_Tab", tabWidget->currentIndex());
    clamui_conf.setValue("Window_Size", size());
    clamui_conf.setValue("Window_Pos", pos());
    clamui_conf.endGroup();
}

void ClamUI::settingsRead(){
    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    tabWidget->setCurrentIndex(clamui_conf.value("Currend_Tab", 0).toInt());
    QPoint pos = clamui_conf.value("Window_Pos", QPoint(200, 200)).toPoint();
    QSize size = clamui_conf.value("Window_Size", QSize(900, 500)).toSize();
    noMessageOnQuit = clamui_conf.value("NoMessage", false).toBool();
    resize(size);
    move(pos);

    if (clamui_conf.value("Hide_Menubar", true).toBool()){
        menubar->setVisible(true);
    } else {
        menubar->setHidden(true);
    }
    if (clamui_conf.value("Hide_Statusbar", true).toBool()){
        statusbar->setVisible(true);
    } else {
        statusbar->setHidden(true);
    }
    if (clamui_conf.value("Hide_Toolbar", true).toBool()){
        toolBar->setVisible(true);
    } else {
        toolBar->setHidden(true);
    }
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    daemonPath = clamui_conf.value("Daemon_Path", "/usr/sbin/").toString();
    programPath = clamui_conf.value("Program_Path", "/usr/bin/").toString();
    configPath =  clamui_conf.value("Config_Path", CLAMAV_PATH).toString();
    virusdbPath =  clamui_conf.value("VirusDB_Path", CLAMAV_VDB_PATH).toString();
    stopClamdOnQuit = clamui_conf.value("StopClamdOnQuit", false).toBool();
    stopFreshclamOnQuit = clamui_conf.value("StopFreshclamOnQuit", false).toBool();
    clamui_conf.endGroup();

    clamui_conf.beginGroup("Freshclam");
    freshclamAsDaemon = clamui_conf.value("Start_As_Demon", false).toBool();
    freshclamInterval = clamui_conf.value("Update_Interval", false).toString();
    clamui_conf.endGroup();
}

void ClamUI::createActions(){

    action_Minimized = new QAction(
                trUtf8("Mi&nimieren"),
                this);
    action_Minimized->setIcon(QIcon::fromTheme(""));
    connect(action_Minimized,
            SIGNAL(triggered()),
            this,
            SLOT(hide()));

    action_Maximized = new QAction(
                trUtf8("Ma&ximieren"),
                this);
    action_Maximized->setIcon(QIcon::fromTheme(""));
    connect(action_Maximized,
            SIGNAL(triggered()),
            this,
            SLOT(showMaximized()));

    action_Restore = new QAction(
                trUtf8("&Wiederherstellen"),
                this);
    action_Restore->setIcon(QIcon::fromTheme("view-restore"));
    connect(action_Restore,
            SIGNAL(triggered()),
            this,
            SLOT(showNormal()));
}

void ClamUI::createTrayIcon(QString iconSysTray, QString statusMessage){

    trayIconMenu = new QMenu(this);
    trayIconMenu->setTitle(QString("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));
    trayIconMenu->addMenu(menuClamAV_Daemon);
    trayIconMenu->addMenu(menuFreshClam);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(action_Settings);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(action_Restore);
    trayIconMenu->addAction(action_Maximized);
    trayIconMenu->addAction(action_Minimized);
    trayIconMenu->addSeparator();
    trayIconMenu->addMenu(menu_Help);
    trayIconMenu->addAction(action_Quit);

    statusNotifierItem = new KStatusNotifierItem(this);
    statusNotifierItem->setToolTip("clamui",
                                   APP_TITLE " - " APP_VERSION,
                                   statusMessage);
    statusNotifierItem->setIconByName(iconSysTray);
    statusNotifierItem->setStandardActionsEnabled(false);
    statusNotifierItem->setCategory(KStatusNotifierItem::ApplicationStatus);
    statusNotifierItem->setStatus(KStatusNotifierItem::Active);
    statusNotifierItem->setContextMenu(trayIconMenu);
    statusNotifierItem->setTitle(QString("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));
}

void ClamUI::clamDaemon(){

    QStringList arguments;
    arguments << "--config-file=" + configPath + "clamd.conf";

    ClamdProcess *startDaemon = new ClamdProcess;

    /*
     * If clamd still running don't try to start the daemon.
     */
    if (startDaemon->clamdRunning()){

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Information!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>ClamAV Dämon</b> ist bereits aktiv."),
                    "dialog-information",
                    10000 );

        if (freshclamAsDaemon)
            freshclamDaemon();
        return;
    }

    if (startDaemon->clamDaemon(daemonPath + "clamd", arguments)) {
        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Hinweis!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>CLamAV Dämon</b> wurde erfolgreich gestartet."),
                    "dialog-information",
                    10000 );

        if (freshclamAsDaemon and startDaemon->state() == QProcess::Running)
            freshclamDaemon();

    } else {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>CLamAV Dämon</b> konnte <b>nicht</b> gestartet werden."),
                    "dialog-warning",
                    10000 );
    }
}

void ClamUI::freshclamDaemon(){

    QStringList arguments;
    arguments << "--config-file=" + configPath +  "freshclam.conf"
              << "--daemon"
              << "--checks=" + freshclamInterval
              << "--daemon-notify=" + configPath + "clamd.conf"
              << "--enable-stats";

    FreshClamProcess *startFreshClam = new FreshClamProcess;

    /*
     * If freshclam still running as daemon don't try to start the program.
     */
    if (startFreshClam->freshclamRunning()){

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Information!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Virendefinitionen Update Dämon</b> "
                           "ist bereits aktiv."),
                    "dialog-information",
                    10000 );
        return;
    }

    if (startFreshClam->freshclamDaemon(programPath + "freshclam", arguments)) {
        statusNotifierItem->showMessage(

                    trUtf8("%1 - %2 - Hinweis!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Virendefinitionen Update Dämon</b> wurde "
                           "erfolgreich gestartet und aktualisiert ")
                    + freshclamInterval + trUtf8(" mal am Tag die "
                                                 "Virendatenbank."),
                    "dialog-information",
                    10000 );
    } else {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Virendefinitionen Update Dämon</b> "
                           "konnte <b>nicht</b> gestartet werden."),
                    "dialog-warning",
                    10000 );
    }
}
