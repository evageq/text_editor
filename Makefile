export TOP_DIR=$(shell pwd)
export BUILD_DIR=$(TOP_DIR)/build

LIB_DIR := $(TOP_DIR)/lib
SRC_DIR := $(TOP_DIR)/src

PVS_CFG := $(TOP_DIR)/PVS-Studio.cfg
# csv, errorfile, fullhtml, html, tasklist, xml
LOG_FORMAT := html
PVS_LOG := $(BUILD_DIR)/project.tasks
PVS_LOG_OUT := $(BUILD_DIR)/pvs.log

INCDIRS := . $(LIB_DIR)
SRCDIRS=$(SRC_DIR) $(LIB_DIR)

SRC := $(wildcard $(addsuffix /*.c,$(SRCDIRS)))
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC)))
DEPS := $(patsubst %.c,$(BUILD_DIR)/%.d,$(notdir $(SRC)))

BEAR_OPTS :=--output $(TOP_DIR)/compile_commands.json
BEAR := bear $(BEAR_OPTS) --

CC := clang

LDLIBS := -lncurses
LDFLAGS := -fuse-ld=mold

DEPFLAGS := -MP -MD
COMPILE_SANITY_OPTS := -Wall -Wextra
CPPFLAGS := $(foreach D,$(INCDIRS),-I$(D))
CFLAGS := -g $(DEPFLAGS)

FILE := $(TOP_DIR)/tmp
ARGS := $(FILE)

PROGRAM := $(TOP_DIR)/editor

vpath %.c $(SRCDIRS)

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $^ $(LOADLIBES) $(LDLIBS) -o $@
ifdef PVS
	pvs-studio-analyzer analyze -o $(PVS_LOG)
ifeq ($(LOG_FORMAT), html)
	plog-converter -a 'GA:1,2' -t $(LOG_FORMAT) $(POBJECTS) -o $(PVS_LOG_OUT).html $(PVS_LOG)
endif

endif

$(OBJ): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

ifeq ($(CPP), 1)
	$(CC) $(CPPFLAGS) $< -E -o $@.i
endif

ifdef PVS
	$(CC) $(CPPFLAGS) $< -E -o $@.PVS-Studio.i
endif

run:
	$(PROGRAM) $(ARGS)

clean:
	rm -f -- $(BUILD_DIR)/*

bear: clean
	@$(BEAR) $(MAKE) -C .

.PHONY: all run clean bear

-include $(DEPS)
