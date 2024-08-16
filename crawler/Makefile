# Makefile for 'crawler' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

OBJS = crawler.o 
LIBS = -L../common -l:common.a -L../libcs50 -lcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make

TARGET = crawler

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)


test: $(TARGET) testing.sh
	bash -v testing.sh >& testing.out

crawler.o: crawler.c

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f counterstest
	rm -f core 
