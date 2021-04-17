test:
	gcc -c main.c
	gcc -c ht_lib.c
	gcc -o ht ht_lib.o main.o
debug:
	gcc -g -c main.c
	gcc -g -c ht_lib.c
	gcc -g -o ht ht_lib.o main.o
sundbox:
	gcc -o sundbox sundbox.c