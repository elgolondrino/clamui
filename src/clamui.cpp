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

    loadThemeIcons();

    setWindowTitle(trUtf8("%1 %2").arg(
                       APP_TITLE).arg(
                       APP_VERSION));
    setWindowIcon(QIcon::fromTheme("clamui"));

    createSlots();
    settingsRead();
    createActions();
    createTrayIcon("clamui", "ClamAV ist derzeit inaktiv.");

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
                    5000 );
    }
    hide();
    event->accept();

}

void ClamUI::createSlots(){

    connect(action_Quit, SIGNAL(triggered(bool)),
            this, SLOT(slotQuit()));
    connect(action_Close, SIGNAL(triggered(bool)),
            this, SLOT(close()));
    connect(pushButton_Close, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(settingsWrite()));
    connect(pushButton_Settings, SIGNAL(clicked(bool)),
            this, SLOT(slotSettings()));
    connect(pushButton_About, SIGNAL(clicked(bool)),
            this ,SLOT(slotAbout()));
    connect(action_AboutClamUI, SIGNAL(triggered(bool)),
            this, SLOT(slotAbout()));


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
    clamui_conf.endGroup();
}

void ClamUI::settingsRead(){
    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    tabWidget->setCurrentIndex(clamui_conf.value("Currend_Tab", 0).toInt());
    clamui_conf.endGroup();
}

void ClamUI::createTrayIcon(QString iconSysTray, QString statusMessage){

    statusNotifierItem = new KStatusNotifierItem(this);
    statusNotifierItem->setToolTip("clamui",
                                   APP_TITLE " - " APP_VERSION,
                                   statusMessage);
    statusNotifierItem->setIconByName(iconSysTray);
    statusNotifierItem->setStatus(KStatusNotifierItem::Active);

}

void ClamUI::createActions(){

}

void ClamUI::loadThemeIcons(){

}
