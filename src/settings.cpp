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

#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent){

    setupUi(this);

    setWindowIcon(QIcon::fromTheme("configure"));
    setWindowTitle(trUtf8("%1 %2 - Einstellungen").arg(
                       APP_TITLE).arg(
                       APP_VERSION));

    settingsRead();
    createSlots();
}

void Settings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void Settings::createSlots(){

    connect(pushButton_Default, SIGNAL(clicked(bool)),
            this, SLOT(settingsDefault()));
    connect(pushButton_Close, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    connect(pushButton_Save, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
}

void Settings::settingsWrite(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamUI");
    clamui_conf.setValue("Language_Manually", groupBox_Language->isChecked());
    clamui_conf.setValue("Language_Index", comboBox_Language->currentIndex());
    clamui_conf.setValue("Language", comboBox_Language->currentText());
    clamui_conf.setValue("Autostart", checkBox_Autostart->isChecked());
    clamui_conf.setValue("Hide_Window", checkBox_HideWindow->isChecked());
    clamui_conf.setValue("Icon_on_Desktop", checkBox_IconOnDesktop->isChecked());
    clamui_conf.setValue("Hide_Menubar", checkBox_MenuBar->isChecked());
    clamui_conf.setValue("Hide_Statusbar", checkBox_StatusBar->isChecked());
    clamui_conf.setValue("Hide_Toolbar", checkBox_ToolBar->isChecked());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    clamui_conf.setValue("Clamscan_Path", lineEdit_BinClamScan->text());
    clamui_conf.setValue("Freshclam_Path", lineEdit_BinFreshClam->text());
    clamui_conf.endGroup();
}

void Settings::settingsRead(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);

    clamui_conf.beginGroup("ClamUI");
    groupBox_Language->setChecked(
                clamui_conf.value("Language_Manually", false).toBool());
    comboBox_Language->setCurrentIndex(
                clamui_conf.value("Language_Index", 0).toInt());
    comboBox_Language->setCurrentText(
                clamui_conf.value("Language", "").toString());
    checkBox_Autostart->setChecked(
                clamui_conf.value("Autostart", false).toBool());
    checkBox_HideWindow->setChecked(
                clamui_conf.value("Hide_Window", false).toBool());
    checkBox_IconOnDesktop->setChecked(
                clamui_conf.value("Icon_on_Desktop", false).toBool());
    checkBox_MenuBar->setChecked(
                clamui_conf.value("Hide_Menubar", true).toBool());
    checkBox_StatusBar->setChecked(
                clamui_conf.value("Hide_Statusbar", true).toBool());
    checkBox_ToolBar->setChecked(
                clamui_conf.value("Hide_Toolbar", true).toBool());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    lineEdit_BinClamScan->setText(
                clamui_conf.value("Clamscan_Path", "/usr/bin/clamscan").toString());
    lineEdit_BinFreshClam->setText(
                clamui_conf.value("Freshclam_Path", "/usr/bin/freshclam").toString());
    clamui_conf.endGroup();
}

void Settings::settingsDefault(){

    QMessageBox msgBox;

    msgBox.setWindowTitle(trUtf8("Zurücksetzen bestätigen"));
    msgBox.setText(trUtf8("Möchten Sie die Werte wirklich auf die "
                          "Standardeinstellungen zurücksetzen?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        if (tabWidget_Settings->currentIndex() == 0)
            settingsDefaultClamUI();
        else if (tabWidget_Settings->currentIndex() == 1)
            settingsDefaultClamAV();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}

void Settings::settingsDefaultClamAV(){
    lineEdit_BinClamScan->setText("/usr/bin/clamscan");
    lineEdit_BinFreshClam->setText("/usr/bin/freshclam");
}

void Settings::settingsDefaultClamUI(){

    groupBox_Language->setChecked(false);
    comboBox_Language->setCurrentIndex(0);
    comboBox_Language->setCurrentText("");
    checkBox_Autostart->setChecked(false);
    checkBox_HideWindow->setChecked(false);
    checkBox_IconOnDesktop->setChecked(false);
    checkBox_MenuBar->setChecked(true);
    checkBox_StatusBar->setChecked(true);
    checkBox_ToolBar->setChecked(true);
}
