#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

// создание ячейки списка; ячейка встает на голову списка
int list_insert(List** lst, char* str) {
    List* box = malloc(sizeof(List));

    box->string = str;

    box->next = *lst;
    *lst = box;

    return 0;
}

// удаление списка
int list_free(List* lst) {

    if (lst) {
        List* next = lst->next;
        free(lst);
        list_free(next);
    }

    return 0;
}

// хэш-функция
unsigned int rot13(char* str) {

    unsigned int hash = 0;

    while (*str) {

        hash += (unsigned char)(*str);
        hash += (hash << 10);
	    hash ^= (hash >> 6); // побитовый XOR в совмещении с присваиванием
    }

    hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

    return hash;
}

// создание хэш-таблицы
int ht_init(HashTable* ht, long int size) {

    ht->table = (List**) calloc(sizeof(List*), size); //! <-
    assert(ht->table != NULL);

    ht->size = size;

    return 0;
}

// поиск элемента в таблице
List* ht_lookup(HashTable* ht, char* str) {

    unsigned int hash = rot13(str);
    List* lst = ht->table[hash % ht->size]; // встали на нужный список

    while ((strcmp(lst->string, str) == 0) || (lst == NULL)) {
        lst = lst->next;
    }

    return lst;
}

// вставка элемента в таблицу
int ht_insert(HashTable* ht, char* str) {

    List* lst = ht_lookup(ht, str);

    if (lst) {
        printf("This key already in table.\n");
        free(lst);
        return 0;
    }

    list_insert(&lst, str);
    ht->table[rot13(str) % ht->size] = lst;

    return 0;
}

//???
int ht_search(HashTable* ht, char* str) {

    List* lst = ht_lookup(ht, str);

    if (lst == NULL) {
        printf("There is no such element in the table\n");
        free(lst);
        return 0;
    }
    else {
        printf("data: ");
        puts(lst->string);
        printf("\n");

        printf("hash = %u", rot13(lst->string));
        return 0;
    }
}

//???
/*int ht_remove(HashTable* ht, char* str) {

    List* lst = ht_lookup(ht, str);

    if (lst == NULL) {
        printf("There is no such element in the table\n");
        free(lst);
        return 0;
    }


}*/

// удаление таблицы
int ht_free(HashTable* ht) {

    for (int i = 0; i < ht->size; i++) {
        list_free(ht->table[i]);
    }
    free(ht->table);
    free(ht);
}

int main() {

    printf("start of program\n");

    HashTable* ht = NULL;
    long int size = 10;
    ht_init(ht, size);
    printf("ht init\n");

    char* str = NULL;
    for (int i = 0; i < size - 6; i++) {
        printf("input string: ");
        gets(str);
        printf("hash = %u\n", rot13(str));

        ht_insert(ht, str);
        str = NULL;
    }

    ht_search(ht, "aaa");
    ht_search(ht, "bbb");
    ht_search(ht, "ccc");

    ht_free(ht);
    printf("end of program\n");

    return 0;
}

//* ht struct
//* hash table construct
//* hash table distruct
//TODO: hash functions (one more)
//TODO: fillfactor       |
//TODO: ht realloc up    |
//TODO: поиск элемента в таблице

//TODO: извлечение всей цепочки по заданному хэшу

//TODO: dump
//TODO: history log
//TODO: вариант решения коллизий спомощью открытой адресации

//TODO: unit test