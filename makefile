
TARGET := sdl_opengles2_test
OBJECTS := main.o

CFLAGS += -I../SDL/include -I../SDL/build/include
LDLIBS += -lSDL2 -lGLESv2 -L../SDL/build/build/.libs

all: prepare $(TARGET)

prepare:
	$(MAKE) -C ../SDL/build

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $< $(LDLIBS)

run: test
	LD_LIBRARY_PATH=../build/.libs ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all prepare run clean
