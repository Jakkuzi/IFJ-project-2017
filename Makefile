CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99
PRJ=IFJ-2017
SOURCES=strings.c scanner.c syntax_check.c parser.c prec.c symtable.c semantic_check.c
NAME=xzichj00

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(PRJ)

deploy:
	tar cvfz $(NAME).tgz ./*.c ./*.h Makefile

clean:
	rm -f $(PRJ) $(NAME).tgz

test:
	python3 tests.py $(PRJ)
