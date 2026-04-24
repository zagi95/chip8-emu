CC = gcc
CFLAGS_RELEASE = -Wall -Wextra -std=c99 -O2
CFLAGS_DEBUG   = -Wall -Wextra -std=c99 -g -O0 -DDEBUG
LDLIBS = -lm -lSDL3

SOURCES = main.c memory.c framebuffer.c cpu.c display.c
TARGET_RELEASE = chip8_emu
TARGET_DEBUG   = chip8_emu_debug

all: $(TARGET_RELEASE)

release: $(TARGET_RELEASE)

debug: $(TARGET_DEBUG)

$(TARGET_RELEASE): $(SOURCES)
	$(CC) $(CFLAGS_RELEASE) -o $@ $^ $(LDLIBS)

$(TARGET_DEBUG): $(SOURCES)
	$(CC) $(CFLAGS_DEBUG) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET_RELEASE) $(TARGET_DEBUG)

run: $(TARGET_RELEASE)
	./$(TARGET_RELEASE) $(ROM)

run-debug: $(TARGET_DEBUG)
	./$(TARGET_DEBUG) $(ROM)

.PHONY: all release debug clean run run-debug
