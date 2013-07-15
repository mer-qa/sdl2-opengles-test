
TARGETS := sdl2_opengles1_test sdl2_opengles2_test
DESKTOPS := $(patsubst %,%.desktop,$(TARGETS))

DESTDIR ?=
PREFIX ?= /usr

all: $(TARGETS)

sdl2_opengles1_test: main_glesv1.cpp common.cpp
	$(CXX) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv1_cm)

sdl2_opengles2_test: main_glesv2.cpp common.cpp
	$(CXX) -o $@ $^ $(shell pkg-config --libs --cflags sdl2 glesv2)

install: $(TARGETS) $(DESKTOPS)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/share/applications/
	install -m755 $(TARGETS) $(DESTDIR)$(PREFIX)/bin/
	install -m644 $(DESKTOPS) $(DESTDIR)$(PREFIX)/share/applications/

clean:
	rm -f $(TARGETS)

.PHONY: all install clean

