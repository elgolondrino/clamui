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
    createTrayIcon("clamui", "ClamAV ist derzeit inaktiv.");

    if (freshclamAsDaemon)
        freshclamDaemon();

    clamDaemon();
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
    if (statusNotifierItem->standardActionsEnabled() == true) {

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

    QMessageBox msgBox;

    msgBox.setWindowTitle(trUtf8("Beenden bestätigen"));
    msgBox.setText(trUtf8("Möchten Sie <b>") +
                          APP_TITLE + trUtf8("</b> wirklich beenden?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
//        startDaemon.stopDaemon();
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
    resize(clamui_conf.value("Window_Size", QSize(900, 500)).toSize());
//    resize(size);
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
    clamui_conf.endGroup();

    clamui_conf.beginGroup("Freshclam");
    freshclamAsDaemon = clamui_conf.value("Start_As_Demon", false).toBool();
    freshclamInterval = clamui_conf.value("Update_Interval", false).toString();
    clamui_conf.endGroup();
}

void ClamUI::createTrayIcon(QString iconSysTray, QString statusMessage){


    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(action_Handbook);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(action_Settings);
    trayIconMenu->setTitle(QString("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));

    statusNotifierItem = new KStatusNotifierItem(this);
    statusNotifierItem->setTitle(QString("%1 - %2").arg(APP_TITLE).arg(APP_VERSION));
    statusNotifierItem->setToolTip("clamui",
                                   APP_TITLE " - " APP_VERSION,
                                   statusMessage);
    statusNotifierItem->setIconByName(iconSysTray);
    statusNotifierItem->setStatus(KStatusNotifierItem::Active);
    statusNotifierItem->setContextMenu(trayIconMenu);

}

void ClamUI::clamDaemon(){

    QStringList arguments;
    arguments << "--config-file=" + configPath + "clamd.conf";

    Clam_Processes startDaemon;
    bool running = startDaemon.clamDaemon(daemonPath + "clamd", arguments);

    if (running) {
        statusNotifierItem->showMessage(

                    trUtf8("%1 - %2 - Hinweis!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>CLamAV Dämon</b> wurde erfolgreich gestartet."),
                    "dialog-information",
                    5000 );

    } else if (!running) {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>CLamAV Dämon</b> konnte <b>nicht</b> gestartet werden."),
                    "dialog-warning",
                    5000 );
    }

}

void ClamUI::freshclamDaemon(){

    QStringList arguments;
    arguments << "--config-file=" + configPath +  "freshclam.conf"
              << "--log=" + configPath + "freshclam.log"
              << "--daemon"
              << "--pid=" + configPath + "freshclam.pid"
              << "--checks=" + freshclamInterval
              << "--datadir=" + virusdbPath
              << "--daemon-notify=" + configPath + "clamd.conf"
              << "--enable-stats";

    FreshClamProsses *startFreshClam = new FreshClamProsses;

    if (startFreshClam->freshclamDaemon(programPath + "freshclam", arguments)) {
        statusNotifierItem->showMessage(

                    trUtf8("%1 - %2 - Hinweis!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Datenbank Update Dämon</b> wurde "
                           "erfolgreich gestartet und aktualisiert ")
                    + freshclamInterval + trUtf8("x am Tag die "
                                                 "Virendatenbank."),
                    "dialog-information",
                    5000 );

    } else {

        statusNotifierItem->showMessage(
                    trUtf8("%1 - %2 - Warnung!").arg(
                        APP_TITLE).arg(
                        APP_VERSION),
                    trUtf8("Der <b>Datenbank Update Dämon</b> konnte <b>nicht</b> "
                           "gestartet werden."),
                    "dialog-warning",
                    5000 );
    }
}
