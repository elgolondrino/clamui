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

    page(0)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(0)->setTitle(
                trUtf8("Der <b>ClamUI</b> Einrichtungsassistent hilft "
                       "Ihnen dabei das Programm in drei Schritten "
                       "einzurichten.<br />"));
    page(0)->setSubTitle(
                trUtf8("Klichen Sie auf <b>Weiter</b>, um mit der "
                       "Einrichtung zu beginnen."));

    page(1)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav.png")));
    page(1)->setTitle(
                trUtf8("Angeben, wie und wann der Virenscanner "
                       "ausgeführt werden soll.<br />"));
    page(1)->setSubTitle(
                trUtf8("1. Schritt: Den Virenscanner <b>ClamAV</b> "
                       "einrichten."));

    page(2)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamav.png")));
    page(2)->setTitle(
                trUtf8("Den Aktualisierungsinterval für die "
                       "Virendatenbank einrichten.<br />"));
    page(2)->setSubTitle(
                trUtf8("2. Schritt: Das Programm <b>FreshClam</b> "
                       "einrichten."));

    page(3)->setPixmap(QWizard::LogoPixmap,
                       QPixmap(QString(":/icons/icons/png/clamui.png")));
    page(3)->setTitle(
                trUtf8("Abschluss der Konfiguration.<br />"));
    page(3)->setSubTitle(
                trUtf8("3. Schritt: Ihre Angaben überprüfen und "
                       "anschließend auf <b>Abschließen</b> klicken."));

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

}

void FirstRunWizard::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
