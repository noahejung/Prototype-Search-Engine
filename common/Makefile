# Makefile for 'counters' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

OBJS = pagedir.o index.o word.o
LIBS = L../libcs50 -lcs50
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o *.a
	rm -f core
