CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -pedantic -Werror
PRJ=IFJ-2017
SOURCES=strings.c scanner.c generator.c syntax_check.c parser.c prec.c symtable.c semantic_check.c 
NAME=xzichj00
FILENAME = dokumentace

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(PRJ)

pack: doc
	tar cvfz $(NAME).tgz ./*.c ./*.h Makefile $(FILENAME).pdf rozdeleni
	mkdir test-odevzdani
	echo "\n\n\nLOG z is_it_ok.sh\n"
	./is_it_ok.sh $(NAME).tgz test-odevzdani
	rm -r test-odevzdani 

clean:
	rm -f $(NAME).tgz $(FILENAME).log $(FILENAME).aux $(FILENAME).ps $(FILENAME).dvi 
 
test:
	python3 tests.py $(PRJ)

doc: $(FILENAME).ps
	ps2pdf $(FILENAME).ps

$(FILENAME).ps: $(FILENAME).dvi
	dvips $(FILENAME).dvi

$(FILENAME).dvi: $(FILENAME).tex
	latex $(FILENAME).tex