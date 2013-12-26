Name:		sdl2-opengles-test
Version:	1.0.6
Release:	1
Summary:	OpenGL ES 1.1 and 2.0 test applications (using SDL2)

Group:		System/GUI/Other
License:	MIT
URL:		https://github.com/thp/sdl2-opengles-test
Source0:	%{name}-%{version}.tar.bz2
BuildRequires:	pkgconfig(sdl2)
BuildRequires:	pkgconfig(egl)
BuildRequires:  pkgconfig(glesv1_cm)
BuildRequires:  pkgconfig(glesv2)
BuildRequires:  pkgconfig(SDL2_image)
BuildRequires:  pkgconfig(SDL2_ttf)
BuildRequires:  pkgconfig(SDL2_mixer)

%description
This application is used to test OpenGL ES 1.1 and 2.0 rendering
using SDL 2.0 under Wayland. Also tested: multi-touch input.

%prep
%setup -q

%build
make

%install
%make_install

%files
%defattr(-,root,root,-)
/usr/bin/*
/usr/share/applications/*.desktop
/usr/share/%(name)/*
