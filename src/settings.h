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

#ifndef SETTINGS_H
#define SETTINGS_H

/* Qt Headers */
#include <QSettings>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QMap>
#include <QTranslator>
#include <QMetaProperty>

#include "freshclamconf.h"
#include "clamdconf.h"
#include "clamavmilterconf.h"
#include "definitionen.h"
#include "languagetools.h"

#include "ui_settings.h"

class Settings : public QDialog, private Ui::Settings
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private:
    void createSlots();
    void settingsRead();
    void settingsDefaultClamUI();
    void settingsDefaultClamAV();
    void createLanguageMenu();

    QTranslator *appTranslator;
    LanguageTools * languages;
    QMap<QString, QString> * fileToNiceName;

private slots:
    void settingsWrite();
    void settingsDefault();
    void slotCheckPath();
};

#endif // SETTINGS_H
