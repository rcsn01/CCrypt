
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

SRCS = main.c ui.c encryption.c library.c utils.c
TARGET = ccrypt

.PHONY: all build clean

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -f $(TARGET) *.o

