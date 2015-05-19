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

    settingsRead();
    createSlots();

    /*
     * Load values to the tableview.
     */
    databaseRead();

    slotDaemonStatus();

    QTimer *daemonStatus = new QTimer;
    connect(daemonStatus,
            SIGNAL(timeout()),
            this,
            SLOT(slotDaemonStatus()));
    daemonStatus->start(1000);
}

void TabScanScheduling::slotDaemonStatus() {

    /*
     * If clamd not running.
     */
    if (!QFile::exists("/tmp/clamd.socket")){

        pushButton_CamdStatus->setText(trUtf8("Starten"));
        label_CamdStatus->setText(trUtf8("<b>Der ClamAV Dämon läuft nicht.</b>"));
        label_CamdStatus->setStyleSheet("color: rgb(255, 0, 0);");
    } else {

        pushButton_CamdStatus->setText(trUtf8("Stoppen"));
        label_CamdStatus->setText(trUtf8("<b>Der ClamAV Dämon läuft.</b>"));
        label_CamdStatus->setStyleSheet("color: rgb(0, 170, 0);");
    }

    /*
     * If freshclam not running.
     */
    if (!QFile::exists(configPath + "freshclam.pid")) {

        pushButton_FreshclamStatus->setText(trUtf8("Starten"));
        label_FreshclamStatus->setText(trUtf8("<b>Läuft nicht.</b>"));
        label_FreshclamStatus->setStyleSheet("color: rgb(255, 0, 0);");
    } else {

        pushButton_FreshclamStatus->setText(trUtf8("Stoppen"));
        label_FreshclamStatus->setText(trUtf8("L<b>äuft</b>"));
        label_FreshclamStatus->setStyleSheet("color: rgb(0, 170, 0);");
    }
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

    connect(pushButton_FilesExclude, SIGNAL(clicked(bool)),
            this, SLOT(addFile()));

    connect(pushButton_DirectoriesExclude, SIGNAL(clicked(bool)),
            this, SLOT(addDirectory()));

    connect(pushButton_ClearListDirectories, SIGNAL(clicked(bool)),
            this, SLOT(removeFromDB()));

    connect(groupBox_Eclude, SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));

    connect(pushButton_FreshclamStatus, SIGNAL(clicked(bool)),
            this, SLOT(slotStartStopFreshclam()));

    connect(pushButton_CamdStatus, SIGNAL(clicked(bool)),
            this, SLOT(slotStartStopClamd()));

    connect(comboBox_Scheduling, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotComboBoxSchedulingIndexChanged()));

    connect(comboBox_Scheduling, SIGNAL(currentIndexChanged(int)),
            this, SLOT(settingsWrite()));
}

void TabScanScheduling::slotStartStopClamd(){

    QStringList arguments;
    arguments << "--config-file=" + configPath + "clamd.conf";

    ClamdProcess start;
    start.clamDaemon(daemonPath + "clamd", arguments);
}

void TabScanScheduling::slotStartStopFreshclam(){

    QStringList arguments;
    arguments << "--config-file=" + configPath +  "freshclam.conf"
              << "--log=" + configPath + "freshclam.log"
              << "--daemon"
              << "--pid=" + configPath + "freshclam.pid"
              << "--checks=" + spinBox_FreshClamUpdate->value()
              << "--datadir=" + virusdbPath
              << "--daemon-notify=" + configPath + "clamd.conf"
              << "--enable-stats";

    FreshClamProcess start;
    start.freshclamDaemon(programPath + "freshclam", arguments);
}

void TabScanScheduling::enableGroupBoxes(){

    if (!groupBox_Eclude->isChecked()) {
        groupBox_ExlcludeDir->setEnabled(true);
        databaseRead();
    } else {
        groupBox_ExlcludeDir->setEnabled(false);
    }
}

void TabScanScheduling::slotComboBoxSchedulingIndexChanged(){

    if (comboBox_Scheduling->currentIndex() == 0
            or comboBox_Scheduling->currentIndex() == 1
            or comboBox_Scheduling->currentIndex() == 2){

        pushButton_DateTimeSave->setEnabled(false);
        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(false);
    }
    if (comboBox_Scheduling->currentIndex() == 3
            or comboBox_Scheduling->currentIndex() == 4
            or comboBox_Scheduling->currentIndex() == 6){

        // Set current date und current time
        dateEdit_ScheduleDate->setDate(QDate::currentDate());
        timeEdit_ScheduleTime->setTime(QTime::currentTime());

        pushButton_DateTimeSave->setEnabled(true);
        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(true);
    }
    if (comboBox_Scheduling->currentIndex() == 5
            or comboBox_Scheduling->currentIndex() == 7
            or comboBox_Scheduling->currentIndex() == 8){

        // Set current date und current time
        dateEdit_ScheduleDate->setDate(QDate::currentDate());
        timeEdit_ScheduleTime->setTime(QTime::currentTime());

        pushButton_DateTimeSave->setEnabled(true);
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
    configPath =  clamui_conf.value("Config_Path", CLAMAV_PATH).toString();
    virusdbPath =  clamui_conf.value("VirusDB_Path", CLAMAV_VDB_PATH).toString();
    daemonPath = clamui_conf.value("Daemon_Path", "/usr/sbin/").toString();
    programPath = clamui_conf.value("Program_Path", "/usr/bin/").toString();
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
 * Add files to exlude from scanning.
 */
void TabScanScheduling::addFile(){

    QStringList file;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, false);
    fileDialog.exec();

    file = fileDialog.selectedFiles();

    saveToDB(file);

}

/*
 * Add directories to exlude from scanning.
 */
void TabScanScheduling::addDirectory(){

    QStringList directory;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    fileDialog.exec();

    directory = fileDialog.selectedFiles();

    saveToDB(directory);

}

void TabScanScheduling::saveToDB(QStringList values){

    QString value = values.value(0);
    if (!value.isEmpty()) {
        QSqlQueryModel save;
        save.setQuery("INSERT INTO dir_files (value) VALUES('" + value + "');");

//        qDebug() << save.lastError();
    }

    databaseRead();

}

/*
 * Load excluded directories to Tableview.
 */
void TabScanScheduling::databaseRead(){

    QSqlQueryModel *query = new QSqlQueryModel;
    query->clear();
    query->setQuery("SELECT DISTINCT value FROM dir_files "
                    "ORDER BY value ASC");

    tableView_DirectoriesExclude->setModel(query);
}

/*
 * Delete excluded files and directories from Tableview and databse.
 */
void TabScanScheduling::removeFromDB(){

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

        deleteData.exec("DELETE FROM dir_files WHERE value = '"
                        + dataDelete + "'");
//        qDebug() << deleteData.lastError();

        databaseRead();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}



