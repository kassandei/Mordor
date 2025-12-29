CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude
SRC_DIR = src

# Legge tutti i file .c all'interno della cartella
SRCS = $(wildcard $(SRC_DIR)/*.c)
TARGET = game

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	./$(TARGET)

compile: 
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
