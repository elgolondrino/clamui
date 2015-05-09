%define name    clamui
%define version 0.4.0
#%define release 0

Name: %{name}
#Provides: %{name}
Summary: An UI for ClamAV
Version: %{version}
Release:
License: GPL v3
Icon: %{name}.xpm
Group: Productivity/Security/Tools
BuildRoot: %{_tmppath}/build-root-%{name}-%{version}-%{release}
Source0: %{name}-%{version}.tar.gz
#Packager: Georg Lassnig
Distribution: OpenSuSE
Url: http://www.e67-its.de/
Vendor: Georg Lassnig
Requires: libc.so.6 libc.so.6(GLIBC_2.0) libc.so.6(GLIBC_2.1.3) libgcc_s.so.1 libgcc_s.so.1(GCC_3.0) libm.so.6 libpthread.so.0 libpthread.so.0(GLIBC_2.0) libQtCore.so.5 libQtWidget.so.5 libstdc++.so.6 libstdc++.so.6(CXXABI_1.3) libstdc++.so.6(GLIBCXX_3.4) 

%description
ClamUI is an UI for the Clam Anti Virus (ClamAV) scanner.

%prep
rm -rf %{buildroot}
mkdir %{buildroot}

%setup -q

%build
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/%{name}
mkdir -p %{buildroot}%{_datadir}/locale/de/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/locale/en/LC_MESSAGES
mkdir -p %{buildroot}%{_datadir}/applications
mkdir -p %{buildroot}%{_datadir}/icons/breeze/apps/software

export PATH=/usr/lib64/qt5/bin:$PATH
export QTDIR=%{_prefix}/lib64/qt5/

/usr/bin/lupdate-qt5 %{name}.pro
/usr/bin/lrelease-qt5 %{name}.pro
qmake  %{name}.pro
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" \
make

%install
cp %{name} %{buildroot}%{_bindir}
cp translations/%{name}_de.qm %{buildroot}%{_datadir}/locale/de/LC_MESSAGES
cp translations/%{name}_en.qm %{buildroot}%{_datadir}/locale/en/LC_MESSAGES
cp icons/%{name}.svg %{buildroot}%{_datadir}/icons/breeze/apps/software/

cat > $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop << EOF[Desktop Entry]
GenericName=ClamUI
GenericName[de]=ClamUI
GenericName[en]=ClamUI
Name=ClamUI
Name[de]=ClamUI
Name[en]=ClamUI
Categories=Qt;KDE;Utility;Security;Tools;
Comment=An UI for ClamAV
Comment[de]= Eine grafische Oberfläche für ClamAV
Comment[en]=An UI for ClamAV
Encoding=UTF-8
Exec=clamui -caption "%c" %u
Icon=clamui
StartupNotify=true
Terminal=false
Type=Application
Version=1.0
EOF

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%lang(de) %{_datadir}/locale/de/LC_MESSAGES/%{name}_de.qm
%lang(en) %{_datadir}/locale/en/LC_MESSAGES/%{name}_en.qm
%{_datadir}/icons/breeze/apps/software/%{name}.svg
%doc AUTHORS COPYING ChangeLog NEWS README.md TODO

%changelog

* Tue Jun 30 2009 Georg Lassnig 0.2.0
- Initial release
