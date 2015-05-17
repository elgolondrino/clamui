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


#include "sqlite_db.h"

SQLite_DB::SQLite_DB(QObject *parent) : QObject(parent){

}

/*
 * Create database and tables.
 */
void SQLite_DB::connectDB(){

    db = QSqlDatabase::addDatabase("QSQLITE");
    // Values from definitionen.h
    db.setDatabaseName(APP_CONFIG_PATH + SQLITE_DB_NAME);

    if (db.open()){

        db.setConnectOptions();
        // Create tables.
        createTable();
    }
    db.close();
}

void SQLite_DB::createTable(){

    QSqlQuery createDirectory(db);
    createDirectory.exec("CREATE TABLE IF NOT EXISTS dir_files("
                         "`id` INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                         "`value` VARCHAR(250) NOT NULL)");
}
