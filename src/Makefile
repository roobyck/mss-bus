
CC = gcc
CFLAGS = -O2 -Wall -Wextra -I$(INCLUDE)

.PHONY : all clean

all: sread swrite

sread: sread.o libser.o
	$(CC) -s -o $@ sread.o libser.o

swrite: swrite.o libser.o
	$(CC) -s -o $@ swrite.o libser.o

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f {sread,swrite,libser}.o

