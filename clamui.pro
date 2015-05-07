lessThan(QT_VERSION, 5.3) {
    error("Program requires at least Qt 5.3!")
}

QT += network
QT += widgets
QT += KIconThemes
QT += Solid
QT += KNotifications
QT += KDESu
INCLUDEPATH += /usr/lib64

TEMPLATE = app
VERSION = -0.4.0
LANGUAGE += C++
CODECFORTR += UTF-8
UI_DIR += src
MOC_DIR += moc
OBJECTS_DIR += obj
target.path += /usr/bin
TARGET = clamui

CONFIG += qt

# #### Programmicons
pixmap.path += /usr/share/pixmaps/
pixmap.files += icons/clamui.svg

# #### Desktop-Eintrag
menu.path += /usr/share/applications/
menu.files += src/clamui.desktop

#### Übersetzungen
translationDE.path += /usr/share/locale/de/LC_MESSAGES
translationDE.files += translations/$(TARGET)_de.qm

translationEN.path += /usr/share/locale/en/LC_MESSAGES
translationEN.files += translations/$(TARGET)_en.qm

# #### Pfade zu den lokalisierten Hilfeseiten.
#helpDE.path += /usr/share/doc/kde/HTML/de/$(TARGET)
#helpDE.files += doc/de/index.cache.bz2 \
#	doc/de/index.docbook
#helpDEimg.path += /usr/share/doc/kde/HTML/de/$(TARGET)/images/
#helpDEimg.files += doc/de/images/ml_01.png \
#		  doc/de/images/ml_02.png \
#		  doc/de/images/ml_03.png \
#		  doc/de/images/ml_04.png \
#		  doc/de/images/ml_05.png

#helpEN.path += /usr/share/doc/kde/HTML/en/$(TARGET)
#helpEN.files += doc/en/index.cache.bz2 \
#	doc/en/index.docbook
#helpENimg.path += /usr/share/doc/kde/HTML/en/$(TARGET)/images/
#helpENimg.files += doc/en/images/ml_01.png \
#		  doc/en/images/ml_02.png \
#		  doc/en/images/ml_03.png \
#		  doc/en/images/ml_04.png \
#		  doc/en/images/ml_05.png

# #### Allgemeine Textdateien.
documentation.path += /usr/share/doc/packages/$(TARGET)
documentation.files += AUTHORS \
    ChangeLog \
    LICENSE \
    INSTALL \
    NEWS \
    README.md \
    TODO
    
INSTALLS += target \
    documentation \
    menu \
#	helpDE \
#	helpEN \
    translationDE \
    translationEN \
    pixmap
DEPENDPATH += . \
    gui \
    src \
    doc \
    translations \
    debian
HEADERS += src/definitionen.h \
    src/about.h \
    src/clamui.h \
    src/settings.h \
    src/tabvirusscan.h \
    src/tabvirusdb.h \
    src/tabvirusquarantine.h \
    src/tabscanscheduling.h \
    src/tababoutclamav.h \
    src/languagetools.h

SOURCES += src/about.cpp \
    src/main.cpp \
    src/clamui.cpp \
    src/settings.cpp \
    src/tabvirusscan.cpp \
    src/tabvirusdb.cpp \
    src/tabvirusquarantine.cpp \
    src/tabscanscheduling.cpp \
    src/tababoutclamav.cpp \
    src/languagetools.cpp
    
FORMS += ui/about.ui \
    ui/clamui.ui \
    ui/settings.ui \
    ui/tabvirusscan.ui \
    ui/tabvirusdb.ui \
    ui/tabvirusquarantine.ui \
    ui/tabscanscheduling.ui \
    ui/tababoutclamav.ui
    
OTHER_FILES += ChangeLog \
    INSTALL \
    README.md \
    NEWS \
    TODO \
    my_rpm \
    AUTHORS \
    clamui.desktop \
    clamui.spec \
    translations/translation_info.txt \
    scripts/Virusscan.desktop
    
RESOURCES += clamui.qrc

TRANSLATIONS += translations/clamui_de.ts \
    translations/clamui_en.ts 
    
DISTFILES += clamui.desktop \
    translations/clamui_de.qm \
    translations/clamui_en.qm \
    AUTHORS \
    ChangeLog \
    LICENSE \
    INSTALL \
    NEWS \
    README.md \
    TODO \
    clamui.spec \
    translations/translation_info.txt \
    scripts/Virusscan.desktop
