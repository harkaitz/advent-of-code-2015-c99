.SUFFIXES:
.PHONY: all clean install check

PROJECT   =advent-of-code-2015-c99
VERSION   =1.0.0
CC        =$(shell which $(TPREFIX)cc $(TPREFIX)gcc 2>/dev/null | head -n 1)
CFLAGS    =-Wall -Wpedantic -g3 -std=c99
PREFIX    =/usr/local
BUILDDIR  ?=.build
UNAME_S   ?=$(shell uname -s)
EXE       ?=$(SUFFIX)$(shell uname -s | awk '/Windows/ || /MSYS/ || /CYG/ { print ".exe" }')
SOURCES    =$(shell find . -regex './[012][0-9].c' | sed '$(SED_CMD)')
PROGS      =$(patsubst ./%.c,$(BUILDDIR)/%$(EXE),$(SOURCES))
LIB_H      =$(shell find aoc -name '*.h')
LIB_C      =$(shell find aoc -name '*.c')
LIB_CRYPTO =-lcrypto

all: $(PROGS)
clean:
	rm -f $(PROGS)
install:
check:
	@wds_aoc all


$(BUILDDIR)/04$(EXE): 04.c $(LIB_H) $(LIB_C)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB_C) $(LIB_CRYPTO)

$(BUILDDIR)/%$(EXE): %.c $(LIB_H) $(LIB_C)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB_C)
