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

#include "freshclamprocess.h"

FreshClamProcess::FreshClamProcess(QObject *parent) : QProcess(parent){

}

bool FreshClamProcess::freshclamDaemon(QString freshclam, QStringList arguments){

    bool status = false;

    start(freshclam, arguments);

    if (waitForStarted() and state() == QProcess::Running)
        status = true;
    else
        status = false;

    return status;
}

bool FreshClamProcess::stopFreshclam(){

    /*
     * TODO:
     * A better way to stop the daemon.
     * I'm too stupid for Threads.
     */
    start("killall freshclam");

    return waitForFinished();
}

bool FreshClamProcess::freshclamManuelly(QString freshclam, QStringList arguments){

    return startDetached(freshclam, arguments);
}

bool FreshClamProcess::freshclamRunning(){

    start("ps -C freshclam");
    waitForFinished();

    QString output = readAll();

    bool status = output.contains("freshclam", Qt::CaseInsensitive);

    return status;
}

