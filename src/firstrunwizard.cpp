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

#include "firstrunwizard.h"

FirstRunWizard::FirstRunWizard(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);
    setWindowIcon(QIcon::fromTheme("tools-wizard"));
    setFixedSize(700, 455);

    page(0)->setPixmap(QWizard::WatermarkPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav-128.png")));
    page(0)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(0)->setTitle(
                trUtf8("Der <b>ClamUI</b> Einrichtungsassistent hilft "
                       "Ihnen dabei das Programm in drei Schritten "
                       "einzurichten.<br />"));
    page(0)->setSubTitle(
                trUtf8("Klichen Sie auf <b>Weiter</b>, um mit der "
                       "Einrichtung zu beginnen."));

    page(1)->setPixmap(QWizard::WatermarkPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav-128.png")));
    page(1)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(1)->setTitle(
                trUtf8("Angeben, wie und wann der Virenscanner "
                       "ausgeführt werden soll.<br />"));
    page(1)->setSubTitle(
                trUtf8("1. Schritt: Den Virenscanner <b>ClamAV</b> "
                       "einrichten."));

    page(2)->setPixmap(QWizard::WatermarkPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav-128.png")));
    page(2)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(2)->setTitle(
                trUtf8("Den Aktualisierungsinterval für die "
                       "Virendatenbank einrichten.<br />"));
    page(2)->setSubTitle(
                trUtf8("2. Schritt: Das Programm <b>FreshClam</b> "
                       "einrichten."));

    page(3)->setPixmap(QWizard::WatermarkPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav-128.png")));
    page(3)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(3)->setTitle(
                trUtf8("Verzeichnis zu den Programmen setzen.<br />"));
    page(3)->setSubTitle(
                trUtf8("3. Schritt: Wählen Sie das Verzeichnis aus, "
                       "in den sich die Programme befinden"));

//    page(4)->setPixmap(QWizard::WatermarkPixmap,
//                       QPixmap(QString(":/icons/icons/png/clamui-128.png")));
//    page(4)->setPixmap(QWizard::LogoPixmap,
//                       QPixmap(QString(":/icons/icons/png/clamui.png")));
//    page(4)->setTitle(
//                trUtf8("Abschluss der Konfiguration.<br />"));
//    page(4)->setSubTitle(
//                trUtf8("4. Schritt: Ihre Angaben überprüfen und "
//                       "anschließend auf <b>Abschließen</b> klicken."));
    /*
     * Load scheduling values in the comboBox_Scheduling
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

    /*
     * Load path values in the comboBox_ClamVDB.
     */
    comboBox_ClamVDB->addItem(CLAMAV_VDB_PATH);
    comboBox_ClamVDB->setItemIcon(0, QIcon::fromTheme("folder"));
    comboBox_ClamVDB->addItem("/usr/local/share/clamav/");
    comboBox_ClamVDB->setItemIcon(1, QIcon::fromTheme("folder"));
    comboBox_ClamVDB->addItem("/usr/share/clamav/");
    comboBox_ClamVDB->setItemIcon(2, QIcon::fromTheme("folder"));
    comboBox_ClamVDB->addItem("/var/lib/clamav/");
    comboBox_ClamVDB->setItemIcon(3, QIcon::fromTheme("folder"));

    /*
     * Load path values in the comboBox_ConfigPath.
     */
    comboBox_ConfigPath->addItem(CLAMAV_PATH);
    comboBox_ConfigPath->setItemIcon(0, QIcon::fromTheme("folder"));
    comboBox_ConfigPath->addItem("/usr/local/share/clamav/");
    comboBox_ConfigPath->setItemIcon(1, QIcon::fromTheme("folder"));
    comboBox_ConfigPath->addItem("/usr/share/clamav/");
    comboBox_ConfigPath->setItemIcon(2, QIcon::fromTheme("folder"));
    comboBox_ConfigPath->addItem("/var/lib/clamav/");
    comboBox_ConfigPath->setItemIcon(3, QIcon::fromTheme("folder"));
    comboBox_ConfigPath->addItem("/usr/local/etc/");
    comboBox_ConfigPath->setItemIcon(4, QIcon::fromTheme("folder"));

    // Set current date und current time
    dateEdit_ScheduleDate->setDate(QDate::currentDate());
    timeEdit_ScheduleTime->setTime(QTime::currentTime());

    if (comboBox_Scheduling->currentIndex() == 0){

        dateEdit_ScheduleDate->setEnabled(false);
        timeEdit_ScheduleTime->setEnabled(false);
    }

    button(QWizard::FinishButton)->setEnabled(false);
    label_TextFinished->setHidden(true);

    connect(comboBox_Scheduling, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotComboBoxSchedulingIndexChanged()));
    connect(button(QWizard::FinishButton), SIGNAL(clicked(bool)),
            this, SLOT(settingsWrite()));
    connect(this, SIGNAL(currentIdChanged(int)),
            this, SLOT(slotCheckPath()));
    connect(comboBox_ProgramPath, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCheckPath()));
    connect(comboBox_DaemonPath, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCheckPath()));
}

void FirstRunWizard::changeEvent(QEvent *e){

    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void FirstRunWizard::slotCheckPath(){

    if (currentId() == 3){
        QString pathPrograms = comboBox_ProgramPath->currentText();
        QString pathDeamon = comboBox_DaemonPath->currentText();
        QFile programPath, daemonPath;

        /*
         * Check where the ClamAV programs are located.
         */
        if (programPath.exists(pathPrograms + "clamscan")
                and programPath.exists(pathPrograms + "freshclam")){
            label_ProgramPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-ok.svg"));
        } else {
            label_ProgramPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-close.svg"));
            button(QWizard::FinishButton)->setEnabled(false);
            label_TextFinished->setHidden(true);
        }

        /*
         * Check where the ClamAV daemon is located.
         */
        if (daemonPath.exists(pathDeamon + "clamd")){
            label_DaemonPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-ok.svg"));
        } else {
            label_DaemonPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-close.svg"));
            button(QWizard::FinishButton)->setEnabled(false);
            label_TextFinished->setHidden(true);
        }

        /*
         * If all ok show finish button and greeter text.
         */
        if (programPath.exists(pathPrograms + "clamscan")
                and daemonPath.exists(pathDeamon + "clamd")) {
            button(QWizard::FinishButton)->setEnabled(true);
            label_TextFinished->setVisible(true);
        }
    }
}

void FirstRunWizard::slotComboBoxSchedulingIndexChanged(){

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

void FirstRunWizard::settingsWrite(){

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("Freshclam");
    clamui_conf.setValue("Start_As_Demon", checkBox_FreshClam->isChecked());
    clamui_conf.setValue("Update_Interval", spinBox_FreshClamUpdate->value());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamUI");
    clamui_conf.setValue("Autostart", checkBox_Autostart->isChecked());
    clamui_conf.setValue("Hide_Window", checkBox_HideWindow->isChecked());
    clamui_conf.setValue("Icon_on_Desktop", checkBox_IconOnDesktop->isChecked());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("ClamAV");
    clamui_conf.setValue("Schedule_Index", comboBox_Scheduling->currentIndex());
    clamui_conf.setValue("Schedule_Date", dateEdit_ScheduleDate->date());
    clamui_conf.setValue("Schedule_Time", timeEdit_ScheduleTime->time());
    clamui_conf.setValue("Program_Path_Id", comboBox_ProgramPath->currentIndex());
    clamui_conf.setValue("Program_Path", comboBox_ProgramPath->currentText());
    clamui_conf.setValue("Daemon_Path_Id", comboBox_DaemonPath->currentIndex());
    clamui_conf.setValue("Daemon_Path", comboBox_DaemonPath->currentText());
    clamui_conf.setValue("VirusDB_Path_Id", comboBox_ClamVDB->currentIndex());
    clamui_conf.setValue("VirusDB_Path", comboBox_ClamVDB->currentText());
    clamui_conf.setValue("Config_Path_Id", comboBox_ConfigPath->currentIndex());
    clamui_conf.setValue("Config_Path", comboBox_ConfigPath->currentText());
    clamui_conf.endGroup();
}
