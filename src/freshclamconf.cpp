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

#include "freshclamconf.h"

FreshClamConf::FreshClamConf(QObject *parent) : QObject(parent)
{

}

bool FreshClamConf::writeFreshClamConf(QStringList values){

    QDateTime currentTime;
    QString configPath, datumZeit;

    QSettings clamui_conf(QSettings::NativeFormat, QSettings::UserScope,
                             APP_TITLE, APP_NAME);
    clamui_conf.beginGroup("ClamAV");
    configPath =  clamui_conf.value("Config_Path", CLAMAV_PATH).toString();
    clamui_conf.endGroup();

    datumZeit = currentTime.currentDateTime().toLocalTime().toString();

    QFile confFile(configPath + "freshclam.conf");
    bool write = confFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream confOutput(&confFile);

    foreach (QString value, values) {

        QStringList field = value.split(" ||| ");
        QString HTTPProxyServer = field[0];
        QString HTTPProxyPort = field[1];
        QString HTTPProxyUsername = field[2];
        QString HTTPProxyPassword = field[3];
        QString UpdateLogFile = field[4];
        QString LogVerbose = field[5];
        QString LogTime = field[6];
        QString LogSyslog = field[7];
        QString PidFile = field[8];
        QString MaxAttempts = field[9];
        QString DatabaseDirectory = field[10];
        QString DNSDatabaseInfo = field[11];
        QString DatabaseMirror = field[12];
        QString DatabaseMirror2 = field[13];

        confOutput.setCodec("UTF-8");
        confOutput
                << "####################################################################"
                << "\n"
                << "# freshclam.conf \n"
                << "#\n"
                << "# " << trUtf8("Generiert am: ") << datumZeit << "\n"
                << "#\n"
                << "# " << trUtf8("Generiert mit: ")
                << APP_TITLE << " - Version " << APP_VERSION << "\n"
                << "#\n"
                << "# " << trUtf8("Pfad: ") << configPath << "\n"
                << "#\n"
                << "####################################################################"
                << "\n\n"
                << HTTPProxyServer << "\n\n"
                << HTTPProxyPort << "\n\n"
                << HTTPProxyUsername << "\n\n"
                << HTTPProxyPassword << "\n\n"
                << UpdateLogFile << "\n\n"
                << LogVerbose << "\n\n"
                << LogTime << "\n\n"
                << LogSyslog << "\n\n"
                << PidFile << "\n\n"
                << MaxAttempts << "\n\n"
                << DatabaseDirectory << "\n\n"
                << DNSDatabaseInfo << "\n\n"
                << DatabaseMirror << "\n\n"
                << DatabaseMirror2 << "\n\n"
                << "###### END OF FILE ######\n";
    }
    return write;
}
