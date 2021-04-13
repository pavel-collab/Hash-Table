#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

// хэш-функция
unsigned int rot13(char* str) {

    unsigned int hash = 0;

    while (*str) {

        hash += (unsigned char)(*str);
        hash += (hash << 10);
	    hash ^= (hash >> 6); // побитовый XOR в совмещении с присваиванием
        str++;
    }

    hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

    return hash;
}

// создание ячейки списка; ячейка встает на голову списка
List* list_insert(List* head, char* str) {
    List* box = (List*) malloc(sizeof(List));

    char* list_str = (char*) calloc(strlen(str), sizeof(char));
    strcpy(list_str, str);

    box->string = list_str;
    box->hash = rot13(list_str);
    box->next = head;

    return box;
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

int list_dump(List* lst, FILE* log) {

    while (lst != NULL) {

        fprintf(log, "%s -> ", lst->string);
        lst = lst->next;
    }
    fprintf(log, "NULL\n");
    return 0;
}

int ht_dump(HashTable* ht, FILE* log) {

    fprintf(log, "\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
    fprintf(log, "\nHash Table DUMP\n");
    if (ht == NULL) {
        fprintf(log, "HT (ERR) (null ptr)\n");
    }
    else {
        fprintf(log, "HT (OK)\n\n");
        fprintf(log, "Table ptr [%x]\n", ht);
        fprintf(log, "List array ptr [%x]\n", ht->table);
        fprintf(log, "Table size = %lld\n\n", ht->size);

        for (long long i = 0; i < ht->size; i++) {
            fprintf(log, "lst(%lld) [%x] | hash: (%8x) | ", i, &ht->table[i], ht->table[i]->hash);
            list_dump(ht->table[i], log);
        }
    }

    fprintf(log, "\nDUMP end.\n\n");
    fprintf(log, "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    return 0;
}

// создание хэш-таблицы
int ht_init(HashTable* ht, long long size) {

    ht->table = (List**) calloc(size, sizeof(List*));
    assert(ht->table != NULL);

    ht->size = size;

    return 0;
}

// поиск элемента в таблице
List* ht_lookup(HashTable* ht, char* str) {

    unsigned int hash = rot13(str);
    List* lst = ht->table[hash % ht->size]; // встали на нужный список

    while ((lst != NULL) && (strcmp(lst->string, str) != 0)) {
        lst = lst->next;
    }

    return lst;
}

// вставка элемента в таблицу
int ht_insert(HashTable* ht, char* str) {

    List* lst = ht_lookup(ht, str);

    if (lst) {
        printf("This key already in table.\n");
        return 0;
    }

    List* head = ht->table[rot13(str) % ht->size];
    ht->table[rot13(str) % ht->size] = list_insert(head, str);

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
        printf("hash = [%x]\n", lst->hash);

        return 0;
    }
}

// удаление таблицы
int ht_free(HashTable* ht) {

    for (int i = 0; i < ht->size; i++) {
        list_free(ht->table[i]);
    }
    free(ht->table);

    printf("Hash table has been removed successful!\n");
    return 0;
}

int main() {

    printf("start of program\n");

    HashTable ht = {NULL, 0};
    long long size = 5;
    ht_init(&ht, size);

    char* str = (char*) calloc(10, sizeof(char));
    for (int i = 0; i < 7; i++) {
        printf("input string: ");
        gets(str);

        ht_insert(&ht, str);
    }

    DUMP(&ht);

    ht_search(&ht, "aaa");
    ht_search(&ht, "bbb");
    ht_search(&ht, "ccc");

    ht_free(&ht);
    printf("\nend of program\n");

    return 0;
}

//* ht struct
//* hash table construct
//* hash table distruct
//TODO: верефикатор (проверка, все строки в списке имеют один хэш)
//TODO: hash functions (one more)
//TODO: fillfactor       |
//TODO: ht realloc up    |
//TODO: поиск элемента в таблице

//TODO: извлечение всей цепочки по заданному хэшу

//* dump

//TODO: history log:
//! классы ++
//! хранение последней примененной функции; ссылки на функции -> один !!!прототип фукции!!!

//TODO: вариант решения коллизий спомощью открытой адресации

//TODO: unit test