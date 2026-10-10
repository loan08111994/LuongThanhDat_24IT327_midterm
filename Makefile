CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700
OBJS = main.o options.o fileinfo.o sort.o display.o util.o
TARGET = myls

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
