lessThan(QT_VERSION, 5.3) {
    error("Program requires at least Qt 5.3!")
}

QT += sql
QT += network
QT += widgets
#QT += KIconThemes
QT += Solid
QT += KNotifications
QT += KDESu
INCLUDEPATH += /usr/lib64

TEMPLATE = app
VERSION = -0.0.0
LANGUAGE += C++
CODECFORTR += UTF-8
UI_DIR += src
MOC_DIR += moc
OBJECTS_DIR += obj
target.path += /usr/bin
TARGET = clamui

CONFIG += qt
DEPENDPATH += . \
    gui \
    src \
    doc \
    icaons \
    ui \
    translations

HEADERS += src/definitionen.h \
    src/about.h \
    src/clamui.h \
    src/settings.h \
    src/tabvirusscan.h \
    src/tabvirusdb.h \
    src/tabvirusquarantine.h \
    src/tabscanscheduling.h \
    src/tababoutclamav.h \
    src/languagetools.h \
    src/sqlite_db.h \
    src/firstrunwizard.h \
    src/clamscanprocess.h \
    src/clamdprocess.h \
    src/freshclamprocess.h \
    src/clamdconf.h \
    src/freshclamconf.h \
    src/clamavmilterconf.h \
    src/showlogfile.h \
    src/clamdscanprocess.h

SOURCES += src/about.cpp \
    src/main.cpp \
    src/clamui.cpp \
    src/settings.cpp \
    src/tabvirusscan.cpp \
    src/tabvirusdb.cpp \
    src/tabvirusquarantine.cpp \
    src/tabscanscheduling.cpp \
    src/tababoutclamav.cpp \
    src/languagetools.cpp \
    src/sqlite_db.cpp \
    src/firstrunwizard.cpp \
    src/clamscanprocess.cpp \
    src/clamdprocess.cpp \
    src/freshclamprocess.cpp \
    src/clamdconf.cpp \
    src/freshclamconf.cpp \
    src/clamavmilterconf.cpp \
    src/showlogfile.cpp \
    src/clamdscanprocess.cpp

FORMS += ui/about.ui \
    ui/clamui.ui \
    ui/settings.ui \
    ui/tabvirusscan.ui \
    ui/tabvirusdb.ui \
    ui/tabvirusquarantine.ui \
    ui/tabscanscheduling.ui \
    ui/tababoutclamav.ui \
    ui/firstrunwizard.ui \
    ui/showlogfile.ui

OTHER_FILES += ChangeLog \
    INSTALL \
    README.md \
    NEWS \
    TODO \
    AUTHORS \
    clamui.desktop \
    clamui.spec \
    translations/translation_info.txt \
    tpl/* \
    scripts/VirusScan.desktop \
    scripts/VirusScan.sh

RESOURCES += clamui.qrc

TRANSLATIONS += translations/clamui_de_DE.ts \
    translations/clamui_en_GB.ts

DISTFILES += clamui.desktop \
    translations/clamui_de_DE.qm \
    translations/clamui_en_GB.qm \
    AUTHORS \
    ChangeLog \
    LICENSE \
    INSTALL \
    NEWS \
    README.md \
    TODO \
    clamui.spec \
    translations/translation_info.txt \
    scripts/VirusScan.desktop \
    scripts/VirusScan.sh

# #### Programmicons
software.path += /usr/share/icons/breeze/apps/software/
software.files += icons/breeze/apps/software/clamav.svg \
    icons/breeze/apps/software/clamui.svg

toolbar.path += /usr/share/icons/breeze/actions/toolbar/
toolbar.files += icons/breeze/actions/toolbar/web-browser.svg

panel.path += /usr/share/icons/breeze/status/panel/
panel.files += icons/breeze/status/panel/clamui-idle.svg \
    icons/breeze/status/panel/clamui-no-virus-found.svg \
    icons/breeze/status/panel/clamui-virus-found.svg

flags.path += /usr/share/$(TARGET)/flags
flags.files += icons/flags/*

# ### Solid Actions
solid.path += /usr/share/solid/actions/
solid.files += scripts/VirusScan.desktop \
    scripts/VirusScan.sh

# #### Desktop-Eintrag
menu.path += /usr/share/applications/
menu.files += src/clamui.desktop

#### Übersetzungen
translation.path += /usr/share/$(TARGET)/translations
translation.files += translations/*

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
    TODO \
    tpl*
    
INSTALLS += target \
    documentation \
    menu \
#	helpDE \
#	helpEN \
    translation \
    software \
    toolbar \
    panel \
    solid \
    flags
