all: 7colors

7colors: 7colors.o
	gcc 7colors.o -o 7colors

7colors.o: 7colors.c
	gcc -Wall -Werror -Wextra 7colors.c -c -g
