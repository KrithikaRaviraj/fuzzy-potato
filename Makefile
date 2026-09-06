# Interactive 3D Solar System and Space Exploration Simulator
# Makefile for GCC (MSYS2 UCRT64 / MinGW-w64)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lfreeglut -lopengl32 -lglu32

SRC = src/main.c
TARGET = bin/solar_sim.exe

all: $(TARGET)

$(TARGET): $(SRC)
	@if not exist bin mkdir bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	@if exist bin\solar_sim.exe del /q bin\solar_sim.exe

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
