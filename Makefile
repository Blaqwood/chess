# ---- Paths ----
SRC = src/main.c
BUILD = chess.out
RAYLIB_SRC = libs/raylib/src
RAYLIB_LIB = $(RAYLIB_SRC)/libraylib.a

# ---- Compiler ----
CC = gcc
CFLAGS = -std=c99 -Wall -I$(RAYLIB_SRC)
LDFLAGS = $(RAYLIB_LIB)

# ---- Platform-specific flags ----
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    LDFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
endif

ifeq ($(UNAME_S), Darwin)  # macOS
    LDFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lm
endif

# ---- Rules ----
all: $(BUILD)

# Link final executable
$(BUILD): $(RAYLIB_LIB)
	$(CC) $(SRC) $(LDFLAGS) -I $(RAYLIB_SRC) -o $(BUILD)

# Build raylib first
$(RAYLIB_SRC)/libraylib.a:
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

# Cleanup
clean:
	rm $(BUILD)

.PHONY: all clean
