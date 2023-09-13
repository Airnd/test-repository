TARGET=test
CC=gcc
CFLAGS=-Wall
MEM=-fsanitize=address

SRC=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRC))

$(TARGET):$(OBJS) makefile
	$(CC)  $(CFLAGS) $(OBJS) -o $@

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJS)