# ---- Paths ----
RAYGUI_SRC = libs/raygui/src
RAYLIB_SRC = libs/raylib/src
RAYLIB_LIB = $(RAYLIB_SRC)/libraylib.a

# ---- Compiler ----
CC = gcc
CFLAGS = -std=c99 -Wall -I$(RAYLIB_SRC) -I$(RAYGUI_SRC)
LDFLAGS = $(RAYLIB_LIB)

# ---- Platform-specific flags ----
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    LDFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(UNAME_S), Darwin)  # macOS
    LDFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lm
endif

# compile
main test: $(RAYLIB_LIB) build
	$(CC) src/$@.c -o build/$@.out $(CFLAGS) $(LDFLAGS)

# build raylib
$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

build:
	mkdir -p build

# Cleanup
clean:
	rm -rf build

.PHONY: main test clean
