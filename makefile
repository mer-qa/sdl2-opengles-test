
TARGETS := sdl2_opengles1_test sdl2_opengles2_test

CFLAGS += -I../SDL/include -I../SDL/build/include
LDLIBS += -lSDL2 -L../SDL/build/build/.libs

all: prepare $(TARGETS)

sdl2_opengles1_test: main_glesv1.cpp common.cpp
	$(CXX) -o $@ $(CFLAGS) $^ $(LDLIBS) -lGLESv1_CM

sdl2_opengles2_test: main_glesv2.cpp common.cpp
	$(CXX) -o $@ $(CFLAGS) $^ $(LDLIBS) -lGLESv2

prepare:
	$(MAKE) -C ../SDL/build

clean:
	rm -f $(TARGETS)

.PHONY: all prepare clean

