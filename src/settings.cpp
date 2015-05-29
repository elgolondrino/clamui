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

    languages = new LanguageTools();
    fileToNiceName = new QMap<QString, QString>();

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

    createLanguageMenu();
    createSlots();
    settingsRead();

    tabWidget_Settings->setCurrentIndex(0);
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
    connect(comboBox_ProgramPath, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCheckPath()));
    connect(comboBox_DaemonPath, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCheckPath()));
    connect(comboBox_FcActivateLogFile, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotFcActivateLogFile()));
    connect(tabWidget_Settings, SIGNAL(currentChanged(int)),
            this, SLOT(slotFcActivateLogFile()));
    connect(comboBox_CdActivateLogFile, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCdActivateLogFile()));
    connect(tabWidget_Settings, SIGNAL(currentChanged(int)),
            this, SLOT(slotCdActivateLogFile()));
    connect(comboBox_CdScanMail, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCdScanMail()));
    connect(tabWidget_Settings, SIGNAL(currentChanged(int)),
            this, SLOT(slotCdScanMail()));
}

void Settings::slotCdScanMail(){

    if (comboBox_CdScanMail->currentIndex() == 0) {

        comboBox_CdScanPartialMessages->setEnabled(false);
        comboBox_CdPhishingSignatures->setEnabled(false);
        comboBox_CdPhishingScanURLs->setEnabled(false);
        comboBox_CdPhishingAlwaysBlockSSLMismatch->setEnabled(false);
        comboBox_CdPhishingAlwaysBlockCloak->setEnabled(false);
        comboBox_CdPartitionIntersection->setEnabled(false);
        comboBox_CdHeuristicScanPrecedence->setEnabled(false);
    } else {

        comboBox_CdScanPartialMessages->setEnabled(true);
        comboBox_CdPhishingSignatures->setEnabled(true);
        comboBox_CdPhishingScanURLs->setEnabled(true);
        comboBox_CdPhishingAlwaysBlockSSLMismatch->setEnabled(true);
        comboBox_CdPhishingAlwaysBlockCloak->setEnabled(true);
        comboBox_CdPartitionIntersection->setEnabled(true);
        comboBox_CdHeuristicScanPrecedence->setEnabled(true);
    }
}

void Settings::slotCdActivateLogFile(){

    if (comboBox_CdActivateLogFile->currentIndex() == 0) {

        lineEdit_CdLogFileSize->setEnabled(false);
        comboBox_CdTimeStamp->setEnabled(false);
        comboBox_CdVerboseLogging->setEnabled(false);
        comboBox_CdSystemLogging->setEnabled(false);
        comboBox_CdExtendedDetectionInfo->setEnabled(false);
    } else if (comboBox_CdActivateLogFile->currentIndex() == 1) {

        lineEdit_CdLogFileSize->setEnabled(true);
        comboBox_CdTimeStamp->setEnabled(true);
        comboBox_CdVerboseLogging->setEnabled(true);
        comboBox_CdSystemLogging->setEnabled(true);
        comboBox_CdExtendedDetectionInfo->setEnabled(true);
    }
}

void Settings::slotFcActivateLogFile(){

    if (comboBox_FcActivateLogFile->currentIndex() == 0) {

        lineEdit_FcLogFileSize->setEnabled(false);
        comboBox_FcTimeStamp->setEnabled(false);
        comboBox_FcVerboseLogging->setEnabled(false);
        comboBox_FcSystemLogging->setEnabled(false);
    } else if (comboBox_FcActivateLogFile->currentIndex() == 1) {

        lineEdit_FcLogFileSize->setEnabled(true);
        comboBox_FcTimeStamp->setEnabled(true);
        comboBox_FcVerboseLogging->setEnabled(true);
        comboBox_FcSystemLogging->setEnabled(true);
    }
}

void Settings::slotCheckPath(){

    if (tabWidget_Settings->currentIndex() == 1){
        QString pathPrograms = comboBox_ProgramPath->currentText();
        QString pathDeamon = comboBox_DaemonPath->currentText();
        QFile programPath, daemonPath;

        /*
         * Check where the ClamAV programs are located.
         */
        if (programPath.exists(pathPrograms + "clamscan")
                and programPath.exists(pathPrograms + "freshclam")){
            label_ProgramPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-ok.png"));
        } else {
            label_ProgramPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-close.png"));
        }

        /*
         * Check where the ClamAV daemon is located.
         */
        if (daemonPath.exists(pathDeamon + "clamd")){
            label_DaemonPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-ok.png"));
        } else {
            label_DaemonPath->setPixmap(
                        QPixmap(":/icons/icons/breeze/actions/toolbar/dialog-close.png"));
        }
    }
}

void Settings::writeFreshClamConf(){

    QString dataCollect;
    QStringList freshclamList;

    if (groupBox_FcProxy->isChecked()) {

        dataCollect += "HTTPProxyServer " + lineEdit_FcProxyServer->text();
        dataCollect += " ||| ";
        dataCollect += "HTTPProxyPort " + lineEdit_FcProxyServerPort->text();
        dataCollect += " ||| ";
        dataCollect += "HTTPProxyUsername " + lineEdit_FcProxyServerUser->text();
        dataCollect += " ||| ";
        dataCollect += "HTTPProxyPassword " + lineEdit_FcProxyServerPassword->text();
        dataCollect += " ||| ";

    } else {

        dataCollect += "#HTTPProxyServer";
        dataCollect += " ||| ";
        dataCollect += "#HTTPProxyPort ";
        dataCollect += " ||| ";
        dataCollect += "#HTTPProxyUsername ";
        dataCollect += " ||| ";
        dataCollect += "#HTTPProxyPassword ";
        dataCollect += " ||| ";
    }

    if (comboBox_FcActivateLogFile->currentIndex() == 1){

        dataCollect += "UpdateLogFile " + comboBox_ConfigPath->currentText() + "freshclam.log";
        dataCollect += " ||| ";

        if (comboBox_FcVerboseLogging->currentIndex() == 1){
            dataCollect += "LogVerbose yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogVerbose no";
            dataCollect += " ||| ";
        }

        if (comboBox_FcTimeStamp->currentIndex() == 1){
            dataCollect += "LogTime yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogVerbose no";
            dataCollect += " ||| ";
        }

        if (comboBox_FcSystemLogging->currentIndex() == 1){
            dataCollect += "LogSyslog yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogVerbose no";
            dataCollect += " ||| ";
        }

        dataCollect += "LogFileMaxSize " + lineEdit_FcLogFileSize->text();
        dataCollect += " ||| ";
    } else {

        dataCollect += "#UpdateLogFile ";
        dataCollect += " ||| ";
        dataCollect += "#LogVerbose yes";
        dataCollect += " ||| ";
        dataCollect += "#LogTime yes";
        dataCollect += " ||| ";
        dataCollect += "#LogSyslog yes";
        dataCollect += " ||| ";
        dataCollect += "#LogFileMaxSize ";
        dataCollect += " ||| ";
    }

    dataCollect += "PidFile " + comboBox_ConfigPath->currentText() + "freshclam.pid";
    dataCollect += " ||| ";
    dataCollect += "MaxAttempts " + spinBox_FcAttempts->text();
    dataCollect += " ||| ";
    dataCollect += "DatabaseDirectory " + comboBox_ClamVDB->currentText();
    dataCollect += " ||| ";
    dataCollect += "DNSDatabaseInfo " + lineEdit_FcDNSverification->text();
    dataCollect += " ||| ";
    dataCollect += "DatabaseMirror " + lineEdit_FcMirror->text();
    dataCollect += " ||| ";
    dataCollect += "DatabaseMirror " + lineEdit_FcMirrorFallBack->text();

    freshclamList << dataCollect;

    FreshClamConf conf;
    if (!conf.writeFreshClamConf(freshclamList)) {

        QMessageBox message;
        message.setWindowTitle("Warnung!");
        message.setText(trUtf8("Die Datei <b>freshclam.conf</b> "
                               "konnte nicht nach <b>")
                               + comboBox_ConfigPath->currentText()
                               + trUtf8("</b> geschrieben werden."));
        message.setInformativeText(trUtf8("Bitte vergewissern Sie sich, dass Sie "
                                          "für das angegebene Verzeichnis auch die "
                                          "benötigten Schreibrechte besitzen."));
        message.setIcon(QMessageBox::Warning);
        message.setWindowIcon(QIcon::fromTheme("dialog-warning"));
        message.exec();
    }
}

void Settings::writeClamdConf(){

    QString dataCollect;
    QStringList clamdList;

    dataCollect += "DatabaseDirectory " + comboBox_ClamVDB->currentText();
    dataCollect += " ||| ";
    dataCollect += "LocalSocket " + comboBox_ConfigPath->currentText() + "clamd.socket";
    dataCollect += " ||| ";
    dataCollect += "FixStaleSocket yes ";
    dataCollect += " ||| ";
    dataCollect += "LocalSocketMode 660 ";
    dataCollect += " ||| ";
    dataCollect += "PidFile " + comboBox_ConfigPath->currentText() + "clamd.pid";
    dataCollect += " ||| ";
    dataCollect += "TemporaryDirectory " + comboBox_CdTempDir->currentText();
    dataCollect += " ||| ";

    if (comboBox_CdLeaveTemporaryFiles->currentIndex() == 1) {
        dataCollect += "LeaveTemporaryFiles yes";
        dataCollect += " ||| ";

    } else {
        dataCollect += "LeaveTemporaryFiles no";
        dataCollect += " ||| ";
    }

    if(comboBox_CdBytecode->currentIndex() == 1){
        dataCollect += "Bytecode yes";
        dataCollect += " ||| ";

    } else {
        dataCollect += "Bytecode no";
        dataCollect += " ||| ";
    }

    dataCollect += "BytecodeSecurity " + comboBox_CdTrustSign->currentText();
    dataCollect += " ||| ";

    dataCollect += "MaxDirectoryRecursion " + spinBox_CdMaxDirectoryRecursion->text();
    dataCollect += " ||| ";

    if (comboBox_CdFollowDirectorySymlinks->currentIndex() == 1) {
        dataCollect += "FollowDirectorySymlinks yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "FollowDirectorySymlinks no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdFollowFileSymlinks->currentIndex() == 1) {
        dataCollect += "FollowFileSymlinks yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "FollowFileSymlinks no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdCrossFilesystems->currentIndex() == 1) {
        dataCollect += "CrossFilesystems yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "CrossFilesystems no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdAlgorithmicDetection->currentIndex() == 1) {
        dataCollect += "AlgorithmicDetection yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "AlgorithmicDetection no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdForceToDisk->currentIndex() == 1) {
        dataCollect += "ForceToDisk yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ForceToDisk no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdDisableCache->currentIndex() == 1) {
        dataCollect += "DisableCache yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "DisableCache no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanOLE2->currentIndex() == 1) {
        dataCollect += "ScanOLE2 yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanOLE2 no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdOLE2BlockMacros->currentIndex() == 1) {
        dataCollect += "OLE2BlockMacros yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "OLE2BlockMacros no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanPDF->currentIndex() == 1) {
        dataCollect += "ScanPDF yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanPDF no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanSWF->currentIndex() == 1) {
        dataCollect += "ScanSWF yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanSWF no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanHTML->currentIndex() == 1) {
        dataCollect += "ScanHTML yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanHTML no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanArchive->currentIndex() == 1) {
        dataCollect += "ScanArchive yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanArchive no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdArchiveBlockEncrypted->currentIndex() == 1) {
        dataCollect += "ArchiveBlockEncrypted yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ArchiveBlockEncrypted no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdDetectPUA->currentIndex() == 1) {
        dataCollect += "DetectPUA yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "DetectPUA no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanPE->currentIndex() == 1) {
        dataCollect += "ScanPE yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanPE no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdDisableCertCheck->currentIndex() == 1) {
        dataCollect += "DisableCertCheck yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "DisableCertCheck no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanELF->currentIndex() == 1) {
        dataCollect += "ScanELF yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "ScanELF no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdDetectBrokenExecutables->currentIndex() == 1) {
        dataCollect += "DetectBrokenExecutables yes";
        dataCollect += " ||| ";
    } else {
        dataCollect += "DetectBrokenExecutables no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdScanMail->currentIndex() == 1) {
        dataCollect += "ScanMail yes";
        dataCollect += " ||| ";

        if (comboBox_CdScanPartialMessages->currentIndex() == 1) {
            dataCollect += "ScanPartialMessages yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "ScanPartialMessages no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdPhishingSignatures->currentIndex() == 1) {
            dataCollect += "PhishingSignatures yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "PhishingSignatures no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdPhishingScanURLs->currentIndex() == 1) {
            dataCollect += "PhishingScanURLs yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "PhishingScanURLs no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdPhishingAlwaysBlockSSLMismatch->currentIndex() == 1) {
            dataCollect += "PhishingAlwaysBlockSSLMismatch yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "PhishingAlwaysBlockSSLMismatch no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdPhishingAlwaysBlockCloak->currentIndex() == 1) {
            dataCollect += "PhishingAlwaysBlockCloak yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "PhishingAlwaysBlockCloak no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdPartitionIntersection->currentIndex() == 1) {
            dataCollect += "PartitionIntersection yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "PartitionIntersection no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdHeuristicScanPrecedence->currentIndex() == 1) {
            dataCollect += "HeuristicScanPrecedence yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "HeuristicScanPrecedence no";
            dataCollect += " ||| ";
        }
    } else {
        dataCollect += "ScanMail no";
        dataCollect += " ||| ";
        dataCollect += "ScanPartialMessages no";
        dataCollect += " ||| ";
        dataCollect += "PhishingSignatures no";
        dataCollect += " ||| ";
        dataCollect += "PhishingScanURLs no";
        dataCollect += " ||| ";
        dataCollect += "PhishingAlwaysBlockSSLMismatch no";
        dataCollect += " ||| ";
        dataCollect += "PhishingAlwaysBlockCloak no";
        dataCollect += " ||| ";
        dataCollect += "PartitionIntersection no";
        dataCollect += " ||| ";
        dataCollect += "HeuristicScanPrecedence no";
        dataCollect += " ||| ";
    }

    if (comboBox_CdActivateLogFile->currentIndex() == 1){

        dataCollect += "LogFile " + comboBox_ConfigPath->currentText() + "clamd.log";
        dataCollect += " ||| ";

        if (comboBox_CdVerboseLogging->currentIndex() == 1) {
            dataCollect += "LogVerbose yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogVerbose no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdTimeStamp->currentIndex() == 1) {
            dataCollect += "LogTime yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogTime no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdSystemLogging->currentIndex() == 1) {
            dataCollect += "LogSyslog yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "LogSyslog no";
            dataCollect += " ||| ";
        }

        if (comboBox_CdExtendedDetectionInfo->currentIndex() == 1) {
            dataCollect += "ExtendedDetectionInfo yes";
            dataCollect += " ||| ";
        } else {
            dataCollect += "ExtendedDetectionInfo no";
            dataCollect += " ||| ";
        }

        dataCollect += "LogFileMaxSize " + lineEdit_CdLogFileSize->text();
    } else {

        dataCollect += "#LogFile ";
        dataCollect += " ||| ";
        dataCollect += "#LogVerbose yes";
        dataCollect += " ||| ";
        dataCollect += "#LogTime yes";
        dataCollect += " ||| ";
        dataCollect += "#LogSyslog yes";
        dataCollect += " ||| ";
        dataCollect += "#ExtendedDetectionInfo yes";
        dataCollect += " ||| ";
        dataCollect += "#LogFileMaxSize ";
    }

    clamdList << dataCollect;

    ClamdConf conf;
    if (!conf.writeClamdConf(clamdList)) {

        QMessageBox message;
        message.setWindowTitle("Warnung!");
        message.setText(trUtf8("Die Datei <b>clamd.conf</b> "
                               "konnte nicht nach <b>")
                               + comboBox_ConfigPath->currentText()
                               + trUtf8("</b> geschrieben werden."));
        message.setInformativeText(trUtf8("Bitte vergewissern Sie sich, dass Sie "
                                          "für das angegebene Verzeichnis auch die "
                                          "benötigten Schreibrechte besitzen."));
        message.setIcon(QMessageBox::Warning);
        message.setWindowIcon(QIcon::fromTheme("dialog-warning"));
        message.exec();
    }
}

void Settings::writeClamavMilterConf(){

    QString dataCollect;
    QStringList milterList;

}

void Settings::settingsWrite(){

    LanguageTools language;

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);

    if (tabWidget_Settings->currentIndex() == 0) {
        clamui_conf.beginGroup("ClamUI");
        clamui_conf.setValue("NoMessage", checkBox_ShowQuitMessage->isChecked());
        clamui_conf.setValue("Language_Manually", groupBox_Language->isChecked());
        //    clamui_conf.setValue("Language_Index", comboBox_Language->currentIndex());
        //    clamui_conf.setValue("languageNiceName",
        //                             comboBox_Language->currentText());
        clamui_conf.setValue("Language", comboBox_Language->currentText());

        clamui_conf.setValue("languageFileName",
                             fileToNiceName->value(
                                 comboBox_Language->currentText()));
        clamui_conf.setValue("Autostart", checkBox_Autostart->isChecked());
        clamui_conf.setValue("Hide_Window", checkBox_HideWindow->isChecked());
        clamui_conf.setValue("Icon_on_Desktop", checkBox_IconOnDesktop->isChecked());
        clamui_conf.setValue("Hide_Menubar", checkBox_MenuBar->isChecked());
        clamui_conf.setValue("Hide_Statusbar", checkBox_StatusBar->isChecked());
        clamui_conf.setValue("Hide_Toolbar", checkBox_ToolBar->isChecked());
        clamui_conf.endGroup();
    }

    if (tabWidget_Settings->currentIndex() == 1
            and toolBox_ClamAV->currentIndex() == 0) {
        clamui_conf.beginGroup("ClamAV");
        clamui_conf.setValue("Program_Path_Id",
                             comboBox_ProgramPath->currentIndex());
        clamui_conf.setValue("Program_Path",
                             comboBox_ProgramPath->currentText());
        clamui_conf.setValue("Daemon_Path_Id",
                             comboBox_DaemonPath->currentIndex());
        clamui_conf.setValue("Daemon_Path",
                             comboBox_DaemonPath->currentText());
        clamui_conf.setValue("VirusDB_Path_Id",
                             comboBox_ClamVDB->currentIndex());
        clamui_conf.setValue("VirusDB_Path",
                             comboBox_ClamVDB->currentText());
        clamui_conf.setValue("Config_Path_Id",
                             comboBox_ConfigPath->currentIndex());
        clamui_conf.setValue("Config_Path",
                             comboBox_ConfigPath->currentText());
        clamui_conf.setValue("StopClamdOnQuit",
                             checkBox_StopClamdOnQuit->isChecked());
        clamui_conf.setValue("StopFreshclamOnQuit",
                             checkBox_StopFreshclamOnQuit->isChecked());
        clamui_conf.endGroup();
    }

    if (tabWidget_Settings->currentIndex() == 1
            and toolBox_ClamAV->currentIndex() == 1) {
        clamui_conf.beginGroup("Clamd");
        clamui_conf.setValue("CdActivateLogFile",
                             comboBox_CdActivateLogFile->currentIndex());
        clamui_conf.setValue("CdSystemLogging",
                             comboBox_CdSystemLogging->currentIndex());
        clamui_conf.setValue("CdTimeStamp",
                             comboBox_CdTimeStamp->currentIndex());
        clamui_conf.setValue("CdVerboseLogging",
                             comboBox_CdVerboseLogging->currentIndex());
        clamui_conf.setValue("CdLogFileSize",
                             lineEdit_CdLogFileSize->text());
        clamui_conf.setValue("CdExtendedDetectionInfo",
                             comboBox_CdExtendedDetectionInfo->currentIndex());        
        clamui_conf.setValue("CdBytecode",
                             comboBox_CdBytecode->currentIndex());
        clamui_conf.setValue("CdTrustSign",
                             comboBox_CdTrustSign->currentIndex());
        clamui_conf.setValue("CdTempDir",
                             comboBox_CdTempDir->currentIndex());
        clamui_conf.setValue("CdCrossFilesystems",
                             comboBox_CdCrossFilesystems->currentIndex());
        clamui_conf.setValue("CdFollowDirectorySymlinks",
                             comboBox_CdFollowDirectorySymlinks->currentIndex());
        clamui_conf.setValue("CdFollowFileSymlinks",
                             comboBox_CdFollowFileSymlinks->currentIndex());
        clamui_conf.setValue("CdLeaveTemporaryFiles",
                             comboBox_CdLeaveTemporaryFiles->currentIndex());
        clamui_conf.setValue("CdMaxDirectoryRecursion",
                             spinBox_CdMaxDirectoryRecursion->value());
        clamui_conf.setValue("CdAlgorithmicDetection",
                             comboBox_CdAlgorithmicDetection->currentIndex());
        clamui_conf.setValue("CdForceToDisk",
                             comboBox_CdForceToDisk->currentIndex());
        clamui_conf.setValue("CdDisableCache",
                             comboBox_CdDisableCache->currentIndex());
        clamui_conf.setValue("CdDetectPUA",
                             comboBox_CdDetectPUA->currentIndex());
        clamui_conf.setValue("CdScanPE",
                             comboBox_CdScanPE->currentIndex());
        clamui_conf.setValue("CdDisableCertCheck",
                             comboBox_CdDisableCertCheck->currentIndex());
        clamui_conf.setValue("CdScanELF",
                             comboBox_CdScanELF->currentIndex());
        clamui_conf.setValue("CdDetectBrokenExecutables",
                             comboBox_CdDetectBrokenExecutables->currentIndex());
        clamui_conf.setValue("CdScanOLE2",
                             comboBox_CdScanOLE2->currentIndex());
        clamui_conf.setValue("CdOLE2BlockMacros",
                             comboBox_CdOLE2BlockMacros->currentIndex());
        clamui_conf.setValue("CdScanPDF",
                             comboBox_CdScanPDF->currentIndex());
        clamui_conf.setValue("CdScanSWF",
                             comboBox_CdScanSWF->currentIndex());
        clamui_conf.setValue("CdScanHTML",
                             comboBox_CdScanHTML->currentIndex());
        clamui_conf.setValue("CdScanArchive",
                             comboBox_CdScanArchive->currentIndex());
        clamui_conf.setValue("CdArchiveBlockEncrypted",
                             comboBox_CdArchiveBlockEncrypted->currentIndex());
        clamui_conf.setValue("CdScanMail",
                             comboBox_CdScanMail->currentIndex());
        clamui_conf.setValue("CdScanPartialMessages",
                             comboBox_CdScanPartialMessages->currentIndex());
        clamui_conf.setValue("CdPhishingSignatures",
                             comboBox_CdPhishingSignatures->currentIndex());
        clamui_conf.setValue("CdPhishingScanURLs",
                             comboBox_CdPhishingScanURLs->currentIndex());
        clamui_conf.setValue("CdPhishingAlwaysBlockSSLMismatch",
                             comboBox_CdPhishingAlwaysBlockSSLMismatch->currentIndex());
        clamui_conf.setValue("CdPhishingAlwaysBlockCloak",
                             comboBox_CdPhishingAlwaysBlockCloak->currentIndex());
        clamui_conf.setValue("CdPartitionIntersection",
                             comboBox_CdPartitionIntersection->currentIndex());
        clamui_conf.setValue("CdHeuristicScanPrecedence",
                             comboBox_CdHeuristicScanPrecedence->currentIndex());
        clamui_conf.endGroup();
        writeClamdConf();

    }

    if (tabWidget_Settings->currentIndex() == 1
            and toolBox_ClamAV->currentIndex() == 2) {
        clamui_conf.beginGroup("Freshclam");
        clamui_conf.setValue("FcProxy",
                             groupBox_FcProxy->isChecked());
        clamui_conf.setValue("FcProxyServer",
                             lineEdit_FcProxyServer->text());
        clamui_conf.setValue("FcProxyServerPassword",
                             lineEdit_FcProxyServerPassword->text());
        clamui_conf.setValue("FcProxyServerPort",
                             lineEdit_FcProxyServerPort->text());
        clamui_conf.setValue("FcProxyServerUser",
                             lineEdit_FcProxyServerUser->text());
        clamui_conf.setValue("FcActivateLogFile",
                             comboBox_FcActivateLogFile->currentIndex());
        clamui_conf.setValue("FcSystemLogging",
                             comboBox_FcSystemLogging->currentIndex());
        clamui_conf.setValue("FcTimeStamp",
                             comboBox_FcTimeStamp->currentIndex());
        clamui_conf.setValue("FcVerboseLogging",
                             comboBox_FcVerboseLogging->currentIndex());
        clamui_conf.setValue("FcLogFileSize",
                             lineEdit_FcLogFileSize->text());
        clamui_conf.setValue("FcAttempts",
                             spinBox_FcAttempts->value());
        clamui_conf.setValue("FcDNSverification",
                             lineEdit_FcDNSverification->text());
        clamui_conf.setValue("FcMirror",
                             lineEdit_FcMirror->text());
        clamui_conf.setValue("FcMirrorFallBack",
                             lineEdit_FcMirrorFallBack->text());
        clamui_conf.endGroup();

        writeFreshClamConf();
    }

    if (tabWidget_Settings->currentIndex() == 1
            and toolBox_ClamAV->currentIndex() == 2) {
        clamui_conf.beginGroup("ClamAVMilter");

        clamui_conf.endGroup();

        writeClamavMilterConf();
    }
}

void Settings::settingsRead(){

    QString locale = QLocale::system().name();

    QString defaultLanguage = "clamui_" + locale + ".qm";

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);

    clamui_conf.beginGroup("ClamUI");
    checkBox_ShowQuitMessage->setChecked(
                clamui_conf.value("NoMessage", false).toBool());
    groupBox_Language->setChecked(
                clamui_conf.value("Language_Manually", false).toBool());
    comboBox_Language->setCurrentIndex(
                comboBox_Language->findText(
                    clamui_conf.value("Language",
                                      defaultLanguage).toString(),
                    Qt::MatchExactly));

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
    comboBox_ProgramPath->setCurrentIndex(
                clamui_conf.value("Program_Path_Id", 0).toInt());
//  QString progranPath = clamui_conf.value("Program_Path", "/usr/bin/").toString());
    comboBox_DaemonPath->setCurrentIndex(
                clamui_conf.value("Daemon_Path_Id", 0).toInt());
//  QString daemonPath =   clamui_conf.value("Daemon_Path", "/usr/sbin/").toString());
    comboBox_ClamVDB->setCurrentIndex(
                clamui_conf.value("VirusDB_Path_Id", 0).toInt());
//  QString virusdbPath =   clamui_conf.value("VirusDB_Path", CLAMAV_VDB_PATH").toString());
    comboBox_ConfigPath->setCurrentIndex(
                clamui_conf.value("Config_Path_Id", 0).toInt());
//   QString configPath =  clamui_conf.value("Config_Path", CLAMAV_PATH).toString());
    checkBox_StopClamdOnQuit->setChecked(
                clamui_conf.value("StopClamdOnQuit", false).toBool());
    checkBox_StopFreshclamOnQuit->setChecked(
                clamui_conf.value("StopFreshclamOnQuit", false).toBool());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("Freshclam");
    groupBox_FcProxy->setChecked(
                clamui_conf.value("FcProxy", false).toBool());
    lineEdit_FcProxyServer->setText(
                clamui_conf.value("FcProxyServer").toString());
    lineEdit_FcProxyServerPassword->setText(
                clamui_conf.value("FcProxyServerPassword").toString());
    lineEdit_FcProxyServerPort->setText(
                clamui_conf.value("FcProxyServerPort").toString());
    lineEdit_FcProxyServerUser->setText(
                clamui_conf.value("FcProxyServerUser").toString());
    comboBox_FcActivateLogFile->setCurrentIndex(
                clamui_conf.value("FcActivateLogFile", 0).toInt());
    comboBox_FcSystemLogging->setCurrentIndex(
                clamui_conf.value("FcSystemLogging", 0).toInt());
    comboBox_FcTimeStamp->setCurrentIndex(
                clamui_conf.value("FcTimeStamp", 0).toInt());
    comboBox_FcVerboseLogging->setCurrentIndex(
                clamui_conf.value("FcVerboseLogging", 0).toInt());
    spinBox_FcAttempts->setValue(
                clamui_conf.value("FcAttempts", 3).toInt());
    lineEdit_FcDNSverification->setText(
                clamui_conf.value("FcDNSverification", "current.cvd.clamav.net").toString());
    lineEdit_FcLogFileSize->setText(
                clamui_conf.value("FcLogFileSize", "1M").toString());
    lineEdit_FcMirror->setText(
                clamui_conf.value("FcMirror", "db.de.clamav.net").toString());
    lineEdit_FcMirrorFallBack->setText(
                clamui_conf.value("FcMirrorFallBack", "database.clamav.net").toString());
    clamui_conf.endGroup();

    clamui_conf.beginGroup("Clamd");
    comboBox_CdActivateLogFile->setCurrentIndex(
                clamui_conf.value("CdActivateLogFile", 0).toInt());
    comboBox_CdSystemLogging->setCurrentIndex(
                clamui_conf.value("CdSystemLogging", 0).toInt());
    comboBox_CdTimeStamp->setCurrentIndex(
                clamui_conf.value("CdTimeStamp", 0).toInt());
    comboBox_CdVerboseLogging->setCurrentIndex(
                clamui_conf.value("CdVerboseLogging", 0).toInt());
    lineEdit_CdLogFileSize->setText(
                clamui_conf.value("CdLogFileSize", "1M").toString());
    comboBox_CdExtendedDetectionInfo->setCurrentIndex(
                clamui_conf.value("CdExtendedDetectionInfo", 0).toInt());
    comboBox_CdBytecode->setCurrentIndex(
                clamui_conf.value("CdBytecode", 1).toInt());
    comboBox_CdTrustSign->setCurrentIndex(
                clamui_conf.value("CdTrustSign", 1).toInt());
    comboBox_CdTempDir->setCurrentIndex(
                clamui_conf.value("CdTempDir", 0).toInt());
    comboBox_CdCrossFilesystems->setCurrentIndex(
                clamui_conf.value("CdCrossFilesystems", 1).toInt());
    comboBox_CdFollowDirectorySymlinks->setCurrentIndex(
                clamui_conf.value("CdFollowDirectorySymlinks", 0).toInt());
    comboBox_CdFollowFileSymlinks->setCurrentIndex(
                clamui_conf.value("CdFollowFileSymlinks", 0).toInt());
    comboBox_CdLeaveTemporaryFiles->setCurrentIndex(
                clamui_conf.value("CdLeaveTemporaryFiles", 0).toInt());
    spinBox_CdMaxDirectoryRecursion->setValue(
                clamui_conf.value("CdMaxDirectoryRecursion", 15).toInt());
    comboBox_CdAlgorithmicDetection->setCurrentIndex(
                clamui_conf.value("CdAlgorithmicDetection", 1).toInt());
    comboBox_CdForceToDisk->setCurrentIndex(
                clamui_conf.value("CdForceToDisk", 1).toInt());
    comboBox_CdDisableCache->setCurrentIndex(
                clamui_conf.value("CdDisableCache", 0).toInt());
    comboBox_CdDetectPUA->setCurrentIndex(
                clamui_conf.value("CdDetectPUA", 0).toInt());
    comboBox_CdScanPE->setCurrentIndex(
                clamui_conf.value("CdScanPE", 1).toInt());
    comboBox_CdDisableCertCheck->setCurrentIndex(
                clamui_conf.value("CdDisableCertCheck", 0).toInt());
    comboBox_CdScanELF->setCurrentIndex(
                clamui_conf.value("CdScanELF", 1).toInt());
    comboBox_CdDetectBrokenExecutables->setCurrentIndex(
                clamui_conf.value("CdDetectBrokenExecutables", 0).toInt());
    comboBox_CdScanOLE2->setCurrentIndex(
                clamui_conf.value("CdScanOLE2", 1).toInt());
    comboBox_CdOLE2BlockMacros->setCurrentIndex(
                clamui_conf.value("CdOLE2BlockMacros", 0).toInt());
    comboBox_CdScanPDF->setCurrentIndex(
                clamui_conf.value("CdScanPDF", 1).toInt());
    comboBox_CdScanSWF->setCurrentIndex(
                clamui_conf.value("CdScanSWF", 1).toInt());
    comboBox_CdScanHTML->setCurrentIndex(
                clamui_conf.value("CdScanHTML", 1).toInt());
    comboBox_CdScanArchive->setCurrentIndex(
                clamui_conf.value("CdScanArchive", 1).toInt());
    comboBox_CdArchiveBlockEncrypted->setCurrentIndex(
                clamui_conf.value("CdArchiveBlockEncrypted", 0).toInt());
    comboBox_CdScanMail->setCurrentIndex(
                clamui_conf.value("CdScanMail", 1).toInt());
    comboBox_CdScanPartialMessages->setCurrentIndex(
                clamui_conf.value("CdScanPartialMessages", 0).toInt());
    comboBox_CdPhishingSignatures->setCurrentIndex(
                clamui_conf.value("CdPhishingSignatures", 1).toInt());
    comboBox_CdPhishingScanURLs->setCurrentIndex(
                clamui_conf.value("CdPhishingScanURLs", 1).toInt());
    comboBox_CdPhishingAlwaysBlockSSLMismatch->setCurrentIndex(
                clamui_conf.value("CdPhishingAlwaysBlockSSLMismatch", 0).toInt());
    comboBox_CdPhishingAlwaysBlockCloak->setCurrentIndex(
                clamui_conf.value("CdPhishingAlwaysBlockCloak", 0).toInt());
    comboBox_CdPartitionIntersection->setCurrentIndex(
                clamui_conf.value("CdPartitionIntersection", 0).toInt());
    comboBox_CdHeuristicScanPrecedence->setCurrentIndex(
                clamui_conf.value("CdHeuristicScanPrecedence", 0).toInt());
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

    if (toolBox_ClamAV->currentIndex() == 0) {
        comboBox_ProgramPath->setCurrentIndex(0);
        comboBox_DaemonPath->setCurrentIndex(0);
        comboBox_ClamVDB->setCurrentIndex(0);
        comboBox_ConfigPath->setCurrentIndex(0);
        checkBox_StopClamdOnQuit->setChecked(false);
        checkBox_StopFreshclamOnQuit->setChecked(false);
    }

    if (toolBox_ClamAV->currentIndex() == 1) {

        comboBox_CdActivateLogFile->setCurrentIndex(0);
        comboBox_CdSystemLogging->setCurrentIndex(0);
        comboBox_CdTimeStamp->setCurrentIndex(0);
        comboBox_CdVerboseLogging->setCurrentIndex(0);
        lineEdit_CdLogFileSize->setText("1M");
        comboBox_CdExtendedDetectionInfo->setCurrentIndex(0);
        comboBox_CdBytecode->setCurrentIndex(1);
        comboBox_CdTrustSign->setCurrentIndex(1);
        comboBox_CdTempDir->setCurrentIndex(0);
        comboBox_CdCrossFilesystems->setCurrentIndex(1);
        comboBox_CdFollowDirectorySymlinks->setCurrentIndex(0);
        comboBox_CdFollowFileSymlinks->setCurrentIndex(0);
        comboBox_CdLeaveTemporaryFiles->setCurrentIndex(0);
        spinBox_CdMaxDirectoryRecursion->setValue(15);
        comboBox_CdAlgorithmicDetection->setCurrentIndex(1);
        comboBox_CdForceToDisk->setCurrentIndex(1);
        comboBox_CdDisableCache->setCurrentIndex(0);
        comboBox_CdDetectPUA->setCurrentIndex(0);
        comboBox_CdScanPE->setCurrentIndex(1);
        comboBox_CdDisableCertCheck->setCurrentIndex(0);
        comboBox_CdScanELF->setCurrentIndex(1);
        comboBox_CdDetectBrokenExecutables->setCurrentIndex(0);
        comboBox_CdScanOLE2->setCurrentIndex(1);
        comboBox_CdOLE2BlockMacros->setCurrentIndex(0);
        comboBox_CdScanPDF->setCurrentIndex(1);
        comboBox_CdScanSWF->setCurrentIndex(1);
        comboBox_CdScanHTML->setCurrentIndex(1);
        comboBox_CdScanArchive->setCurrentIndex(1);
        comboBox_CdArchiveBlockEncrypted->setCurrentIndex(0);
        comboBox_CdScanMail->setCurrentIndex(1);
        comboBox_CdScanPartialMessages->setCurrentIndex(0);
        comboBox_CdPhishingSignatures->setCurrentIndex(1);
        comboBox_CdPhishingScanURLs->setCurrentIndex(1);
        comboBox_CdPhishingAlwaysBlockSSLMismatch->setCurrentIndex(0);
        comboBox_CdPhishingAlwaysBlockCloak->setCurrentIndex(0);
        comboBox_CdPartitionIntersection->setCurrentIndex(0);
        comboBox_CdHeuristicScanPrecedence->setCurrentIndex(0);
    }

    if (toolBox_ClamAV->currentIndex() == 2) {

        groupBox_FcProxy->setChecked(false);
        lineEdit_FcProxyServer->clear();
        lineEdit_FcProxyServerPassword->clear();
        lineEdit_FcProxyServerPort->clear();
        lineEdit_FcProxyServerUser->clear();
        comboBox_FcActivateLogFile->setCurrentIndex(0);
        comboBox_FcSystemLogging->setCurrentIndex(0);
        comboBox_FcTimeStamp->setCurrentIndex(0);
        comboBox_FcVerboseLogging->setCurrentIndex(0);
        spinBox_FcAttempts->setValue(3);
        lineEdit_FcDNSverification->setText("current.cvd.clamav.net");
        lineEdit_FcLogFileSize->setText("1M");
        lineEdit_FcMirror->setText("db.de.clamav.net");
        lineEdit_FcMirrorFallBack->setText("database.clamav.net");
    }

    if (toolBox_ClamAV->currentIndex() == 3) {

    }

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
    checkBox_ShowQuitMessage->setChecked(false);
}

void Settings::createLanguageMenu() {

    QDir dir(LANG_PATH);
    QStringList filter;
    filter << "clamui*.qm";
    QStringList fileNames = dir.entryList(filter);

    QString languageName;
    QIcon * countryFlag;
    LanguageInfo info;
    //		info = languages->getLanguageInfo("clamui_en.qm");

    languageName = info.niceName;
    QString trlangName = tr(languageName.toStdString().c_str());

    countryFlag = new QIcon("/usr/share/" APP_NAME "/flags/" + info.flagFile);

    comboBox_Language->setInsertPolicy(QComboBox::InsertAtTop);
    comboBox_Language->insertItem(0, *(countryFlag), trlangName);

    //		fileToNiceName->insert(trlangName, "clamui_en.qm");

    for (int i = 0; i < (int)fileNames.size(); ++i) {
        info = languages->getLanguageInfo(fileNames[i]);
        languageName = info.niceName;

        trlangName = trUtf8(languageName.toStdString().c_str());
        countryFlag = new QIcon("/usr/share/" APP_NAME "/flags/" +
                                info.flagFile);

        comboBox_Language->insertItem(i+1, *(countryFlag), trlangName);

        fileToNiceName->insert(trlangName, fileNames[i]);
    }
}
