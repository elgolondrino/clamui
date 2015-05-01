%define name    mysqloader
%define version 0.4.0
#%define release 0

Name: %{name}
#Provides: %{name}
Summary: A small tool for MySQL
Version: %{version}
Release:
License: GPL v3
Icon: %{name}.xpm
Group: Productivity/Databases/Tools
BuildRoot: %{_tmppath}/build-root-%{name}-%{version}-%{release}
Source0: %{name}-%{version}.tar.gz
#Packager: Georg Lassnig
Distribution: OpenSuSE
Url: http://www.e67-its.de/
Vendor: Georg Lassnig
Requires: libc.so.6 libc.so.6(GLIBC_2.0) libc.so.6(GLIBC_2.1.3) libgcc_s.so.1 libgcc_s.so.1(GCC_3.0) libm.so.6 libpthread.so.0 libpthread.so.0(GLIBC_2.0) libQtCore.so.5 libQtWidget.so.5 libstdc++.so.6 libstdc++.so.6(CXXABI_1.3) libstdc++.so.6(GLIBCXX_3.4) 

%description
MySQLoader is a small KDE4 application for the system tray. You can start/stop the MySQL server and launch the MySQL tools like Query Browser, Workbench and MySQL Administrator

%prep
rm -rf %{buildroot}
mkdir %{buildroot}

%setup -q

%build
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/%{name}
mkdir -p %{buildroot}%{_datadir}/locale/de/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/es/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/it/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/en/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/fr/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/nl/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/pt/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/applications/kde4/
mkdir -p %{buildroot}%{_datadir}/pixmaps

#export PATH=/usr/lib64/qt5/bin:$PATH
#export QTDIR=%{_prefix}/lib64/qt5/

#/usr/bin/lupdate %{name}.pro
#/usr/bin/lrelease %{name}.pro
qmake  %{name}.pro
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" \
make

%install
cp %{name} %{buildroot}%{_bindir}
cp translations/%{name}_de.qm %{buildroot}%{_datadir}/locale/de/LC_MESSAGES
cp translations/%{name}_fr.qm %{buildroot}%{_datadir}/locale/fr/LC_MESSAGES
cp translations/%{name}_en.qm %{buildroot}%{_datadir}/locale/en/LC_MESSAGES
cp translations/%{name}_es.qm %{buildroot}%{_datadir}/locale/es/LC_MESSAGES
cp translations/%{name}_nl.qm %{buildroot}%{_datadir}/locale/nl/LC_MESSAGES
cp translations/%{name}_pt.qm %{buildroot}%{_datadir}/locale/pt/LC_MESSAGES
cp translations/%{name}_it.qm %{buildroot}%{_datadir}/locale/it/LC_MESSAGES
cp images/48x48/%{name}_off.png %{buildroot}%{_datadir}/pixmaps/
cp images/48x48/%{name}_on.png %{buildroot}%{_datadir}/pixmaps/
cp images/48x48/%{name}.png %{buildroot}%{_datadir}/pixmaps/

cat > $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop << EOF
[Desktop Entry]
Categories=Qt;KDE;Office;Database;
Comment=MySQLoader is a small application for the systray to start and stop the MySQL-Server and launch the MySQL-Tools
Comment[de]=MySQLoader ist ein kleines Tool um den MySQL-Server zu starten und stoppen
Comment[nl]=MySQLoader is een kleine tol op de MySQL-server te starten en te stoppen
Comment[pt]=MySQLoader é uma pequena aplicação para a área de notificação pra iniciar e parar o Servidor MySQL e executar suas ferramentas
Comment[it]=MySQLoader è un piccolo strumento al server MySQL per avviare e arrestare
Comment[es]=MySQLoader es una pequeña herramienta para el servidor MySQL para arrancar y parar
Comment[en]=MySQLoader is a small application for the systray to start and stop the MySQL-Server and launch the MySQL-Tools
Encoding=UTF-8
Exec=%{name} -caption "%c" %u
GenericName=MySQLoader
GenericName[de]=MySQLoader
GenericName[nl]=MySQLoader
GenericName[pt]=MySQLoader
GenericName[it]=MySQLoader
GenericName[es]=MySQLoader
GenericName[en]=MySQLoader
GenericName[fr]=MySQLoader
Icon=%{name}
MimeType=
Name=MySQLoader
Name[de]=MySQLoader
Name[nl]=MySQLoader
Name[pt]=MySQLoader
Name[it]=MySQLoader
Name[es]=MySQLoader
Name[en]=MySQLoader
Name[f]=MySQLoader
StartupNotify=true
Terminal=false
Type=Application
Version=1.0
X-SuSE-translate=true
EOF

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%lang(de) %{_datadir}/locale/de/LC_MESSAGES/%{name}_de.qm
%lang(en) %{_datadir}/locale/fr/LC_MESSAGES/%{name}_fr.qm
%lang(en) %{_datadir}/locale/en/LC_MESSAGES/%{name}_en.qm
%lang(es) %{_datadir}/locale/es/LC_MESSAGES/%{name}_es.qm
%lang(nl) %{_datadir}/locale/nl/LC_MESSAGES/%{name}_nl.qm
%lang(pt) %{_datadir}/locale/pt/LC_MESSAGES/%{name}_pt.qm
%lang(it) %{_datadir}/locale/it/LC_MESSAGES/%{name}_it.qm
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/pixmaps/%{name}_off.png
%{_datadir}/pixmaps/%{name}_on.png
%doc AUTHORS COPYING ChangeLog NEWS README TODO

%changelog
* Wed Oct 22 2014 Georg Lassnig 0.4
- REMOVED: KDE dependencies.
- ADDED: Qt5 support

* Thu Jun 30 2011 Georg Lassnig 0.3.6.1
- FIXED: The program crashed reproducibly after a few minutes.

* Thu Jun 30 2011 Georg Lassnig 0.3.6
- CHANGED: The handling of the system tray icon is now KDE compliant.
- UPDATED: Translations: dutch, english, french, german, italian, portugues (BR/PT), spain.

* Wed Mar 09 2011 Georg Lassnig 0.3.5
- UPDATED: French translation.
- CHANGED: SysTray menu. In the new versions of MySQL MySQL-Administrator and MySQL-Query is no longer available. These applications have been integrated into the MySQL-Workbench.

* Tue Dec 22 2010 Georg Lassnig 0.3.4
- ADDED: French translation. Thanks to Henri Girard.
- ADDED: A new pixmap.

* Thu Aug 05 2010 Georg Lassnig 0.3.3
- CHANGED: Some functions for application checks.
- ADDED: A pushbutton to open the settings in the missed apps dialog.
- UPDATED: Language files.

* Sat Jul 17 2010 Georg Lassnig 0.3.2
- ADDED: New App-Icons.
- ADDED: The german and english handbook. Other languages will follow as soon as possible.
- CHANGED: The Menu-Structure.

* Fri Jun 25 2010 Georg Lassnig 0.3.1.1
- FIXED: A bug in the in the manual language selection.

* Sun Jun 20 2010 Georg Lassnig 0.3.1
- CHANGED: Some functions and little changes in ui-style.
- UPDATED: Language files.

* Wed May 26 2010 Georg Lassnig 0.3.0
- ADDED: KUniqueApplication. This prevents that a second instance of the program can be started.
- CHANGED: Some functions and little changes in ui-style.
- UPDATED: Language files.

* Thu May 06 2010 Georg Lassnig 0.2.9.1
- FIXED: Lost button in the settings window. It is gone lost like a sock in a washing machine. ;o)
- FIXED: Broken spanish langusge file.
- NEW: Online Update-Check for the Application.
- UPDATED: Language files.

* Tue May 04 2010 Georg Lassnig 0.2.9
- ADDED: MySQLoader now supports KDE-GUI-style settings for a Better integration to KDE.
- NEW: Italian and Spain Translation files added.
- CHANGED: The classes and widgets to be changed step by step from Qt to KDE

*Thu Apr 22 2010 Georg Lassnig 0.2.8
- ADDED: Closer KDE4-Integration.
- ADDED: A .spec file to build rpm packages.
- FIXED: Various small bugfixes

* Sun Dec 13 2009 Georg Lassnig 0.2.7
- ADDED: SysTray messages about the MySQL-Server status, if start or stop of the server is not succesfull.
- CLEANED: Uggly code. I'm still learning C++! ;o)

* Mon Dec 07 2009 Georg Lassnig 0.2.6
- ADDED: A message window about missing applications.
- ADDED: Enable or disable application messages.
- CHANGED: GUI-Design.

* Sat Nov 21 2009 Georg Lassnig 0.2.5
- CHANGED: only one instance can run at same time.

* Tue Nov 17 2009 Georg Lassnig 0.2.4
- ADDED: Brasilian portuguese translation. Thank's to Márcio Moraes.

* Sun Nov 15 2009 Georg Lassnig 0.2.3
- ADDED: It is now possible to change the path for the PID-File in the settings window.
- ADDED: A timer event to check the server state.
- FIXED: Some stupid bugs.

* Tue Jul 8 2009 Georg Lassnig 0.2.2
- ADDED: It is now possible to change the language in the settings window.

* Mon Jul 6 2009 Georg Lassnig 0.2.1
- ADDED: Dutch translation.
- CORRECTED: English Translation.
- FIXED: Some small bugs fixed.

* Tue Jun 30 2009 Georg Lassnig 0.2.0
- Initial release
