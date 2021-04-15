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
List* list_insert(List* head, char* key, char* value) {
    List* box = (List*) malloc(sizeof(List));

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

// дамп отдельного списка
int list_dump(List* lst, FILE* log) {

    while (lst != NULL) {

        fprintf(log, "key {%s} %s -> ", lst->key, lst->value);
        lst = lst->next;
    }
    fprintf(log, "NULL\n");
    return 0;
}

// дамп всей хэш-таблицы
int ht_dump(HashTable* ht, FILE* log) {

    fprintf(log, "\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
    fprintf(log, "\nHash Table DUMP\n");
    if (ht == NULL) {
        fprintf(log, "HT (ERR) (null ptr)\n");
        return -1;
    }
    else {
        fprintf(log, "HT (OK)\n\n");
        fprintf(log, "Table ptr [%x]\n", ht);
        fprintf(log, "List array ptr [%x]\n", ht->table);
        fprintf(log, "Table capacity = %lld\n", ht->capacity);
        fprintf(log, "Table size = %lld\n", ht->size);
        fprintf(log, "Table fill factor = %0.2f\n\n", ht->fill_fact);

        for (long long i = 0; i < ht->capacity; i++) {
            fprintf(log, "lst(%lld) [%x] ", i, &ht->table[i]);
            if (ht->table[i]) {
                fprintf(log, "| hash: (%8x) | ", ht->table[i]->hash);
            }
            list_dump(ht->table[i], log);
        }
    }

    fprintf(log, "\nDUMP end.\n\n");
    fprintf(log, "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    return 0;
}

// создание хэш-таблицы
int ht_init(HashTable* ht, long long capacity) {

    ht->table = (List**) calloc(capacity, sizeof(List*));
    assert(ht->table != NULL);

    ht->capacity = capacity;

    return 0;
}

// пересчет филл-фактора
float fill_factor(HashTable* ht) {

    assert(ht->capacity != 0);
    double fill_fact = (float) ht->size / ht->capacity;
    return fill_fact;
}

// поиск элемента в таблице
List* ht_lookup(HashTable* ht, char* key, char* value) {

    unsigned int hash = rot13(key);
    List* lst = ht->table[hash % ht->capacity]; // встали на нужный список

    while ((lst != NULL) && (strcmp(lst->value, value) != 0)) {
        lst = lst->next;
    }

    return lst;
}

// перезапись таблицы (пересчет хэшей после реаллокации)
int ht_rewriting(HashTable* ht) {

    List* lst = NULL;
    for (int i = 0; i < ht->capacity; i++) {

        lst = ht->table[i];

        while (lst) {
            ht->table[i] = NULL;
            ht->size--;
            ht_insert(ht, lst->key, lst->value);

            lst = lst->next;
        }

        lst = NULL;
    }

    printf("REWRITING\n");
    return 0;
}

// реаллокация (расширение таблицы)
int ht_realloc(HashTable* ht) {

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

    return 0;
}

// вставка элемента в таблицу
int ht_insert(HashTable* ht, char* key, char* value) {

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
        //DUMP(ht);
        ht_realloc(ht);
    }

    return 0;
}

// поиск значения по ключу
int ht_search(HashTable* ht, char* key) {

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

// удаление таблицы
int ht_free(HashTable* ht) {

    for (int i = 0; i < ht->capacity; i++) {
        list_free(ht->table[i]);
    }
    free(ht->table);

    printf("Hash table has been removed successful!\n");
    return 0;
}

int main() {

    printf("start of program\n");

    HashTable ht = {NULL, 0, 0, 0};
    long long capacity = 5;
    ht_init(&ht, capacity);

    char* key = (char*) calloc(10, sizeof(char));
    char* value = (char*) calloc(10, sizeof(char));
    for (int i = 0; i < 6; i++) {
        printf("input key: ");
        gets(key);
        printf("input value: ");
        gets(value);

        ht_insert(&ht, key, value);
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
//* fillfactor           |
//* ht realloc up        |
//* поиск элемента в таблице по ключу

//TODO: извлечение всей цепочки с одним хэшом

//* dump


//TODO: вариант решения коллизий спомощью открытой адресации

//TODO: unit test
//TODO: интерфейс