CC=gcc

CFLAGS= -g -std=c99 -Werror -Wall

IPATH  = -I. 

LIBS=

FILES  = client.c

all:
	$(CC) $(CFLAGS) $(IPATH) $(FILES) $(LIBS) -o client
	@echo LED Client Build Complete!

.PHONY: clean

clean:
	@rm -vf client
	@echo Client Executable Removed!
