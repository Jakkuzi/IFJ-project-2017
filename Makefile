all:
	gcc -std=c99 strings.c scanner.c syntax_check.c parser.c prec.c -Werror -Wall -pedantic -o a.out 
