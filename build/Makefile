SRC = $(wildcard $(TOP_DIR)/*.c)
OBJ = $(patsubst $(TOP_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

BEAR_OPTS=-o $(TOP_DIR)
BEAR=bear $(BEAR_OPTS) --
CC=clang

LDLIBS=-lncurses
LDFLAGS=-fuse-ld=mold

FILE=$(TOP_DIR)/tmp
ARGS=$(FILE)

PROGRAM=$(TOP_DIR)/text_editor

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(TOP_DIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

run:
	@$(PROGRAM) $(ARGS)

clean:
	rm -f -- $(OBJ) $(PROGRAM)

bear: clean
	@$(BEAR) $(MAKE) -C .

.PHONY: all run clean bear
