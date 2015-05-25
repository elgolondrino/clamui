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
                << field[0] << "\n\n"
                << field[1] << "\n\n"
                << field[2] << "\n\n"
                << field[3] << "\n\n"
                << field[4] << "\n\n"
                << field[5] << "\n\n"
                << field[6] << "\n\n"
                << field[7] << "\n\n"
                << field[8] << "\n\n"
                << field[9] << "\n\n"
                << field[10] << "\n\n"
                << field[11] << "\n\n"
                << field[12] << "\n\n"
                << field[12] << "\n\n"
                << "###### END OF FILE ######\n";
    }
    return write;
}
