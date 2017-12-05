CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -pedantic -Werror
PRJ=IFJ-2017
SOURCES=strings.c scanner.c generator.c syntax_check.c parser.c prec.c symtable.c semantic_check.c 
NAME=xzichj00

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(PRJ)

test-pack:
	tar cvfz $(NAME).tgz ./*.c ./*.h Makefile tests.py tests/*

pack:
	tar cvfz $(NAME).tgz ./*.c ./*.h Makefile

clean:
	rm -f $(PRJ) $(NAME).tgz

test:
	python3 tests.py $(PRJ)
