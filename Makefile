CC = gcc
CFLAGS = -Iinclude
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
TARGET = game

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
