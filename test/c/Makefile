CC=gcc
CFLAGS=-g -Wall -std=c99 -I../..
CFILES=test-c-code.c ../../currency_data.c

test-c-code: $(CFILES) $(HFILES) $(wildcard ../*.hpp)
	gcc -o test-c-code $(CFILES) $(CFLAGS)

.PHONY : clean

clean :
	rm test-c-code

