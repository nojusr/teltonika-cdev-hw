CFLAGS:=-Wall -Wextra -g
LDFLAGS:=-lconfig

PREFIX:=/usr/local

OUTPUTDIR=./bin/

SRC:=$(wildcard src/*.c)
OBJ:=$(SRC:.c=.o)
BIN:=$(OUTPUTDIR)tcdh


.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p $(OUTPUTDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	$(RM) $(BIN) $(OBJ)
