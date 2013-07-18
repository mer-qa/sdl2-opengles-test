
PACKAGE := sdl2-opengles-test

TARGETS := sdl2_opengles1_test sdl2_opengles2_test sdl2_image_test sdl2_ttf_test sdl2_mixer_test
DESKTOPS := $(patsubst %,%.desktop,$(TARGETS))
DATA_FILES := $(wildcard images/* fonts/* sounds/*)

DESTDIR ?=
PREFIX ?= /usr

DATADIR := $(PREFIX)/share/$(PACKAGE)/

CXXFLAGS ?= -g
CXXFLAGS += -DDATADIR=\"$(DATADIR)\"

all: $(TARGETS)

sdl2_opengles1_test: main_glesv1.cpp common.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv1_cm)

sdl2_opengles2_test: main_glesv2.cpp common.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv2)

sdl2_image_test: main_image.cpp common.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv1_cm SDL2_image)

sdl2_ttf_test: main_ttf.cpp common.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv1_cm SDL2_ttf)

sdl2_mixer_test: main_mixer.cpp common.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv1_cm SDL2_mixer)

install: $(TARGETS) $(DESKTOPS)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/share/applications/
	install -d $(DESTDIR)$(DATADIR)
	install -m755 $(TARGETS) $(DESTDIR)$(PREFIX)/bin/
	install -m644 $(DESKTOPS) $(DESTDIR)$(PREFIX)/share/applications/
	install -m644 $(DATA_FILES) $(DESTDIR)$(DATADIR)

clean:
	rm -f $(TARGETS)

.PHONY: all install clean

