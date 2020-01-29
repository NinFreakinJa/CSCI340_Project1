all: CSCI340_P1
clean:
	rm -f CSCI340_P1
CSCI340_P1: CSCI340_P1.c
	gcc -Wall -g -o CSCI340_P1 CSCI340_P1.c