CC=gcc

CFLAGS= -g -std=c99 -Werror -Wall

IPATH  = -I. 

LIBS=

FILES  = server.c

all:
	$(CC) $(CFLAGS) $(IPATH) $(FILES) $(LIBS) -o led_char_server
	@echo LED Server Build Complete!

.PHONY: clean

clean:
	@rm -vf led_char_server
	@echo Server Executable Removed!
