export TOP_DIR=$(shell pwd)
export BUILD_DIR=$(TOP_DIR)/build

PVS_CFG=./PVS-Studio.cfg
# csv, errorfile, fullhtml, html, tasklist, xml
LOG_FORMAT=html
PVS_LOG=./project.tasks
PVS_LOG_OUT=./pvs.log

INCDIRS=.

SRC = $(wildcard $(TOP_DIR)/*.c)
OBJ = $(patsubst $(TOP_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
DEPS = $(patsubst $(TOP_DIR)/%.c,$(BUILD_DIR)/%.d,$(SRC))

BEAR_OPTS=--output $(TOP_DIR)
BEAR=bear $(BEAR_OPTS) --

CC=clang

LDLIBS=-lncurses
LDFLAGS=-fuse-ld=mold

DEPFLAGS=-MP -MD
COMPILE_SANITY_OPTS=-Wall -Wextra
CPPFLAGS=$(foreach D,$(INCDIRS),-I$(D))
CFLAGS=-g $(DEPFLAGS)

FILE=$(TOP_DIR)/tmp
ARGS=$(FILE)

PROGRAM=$(TOP_DIR)/text_editor

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $^ $(LOADLIBES) $(LDLIBS) -o $@
ifdef PVS
	pvs-studio-analyzer analyze -o $(PVS_LOG)
ifeq ($(LOG_FORMAT), html)
	plog-converter -a 'GA:1,2' -t $(LOG_FORMAT) $(POBJECTS) -o $(PVS_LOG_OUT).html $(PVS_LOG)
endif

endif

$(BUILD_DIR)/%.o: $(TOP_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

ifdef CPP
	$(CC) $(CPPFLAGS) $< -E -o $@.i
endif

ifdef PVS
	$(CC) $(CPPFLAGS) $< -E -o $@.PVS-Studio.i
endif

run:
	@$(PROGRAM) $(ARGS)

clean:
	rm -f -- $(OBJ) $(PROGRAM)
ifdef PVS
	rm -f -- $(BUILD_DIR)/*.PVS-Studio* $(DEPS) $(PVS_LOG) $(PVS_LOG_OUT).*
endif

bear: clean
	@$(BEAR) $(MAKE) -C .

.PHONY: all run clean bear

-include $(DEPS)
