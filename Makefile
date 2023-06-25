PROG = lzrw3a-decompress
CFLAGS ?= -O3 -fomit-frame-pointer -fvisibility=hidden -DNDEBUG -flto -Wall

SRCS = LZRW3.c LZRW3-A.c $(PROG).c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.dSYM $(PROG)
