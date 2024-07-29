
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = fs
SRCS = filesystem.c

OBJS = $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)


.PHONY: all clean
