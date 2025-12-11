# ---- Paths ----
PROJECT_NAME = mygame
SRC_DIR = src
BUILD_DIR = build
RAYLIB_DIR = libs/raylib
RAYLIB_SRC = $(RAYLIB_DIR)/src
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

# ---- Sources ----
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# ---- Rules ----
all: $(BUILD_DIR)/$(PROJECT_NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build raylib first
$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

# Build objects
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link final executable
$(BUILD_DIR)/$(PROJECT_NAME): $(RAYLIB_LIB) $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@

# Cleanup
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

