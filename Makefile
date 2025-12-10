SRCS = $(wildcard *.c)

BEAR=bear --
CC=clang

all:
	$(BEAR) $(CC) -g $(SRCS) -lncurses

preprocess:
	$(CC) -E render.c -o render.i
