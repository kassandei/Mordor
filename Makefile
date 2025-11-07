CC = gcc
CFLAGS = -Iinclude
SRC_DIR = src

# Legge tutti i file .c all'interno della cartella
SRCS = $(wildcard $(SRC_DIR)/*.c)
TARGET = game

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) 
	./$(TARGET)

