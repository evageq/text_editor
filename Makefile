SRCS = $(wildcard *.c)

BEAR=bear --
CC=clang

FILE=tmp
ARGS=$(FILE)

PROGRAM=a.out

all:
	$(BEAR) $(CC) -g $(SRCS) -lncurses -o $(PROGRAM)

run:
	./$(PROGRAM) $(ARGS)

preprocess:
	$(CC) -E render.c -o render.i
