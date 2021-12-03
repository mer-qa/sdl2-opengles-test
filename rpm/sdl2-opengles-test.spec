Name:		sdl2-opengles-test
Version:	1.0.8
Release:	1
Summary:	OpenGL ES 1.1 and 2.0 test applications (using SDL2)
License:	MIT
URL:		https://github.com/mer-qa/sdl2-opengles-test
Source0:	%{name}-%{version}.tar.bz2
BuildRequires:	pkgconfig(sdl2)
BuildRequires:	pkgconfig(egl)
BuildRequires:  pkgconfig(glesv1_cm)
BuildRequires:  pkgconfig(glesv2)
BuildRequires:  pkgconfig(SDL2_image)
BuildRequires:  pkgconfig(SDL2_ttf)
BuildRequires:  pkgconfig(SDL2_mixer)
BuildRequires:  pkgconfig(audioresource)
BuildRequires:  pkgconfig(glib-2.0)

%description
This application is used to test OpenGL ES 1.1 and 2.0 rendering
using SDL 2.0 under Wayland. Also tested: multi-touch input.

%prep
%autosetup

%build
%make_build

%install
%make_install

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_datadir}/applications/*.desktop
%{_datadir}/sdl2_*_test
