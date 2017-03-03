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
Version:        1.12
Release:        2
License:        GPLv2
Summary:        Audio Metadata Editor
Url:            http://qt-apps.org/content/show.php/Coquillo?content=141896
Group:          Productivity/Multimedia/Sound/Utilities
Source0:        %{name}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}
BuildRequires:  taglib-devel >= 1.6

%if %{defined fedora_version}
BuildRequires:  qt-devel
BuildRequires:  gcc-c++
%else
BuildRequires:  libqt4-devel
%endif

%if 0%{?suse_version}
BuildRequires:  update-desktop-files
%endif

Requires:       taglib >= 1.6

%description
Coquillo is a utility with which one can edit audio file metadata, so-called tags.
It is based on TagLib and tries to support the fileformats TagLib does. Most 
important file formats supported are MP3, Ogg/Vorbis and FLAC.

%prep
%setup -q

%build
%if %{defined fedora_version}
  qmake-qt4
%else
  qmake
%endif

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
%doc CHANGES README
%{_bindir}/coquillo
%{_datadir}/applications/coquillo.desktop
%{_datadir}/pixmaps/coquillo.png

%changelog
