#
# spec file for package python-$FOO
#
# Copyright (c) $CURRENT_YEAR $YOUR_NAME_WITH_MAIL_ADDRESS
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           coquillo
Version:        2.0.1
Release:        1
License:        LGPL-3.0-only
Summary:        Utility for editing metadata contained in audio files
Url:            http://qt-apps.org/content/show.php/Coquillo?content=141896
Group:          Productivity/Multimedia/Sound/Utilities
Source0:        %{name}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}

BuildRequires:  taglib-devel >= 1.6
BuildRequires:  libmusicbrainz5-devel

%if %{defined fedora_version}
BuildRequires:  qt5-devel
BuildRequires:  gcc-c++
Requires:       libmusicbrainz5
%else
BuildRequires:  libqt5-qttools-devel >= 5.4
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(Qt5Concurrent) >= 5.4
BuildRequires:  pkgconfig(Qt5Core) >= 5.4
BuildRequires:  pkgconfig(Qt5Multimedia) >= 5.4
BuildRequires:  pkgconfig(Qt5Network) >= 5.4
BuildRequires:  pkgconfig(Qt5Widgets) >= 5.4
%endif

%if 0%{?suse_version}
BuildRequires:  update-desktop-files
%endif

Requires:       taglib >= 1.6

%{!?qmake5:%define qmake5 qmake-qt5}

%description
Coquillo is a utility with which one can edit audio file metadata, so-called tags.
It is based on TagLib and tries to support the fileformats TagLib does. Most
important file formats supported are MP3, Ogg/Vorbis and FLAC.

# Fixes error "Empty %%files file /home/abuild/rpmbuild/BUILD/coquillo-2.0.0/debugsourcefiles.list" on Fedora
%global debug_package %{nil}

%prep
%setup -q

%build
%qmake5
make %{?_smp_mflags}

%install
make INSTALL_ROOT=$RPM_BUILD_ROOT install

%if %{defined suse_version}
%suse_update_desktop_file coquillo
%endif

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root, -)
# %%doc CHANGES README
%{_bindir}/coquillo
%{_datadir}/applications/coquillo.desktop
%{_datadir}/pixmaps/coquillo.png

%changelog
* Sat Jul 28 2018 Samu Juvonen <samu.juvonen@gmail.com> - 2.0.1
- Fixed issue with renaming files when "safe filenames" setting is enabled.
* Tue Apr 24 2018 Samu Juvonen <samu.juvonen@gmail.com> - 2.0.0
- Rewritten for Qt 5.
- Improved user interface.
