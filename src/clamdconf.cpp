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

#include "clamdconf.h"

ClamdConf::ClamdConf(QObject *parent) : QObject(parent)
{

}

void ClamdConf::writeClamdConf(QStringList values){

    QDateTime currentTime;
    QString configPath, datumZeit;

    datumZeit = currentTime.currentDateTime().toLocalTime().toString();

    QFile confFile(configPath + "clamd.conf");
    confFile.open(QIODevice::WriteOnly);
    QTextStream confOutput(&confFile);
    confOutput.setCodec("UTF-8");
    confOutput
            << "####################################################################"
            << "\n"
            << "# clamd.conf \n"
            << "#\n"
            << "# " << trUtf8("Generiert am: ") << datumZeit << "\n"
            << "#\n"
            << "# " << trUtf8("Generiert mit: ")
            << APP_TITLE << " - Version: " << APP_VERSION << "\n"
            << "#\n"
            << "# " << trUtf8("Pfad: ") << configPath
            << "#\n"
            << "####################################################################"
            << "\n\n";
}

