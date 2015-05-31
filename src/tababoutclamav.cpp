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

#include "tababoutclamav.h"

TabAboutClamAV::TabAboutClamAV(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    createSlots();
    settingsRead();
}

void TabAboutClamAV::changeEvent(QEvent *e)
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

void TabAboutClamAV::createSlots(){

    connect(pushButton_ClamdLogFile, SIGNAL(clicked(bool)),
            this, SLOT(openLogFileClamd()));

    connect(pushButton_FreshclamLogFile, SIGNAL(clicked(bool)),
            this, SLOT(openLogFileFreshclam()));
}

void TabAboutClamAV::openLogFileClamd(){

    ShowLogFile *logFile = new ShowLogFile;
    logFile->show();
    logFile->openLogFile(configPath + "clamd.log");
}

void TabAboutClamAV::openLogFileFreshclam(){

    ShowLogFile *logFile = new ShowLogFile;
    logFile->show();
    logFile->openLogFile(configPath + "freshclam.log");
}

void TabAboutClamAV::settingsRead(){


    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamAV");
    configPath =  clamui_conf.value("Config_Path", CLAMAV_PATH).toString();
    clamui_conf.endGroup();
}
