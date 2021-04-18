#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// создание ячейки списка; ячейка встает на голову списка
List* list_insert(List* head, char* key, char* value) {
    List* box = (List*) malloc(sizeof(List));
    assert(box != NULL);

    char* list_key = (char*) calloc(strlen(key), sizeof(char));
    char* list_value = (char*) calloc(strlen(value), sizeof(char));
    strcpy(list_key, key);
    strcpy(list_value, value);

    box->key = list_key;
    box->value = list_value;
    box->hash = rot13(list_key);
    box->next = head;

    return box;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// удаление списка
int list_free(List* lst) {

    if (lst) {
        List* next = lst->next;
        free(lst->key);
        free(lst->value);
        free(lst);
        list_free(next);
    }

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// дамп отдельного списка
int list_dump(List* lst, FILE* log) {

    while (lst != NULL) {

        fprintf(log, "(key {%s} %s) -> ", lst->key, lst->value);
        lst = lst->next;
    }
    fprintf(log, "NULL\n");
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// дамп всей хэш-таблицы
int ht_dump(HashTable* ht, FILE* log) {

    HASH_TABLE_OK(ht);

    fprintf(log, "\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
    fprintf(log, "\nHash Table DUMP\n");
    if (ht == NULL) {
        fprintf(log, "HT (ERR) (null ptr)\n");
        return -1;
    }
    else {
        fprintf(log, "HT (OK)\n\n");
        fprintf(log, "Table ptr [%p]\n", ht);
        fprintf(log, "List array ptr [%p]\n", ht->table);
        fprintf(log, "Table capacity = %lld\n", ht->capacity);
        fprintf(log, "Table size = %lld\n", ht->size);
        fprintf(log, "Table fill factor = %0.2f\n\n", ht->fill_fact);

        for (long long i = 0; i < ht->capacity; i++) {
            fprintf(log, "lst(%lld) [%p] ", i, &ht->table[i]);
            if (ht->table[i]) {
                fprintf(log, "| hash: (%8x) | ", ht->table[i]->hash);
            }
            list_dump(ht->table[i], log);
        }
    }

    fprintf(log, "\nDUMP end.\n\n");
    fprintf(log, "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    HASH_TABLE_OK(ht);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// создание хэш-таблицы
int ht_init(HashTable* ht, long long capacity) {

    assert(ht != NULL);
    ht->table = (List**) calloc(capacity, sizeof(List*));
    assert(ht->table != NULL);

    ht->capacity = capacity;
    HASH_TABLE_OK(ht);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// пересчет филл-фактора
float fill_factor(HashTable* ht) {
    HASH_TABLE_OK(ht);
    assert(ht->capacity != 0);
    double fill_fact = (float) ht->size / ht->capacity;
    return fill_fact;
    HASH_TABLE_OK(ht);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// поиск элемента в таблице
List* ht_lookup(HashTable* ht, char* key, char* value) {
    HASH_TABLE_OK(ht);
    unsigned int hash = rot13(key);
    List* lst = ht->table[hash % ht->capacity]; // встали на нужный список

    while ((lst != NULL) && (strcmp(lst->value, value) != 0)) {
        lst = lst->next;
    }
    HASH_TABLE_OK(ht);
    return lst;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// перезапись таблицы (пересчет хэшей после реаллокации)
int ht_rewriting(HashTable* ht) {
    HASH_TABLE_OK(ht);
    List* lst = NULL;
    for (int i = 0; i < ht->capacity; i++) {

        lst = ht->table[i];
        ht->table[i] = NULL;

        while (lst) {
            ht->size--;
            ht_insert(ht, lst->key, lst->value);

            lst = lst->next;
        }

        lst = NULL;
    }
    
    printf("REWRITING\n");
    HASH_TABLE_OK(ht);
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// реаллокация (расширение таблицы)
int ht_realloc(HashTable* ht) {
    HASH_TABLE_OK(ht);
    // create local variabe
    void* local_arrow = realloc(ht->table, ((ht->capacity) * 2) * sizeof(List*));

    // if the local return NULL, there is fail --> abort the program
    if (local_arrow == NULL) {
        printf("REALLOCATION FAILED!\n\n");
        return -1;
    }
    else {
        ht->table = (List**) local_arrow;
    }

    ht->capacity = ((ht->capacity)*2);
    ht->fill_fact = fill_factor(ht);

    for (int i = (ht->capacity / 2); i < ht->capacity; i++) {
        ht->table[i] = NULL;
    }

    ht_rewriting(ht);

    printf("REALLOCATION\n");
    HASH_TABLE_OK(ht);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// вставка элемента в таблицу
int ht_insert(HashTable* ht, char* key, char* value) {
    HASH_TABLE_OK(ht);
    List* lst = ht_lookup(ht, key, value);

    if (lst) {
        printf("This element already in table.\n");
        return 0;
    }

    List* head = ht->table[rot13(key) % ht->capacity];
    ht->table[rot13(key) % ht->capacity] = list_insert(head, key, value);
    ht->size++;
    ht->fill_fact = fill_factor(ht);

    if (ht->fill_fact >= 0.7) {
        DUMP(ht);
        ht_realloc(ht);
    }
    HASH_TABLE_OK(ht);
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// поиск значения по ключу
int ht_search(HashTable* ht, char* key) {
    HASH_TABLE_OK(ht);
    unsigned int hash = rot13(key);
    List* lst = ht->table[hash % ht->capacity]; // встали на нужный список

    
    while ((lst != NULL) && (strcmp(lst->key, key) != 0)) {
        lst = lst->next;
    }

    if (lst == NULL) {
        printf("There is no such element in the table\n");
        free(lst);
        return 0;
    }
    else {
        printf("value: ");
        puts(lst->value);
        printf("key: {%s} hash = [%x]\n", lst->key, lst->hash);

        return 0;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// удаление элемента списка
int ht_remove(HashTable* ht, char* key) {
    HASH_TABLE_OK(ht);
    unsigned int hash = rot13(key);
    List* lst = ht->table[hash % ht->capacity]; // встали на нужный список

    if (strcmp(lst->key, key) == 0) {
        ht->table[hash % ht->capacity] = lst->next;
        free(lst);
    }
    else {
        while ((lst->next != NULL) && (strcmp(lst->next->key, key) != 0)) {
            lst = lst->next;
        }

        if (lst->next == NULL) {
            printf("There is no such element in the table\n");
            lst = NULL;
            return 0;
        }
        else {
            List* box = lst->next;
            lst->next = box->next;
            box = NULL;
        }
    }
    HASH_TABLE_OK(ht);
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// удаление таблицы
int ht_free(HashTable* ht) {

    for (int i = 0; i < ht->capacity; i++) {
        list_free(ht->table[i]);
    }
    free(ht->table);

    printf("Hash table has been removed successful!\n");
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int ht_verefication(HashTable* ht) {

    if (ht == NULL)
        return HT_IS_NULL;

    if (ht->table == NULL)
        return TABLE_IS_NULL;

    if (ht->size > ht->capacity)
        return OUT_OF_CAPACITY;

    if (ht->capacity == 0) {
        return EMPTY_HT;
    }

    return 0;

}