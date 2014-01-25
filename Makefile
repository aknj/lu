CC = gcc
IDIR = /home/ee/stud/alechnoa/gsl/local/include
CFLAGS = -g -Wall -pedantic -std=c99 -I$(IDIR)
LFLAGS = -L$(IDIR) -lgsl -lgslcblas -lm

solve: main.o
	$(CC) -o solve $^ $(LFLAGS)

main.o: main.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o

test: solve
	./solve test.txt

debug: main.c
	$(CC) -c -o main.o $< -DDEBUG $(CFLAGS)
	$(CC) -o solve main.o $(LFLAGS)

.PHONY: clean test debug
