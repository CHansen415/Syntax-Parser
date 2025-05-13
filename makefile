CC = gcc
CFLAGS = -Wall

all: cooke_parser

cooke_parser: front.o parser.o 
	$(CC) $(CFLAGS) -o cooke_parser front.o parser.o

front.o: front.c front.h parser.h
	$(CC) $(CFLAGS) -c front.c
parser.o: parser.c parser.h front.h
	$(CC) $(CFLAGS) -c parser.c

clean: 
	rm -f *.o cooke_analyzer
