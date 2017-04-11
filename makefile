SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
CFLAGS = -g -Wall
TARGET = minesweeper
CC = gcc

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(OUTPUT_OPTION) $(CFLAGS) $^

clean:
	$(RM) $(OBJS) $(TARGET)

