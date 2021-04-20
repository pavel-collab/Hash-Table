unit_test:
	gcc -c unit_test.c
	gcc -c ht_lib.c
	gcc -o ut ht_lib.o unit_test.o
unit_test_debug:
	gcc -g -c unit_test.c
	gcc -g -c ht_lib.c
	gcc -g -o ut ht_lib.o unit_test.o

hash_table:
	gcc -c main.c
	gcc -c ht_lib.c
	gcc -o ht ht_lib.o main.o
debug:
	gcc -g -c main.c
	gcc -g -c ht_lib.c
	gcc -g -o ht ht_lib.o main.o