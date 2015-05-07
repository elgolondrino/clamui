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

#include "tabscanscheduling.h"

TabScanScheduling::TabScanScheduling(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    createSlots();

    settingsRead();
}

void TabScanScheduling::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}


void TabScanScheduling::createSlots(){

    connect(checkBox_FreshClam, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(spinBox_FreshClamUpdate, SIGNAL(valueChanged(int)),
            this, SLOT(settingsWrite()));
    connect(spinBox_Hourly, SIGNAL(valueChanged(int)),
            this, SLOT(settingsWrite()));
    connect(timeEdit_Daily, SIGNAL(timeChanged(QTime)),
            this, SLOT(settingsWrite()));
    connect(checkBox_ExternalDevice, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(radioButton_Daily, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(radioButton_Hourly, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
}

void TabScanScheduling::settingsWrite(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("Freshclam");
    clamui_conf.setValue("Start_As_Demon", checkBox_FreshClam->isChecked());
    clamui_conf.setValue("Update_Interval", spinBox_FreshClamUpdate->value());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    clamui_conf.setValue("HourInterval", spinBox_Hourly->value());
    clamui_conf.setValue("DayInterval", timeEdit_Daily->time());
    clamui_conf.setValue("ScanExternal", checkBox_ExternalDevice->isChecked());
    clamui_conf.setValue("ScanDaily", radioButton_Daily->isChecked());
    clamui_conf.setValue("ScanHourly", radioButton_Hourly->isChecked());
    clamui_conf.endGroup();

}

void TabScanScheduling::settingsWriteDirectories(){

    QList<Directory> directories;

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginWriteArray("DirectoriesExclude");
    for (int i = 0; i < directories.size(); ++i) {
        clamui_conf.setArrayIndex(i);
        clamui_conf.setValue("Directory", directories.at(i).directoryName);
    }
    clamui_conf.endArray();
}

void TabScanScheduling::settingsWriteFiles(){

    QList<File> files;

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginWriteArray("DirectoriesFiles");
    for (int i = 0; i < files.size(); ++i) {
        clamui_conf.setArrayIndex(i);
        clamui_conf.setValue("Files", files.at(i).fileName);
    }
    clamui_conf.endArray();
}

void TabScanScheduling::settingsRead(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("Freshclam");
    checkBox_FreshClam->setChecked(clamui_conf.value("Start_As_Demon", false).toBool());
    spinBox_FreshClamUpdate->setValue(clamui_conf.value("Update_Interval", 12).toInt());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    spinBox_Hourly->setValue(clamui_conf.value("HourInterval", 6).toInt());
    timeEdit_Daily->setTime(clamui_conf.value("DayInterval", "03:00").toTime());
    checkBox_ExternalDevice->setChecked(clamui_conf.value("ScanExternal", false).toBool());
    radioButton_Daily->setChecked(clamui_conf.value("ScanDaily", true).toBool());
    radioButton_Hourly->setChecked(clamui_conf.value("ScanHourly", false).toBool());
    clamui_conf.endGroup();

    if (checkBox_FreshClam->isChecked()) {
        label_2->setEnabled(true);
        spinBox_FreshClamUpdate->setEnabled(true);
    }
}


