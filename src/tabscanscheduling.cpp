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

    /*
     * Open the SQLite3 database.
     */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.close();
    db.setDatabaseName(APP_CONFIG_PATH + SQLITE_DB_NAME);
    db.open();

    /*
     * Load Scheduling values in the comboBox_Scheduling
     */
    comboBox_Scheduling->addItem(trUtf8("Keine automatische Prüfung"));
    comboBox_Scheduling->addItem(trUtf8("Beim einloggen in KDE"));
    comboBox_Scheduling->addItem(trUtf8("Jeden Tag, ab jetzt"));
    comboBox_Scheduling->addItem(trUtf8("Täglich zu einer festgelegten Zeit"));
    comboBox_Scheduling->addItem(trUtf8("Wöchentlich ab jetzt, zu einer festgelegten Zeit"));
    comboBox_Scheduling->addItem(trUtf8("Wöchentlich ab diesem Termin (Datum/Zeit)"));
    comboBox_Scheduling->addItem(trUtf8("Monatlich ab jetzt, zu einer festgelegten Zeit"));
    comboBox_Scheduling->addItem(trUtf8("Monatlich ab diesem Termin (Datum/Zeit)"));
    comboBox_Scheduling->addItem(trUtf8("Nur einmal an diesem Termin (Datum/Zeit)"));

    // Set current date und current time
//    dateEdit_ScheduleDate->setDate(QDate::currentDate());
//    timeEdit_ScheduleTime->setTime(QTime::currentTime());

    settingsRead();
    createSlots();

    /*
     * Load values to the tableview.
     */
    databaseReadDirectories();
    databaseReadFiles();
}

void TabScanScheduling::changeEvent(QEvent *e){

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
    connect(timeEdit_ScheduleTime, SIGNAL(timeChanged(QTime)),
            this, SLOT(settingsWrite()));
    connect(checkBox_ExternalDevice, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(pushButton_DirectoriesExclude, SIGNAL(clicked(bool)),
            this, SLOT(saveDirectories()));
    connect(pushButton_OpenDirExclude, SIGNAL(clicked(bool)),
            this, SLOT(saveFiles()));
    connect(pushButton_ClearListExcludeFiles, SIGNAL(clicked(bool)),
            this, SLOT(removeFiles()));
    connect(pushButton_ClearListDirectories, SIGNAL(clicked(bool)),
            this, SLOT(removeDirectories()));
    connect(groupBox_Eclude, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(comboBox_Scheduling, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotComboBoxSchedulingIndexChanged()));
    connect(comboBox_Scheduling, SIGNAL(currentIndexChanged(int)),
            this, SLOT(settingsWrite()));
}

void TabScanScheduling::enableGroupBoxes(){

    if (!groupBox_Eclude->isChecked()) {
        groupBox_ExlcludeDir->setEnabled(true);
        groupBox_ExlcludeFile->setEnabled(true);
        databaseReadDirectories();
        databaseReadFiles();
    } else {
        groupBox_ExlcludeDir->setEnabled(false);
        groupBox_ExlcludeFile->setEnabled(false);
    }
}

void TabScanScheduling::slotComboBoxSchedulingIndexChanged(){

    if (comboBox_Scheduling->currentIndex() == 0
            or comboBox_Scheduling->currentIndex() == 1
            or comboBox_Scheduling->currentIndex() == 2){

        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(false);
    }
    if (comboBox_Scheduling->currentIndex() == 3
            or comboBox_Scheduling->currentIndex() == 4
            or comboBox_Scheduling->currentIndex() == 6){

        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(true);
    }
    if (comboBox_Scheduling->currentIndex() == 5
            or comboBox_Scheduling->currentIndex() == 7
            or comboBox_Scheduling->currentIndex() == 8){

        dateEdit_ScheduleDate->setEnabled(true);
        timeEdit_ScheduleTime->setEnabled(true);
    }
}

void TabScanScheduling::settingsWrite(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                          APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("Freshclam");
    clamui_conf.setValue("Start_As_Demon", checkBox_FreshClam->isChecked());
    clamui_conf.setValue("Update_Interval", spinBox_FreshClamUpdate->value());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    clamui_conf.setValue("ScanExternal", checkBox_ExternalDevice->isChecked());
    clamui_conf.setValue("Scan_All", groupBox_Eclude->isChecked());
    clamui_conf.setValue("Schedule_Index", comboBox_Scheduling->currentIndex());
    clamui_conf.setValue("Schedule_Date", dateEdit_ScheduleDate->date());
    clamui_conf.setValue("Schedule_Time", timeEdit_ScheduleTime->time());
    clamui_conf.endGroup();

    settingsRead();

}

void TabScanScheduling::settingsRead(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("Freshclam");
    checkBox_FreshClam->setChecked(
                clamui_conf.value("Start_As_Demon", false).toBool());
    spinBox_FreshClamUpdate->setValue(
                clamui_conf.value("Update_Interval", 12).toInt());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    checkBox_ExternalDevice->setChecked(
                clamui_conf.value("ScanExternal", false).toBool());
    groupBox_Eclude->setChecked(
                clamui_conf.value("Scan_All", true).toBool());
    comboBox_Scheduling->setCurrentIndex(
                clamui_conf.value("Schedule_Index", 0).toInt());
    dateEdit_ScheduleDate->setDate(
                clamui_conf.value("Schedule_Date", QDate::currentDate()).toDate());
    timeEdit_ScheduleTime->setTime(
                clamui_conf.value("Schedule_Time", QTime::currentTime()).toTime());
    clamui_conf.endGroup();

    if (checkBox_FreshClam->isChecked()) {
        label_2->setEnabled(true);
        spinBox_FreshClamUpdate->setEnabled(true);
    }

    if (comboBox_Scheduling->currentIndex() == 0
            or comboBox_Scheduling->currentIndex() == 1
            or comboBox_Scheduling->currentIndex() == 2){

        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(false);
    }
    if (comboBox_Scheduling->currentIndex() == 3
            or comboBox_Scheduling->currentIndex() == 4
            or comboBox_Scheduling->currentIndex() == 6){

        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(true);
    }
    if (comboBox_Scheduling->currentIndex() == 5
            or comboBox_Scheduling->currentIndex() == 7
            or comboBox_Scheduling->currentIndex() == 8){

        dateEdit_ScheduleDate->setEnabled(true);
        timeEdit_ScheduleTime->setEnabled(true);
    }

    enableGroupBoxes();
}

/*
 * Add directories to exlude from scanning.
 */
void TabScanScheduling::saveDirectories(){

    QStringList directory;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    fileDialog.exec();

    directory = fileDialog.selectedFiles();

    QString value = directory.value(0);
    if (!value.isEmpty()) {
        QSqlQueryModel save;
        save.setQuery("INSERT INTO directories (directory) VALUES('" + value + "');");

//        qDebug() << save.lastError();
    }

    databaseReadDirectories();

}

/*
 * Load excluded directories to Tableview.
 */
void TabScanScheduling::databaseReadDirectories(){

    QSqlQueryModel *query = new QSqlQueryModel;
    query->clear();
    query->setQuery("SELECT DISTINCT directory FROM directories "
                    "ORDER BY directory ASC");

    tableView_DirectoriesExclude->setModel(query);
}

/*
 * Delete excluded directories from Tableview and databse.
 */
void TabScanScheduling::removeDirectories(){

    QString dataDelete = tableView_DirectoriesExclude->currentIndex().data(0).toString();

    QSqlQuery deleteData;

    QMessageBox msgBox;

    msgBox.setWindowTitle(trUtf8("Löschen bestätigen"));
    msgBox.setText(trUtf8("Möchten Sie die den Eintrag <b>")
                   + dataDelete +
                   trUtf8("</b> wirklich löschen?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:

        deleteData.exec("DELETE FROM directories WHERE directory = '"
                        + dataDelete + "'");
//        qDebug() << deleteData.lastError();

        databaseReadDirectories();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}

/*
 * Add files to exlude from scanning.
 */
void TabScanScheduling::saveFiles(){

    QStringList files;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::Detail);
    if (fileDialog.exec())
        files = fileDialog.selectedFiles();

    QString value = files.value(0);

    if (!value.isEmpty()) {
        QSqlQueryModel save;
        save.setQuery("INSERT INTO files (file) VALUES('" + value + "');");
    }

    databaseReadFiles();
}

/*
 * Load excluded fieles to Tableview.
 */
void TabScanScheduling::databaseReadFiles(){

    QSqlQueryModel *query = new QSqlQueryModel;
    query->clear();
    query->setQuery("SELECT DISTINCT file FROM files "
                    "ORDER BY file ASC");

    tableView_FilesExclude->setModel(query);
}

/*
 * Delete excluded files from Tableview and databse.
 */
void TabScanScheduling::removeFiles(){

    QString dataDelete = tableView_FilesExclude->currentIndex().data(0).toString();

    QSqlQuery deleteData;

    QMessageBox msgBox;

    msgBox.setWindowTitle(trUtf8("Löschen bestätigen"));
    msgBox.setText(trUtf8("Möchten Sie die den Eintrag <b>")
                   + dataDelete +
                   trUtf8("</b> wirklich löschen?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:

        deleteData.exec("DELETE FROM files WHERE file = '"
                        + dataDelete + "'");

        databaseReadFiles();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}


