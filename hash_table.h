#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

typedef struct _List {
    char* string;
    unsigned int hash;
    struct _List* next;
} List;

typedef struct _HashTable {

    List** table;             // таблица (указатель на массив списков)
    long long capacity;            // размер таблицы
    long long size;
    float fill_fact;

} HashTable;

#define DUMP(ht_name) {\
    FILE* log_txt = fopen("log.txt", "a"); \
    assert(log_txt != NULL); \
    ht_dump((ht_name), log_txt); \
    fclose(log_txt);\
}

#endif /*_HASH_TABLE_H*/