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

#include "languagetools.h"

LanguageTools::LanguageTools()
{
	this->readLanguages();
}

LanguageTools::~LanguageTools() {

	delete languageInfoMap;

}

void LanguageTools::readLanguages() {

	/*
		Here we read and create a map to hold the informations about
		Language names
	*/

	languageInfoMap = new QMap<QString, LanguageInfo>;

	QFile translationsFile(":/translations/translations/translation_info.txt");

	if (!translationsFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&translationsFile);

	//We have to initialize en language by hand

	//	LanguageInfo info;
	//	info.niceName = "English";
	//	info.flagFile = "english.png";
	//
	//	languageInfoMap->insert("mysqloader_en.qm", info);

	while (!in.atEnd()) {

		QString line = in.readLine();

		/*Each line has the format:
			translation_file,Language Name,flag file
		*/
		QStringList languageData = line.split(",");

		LanguageInfo info;
		info.niceName = languageData[1];
		info.flagFile = languageData[2];

		languageInfoMap->insert(languageData[0], info);

	}

}

const LanguageInfo LanguageTools::getLanguageInfo(const QString & language) const {
	return languageInfoMap->value(language);
}
