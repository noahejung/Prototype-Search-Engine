# Makefile for 'querier' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Noah Jung February 2024

OBJS = querier.o 
LIBS = -L../common -l:common.a -L../libcs50 -lcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make

TARGET = querier 

# Default target
all: $(TARGET)

querier: querier.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)



test: $(TARGET) testing.sh
	bash -v testing.sh >& testing.out 

querier.o: querier.c 


.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core 
	rm querier
