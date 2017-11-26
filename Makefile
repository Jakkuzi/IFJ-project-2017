all:
	gcc -std=c99 strings.c scanner.c syntax_check.c parser.c prec.c symtable.c -Werror -Wall -Wextra -pedantic -o a.out 

