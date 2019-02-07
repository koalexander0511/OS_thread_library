# Target library
lib := libuthread.a
objs := queue.o uthread.o preempt.o context.o
CC := gcc
CFLAGS := -Wall -Werror


all: $(lib)

libuthread.a: $(objs)
	ar rcs libuthread.a $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(lib) $(objs)