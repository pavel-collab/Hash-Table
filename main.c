#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

// создание ячейки списка; ячейка встает на голову списка
List* list_insert(List* head, char* str) {
    List* box = (List*) malloc(sizeof(List));
    box->string = str;
    printf("\n %s \n", box->string);
    box->next = head;
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    printf("box [%x]\n", box);
    printf("box->next [%x]\n", box->next);
    printf("~~~~~~~~~~~~~~~~~~~~\n");

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

int list_dump(List* lst) {

    while (lst != NULL) {

        printf("%s -> ", lst->string);
        lst = lst->next;
    }
    printf("NULL\n");
    return 0;
}

int ht_dump(HashTable* ht) {

    printf("\nHash Table DUMP\n");
    if (ht == NULL) {
        printf("HT (ERR) (null ptr)\n");
    }
    else {
        printf("HT (OK)\n\n");
        printf("Table ptr [%x]\n", ht);
        printf("List array ptr [%x]\n", ht->table);
        printf("Table size = %lld\n\n", ht->size);

        for (long long i = 0; i < ht->size; i++) {
            printf("list(%lld) [%x] ", i, &ht->table[i]);
            list_dump(ht->table[i]);
        }
    }

    printf("\nDUMP end.\n\n");

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
    printf("==========================\n");
    printf("hash = [%x], idx = %d\n", hash, hash % ht->size);
    printf("list adr [%x]\n", lst);

    while ((lst != NULL) && (strcmp(lst->string, str) != 0)) {
        lst = lst->next;
    }

    printf("returned lst is [%x]\n", lst);
    printf("==========================\n");

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

    ht_dump(ht);

    return 0;
}

//???
int ht_search(HashTable* ht, char* str) {

    List* lst = ht_lookup(ht, str);

    if (lst == NULL) {
        printf("There is no such element in the table\n");
        return 0;
    }
    else {
        printf("data: ");
        puts(lst->string);
        printf("hash = [%x]\n", rot13(lst->string));

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
    printf("Hash table has been removed successful!\n");
    return 0;
}

int main() {

    printf("start of program\n");

    HashTable ht = {NULL, 0};
    long long size = 10;
    ht_init(&ht, size);

    char* str = (char*) calloc(10, sizeof(char));
    for (int i = 0; i < size - 7; i++) {
        printf("input string: ");
        gets(str);
        printf("hash = %x, idx = [%d]\n", rot13(str), rot13(str) % ht.size);

        ht_insert(&ht, str);
    }

    ht_dump(&ht);

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

//TODO: dump

//TODO: history log:
//! классы ++
//! хранение последней примененной функции; ссылки на функции -> один !!!прототип фукции!!!

//TODO: вариант решения коллизий спомощью открытой адресации

//TODO: unit test