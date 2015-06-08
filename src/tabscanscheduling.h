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

#ifndef TABSCANSCHEDULING_H
#define TABSCANSCHEDULING_H

/* Qt Headers */
#include <QSettings>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QTimer>

#include "definitionen.h"
#include "sqlite_db.h"
#include "clamdprocess.h"
#include "freshclamprocess.h"

#include "ui_tabscanscheduling.h"

class TabScanScheduling : public QWidget, private Ui::TabScanScheduling
{
    Q_OBJECT

public:
    explicit TabScanScheduling(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);


private:
    void createSlots();
    void settingsRead();
    void settingsReadDirectories();
    void settingsReadFiles();
    void databaseReadDir();
    void databaseReadFile();
    void enableGroupBoxes();
    void saveToDB(QString table, QStringList values);

    QString daemonPath;
    QString programPath;
    QString virusdbPath;
    QString configPath;
    QString freshclamInterval;

    QSqlDatabase db;

private slots:
    void addFile();
    void addDirectory();
    void removeDirectoriesFromDB();
    void removeFilesFromDB();
    void settingsWrite();
    void slotDaemonStatus();
    void slotStartStopClamd();
    void slotStartStopFreshclam();
    void slotComboBoxSchedulingIndexChanged();

};

#endif // TABSCANSCHEDULING_H
