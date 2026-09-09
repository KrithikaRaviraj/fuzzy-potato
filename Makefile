# Interactive 3D Solar System and Space Exploration Simulator
# Makefile for GCC (MSYS2 UCRT64 / MinGW-w64)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lfreeglut -lopengl32 -lglu32

SRC = src/main.c src/camera.c
TARGET = bin/solar_sim.exe

all: $(TARGET)

$(TARGET): $(SRC)
		@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
		@rm -f bin/solar_sim.exe

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
