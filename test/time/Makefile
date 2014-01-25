CC=g++
CFLAGS=-O3 -I../.. -g
#CFLAGS=-O0 -I../.. -g
CFILES=time-isomon.cpp ../../currency_data.c

time-isomon: $(CFILES) $(wildcard ../../*.hpp)
	$(CC) -o time-isomon $(CFILES) $(CFLAGS)

.PHONEY: clean

clean:
	rm time-isomon

