test:
	gcc -c main.c
	gcc -c ht_lib.c
	gcc -o ht ht_lib.o main.o
debug:
	gcc -g -o ht main.c