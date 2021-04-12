#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

typedef struct _List {
    char* string;
    struct _List* next;
} List;

typedef struct _HashTable {

    List** table;             // таблица (указатель на массив списков)
    long int size;            // размер таблицы

} HashTable;

#endif /*_HASH_TABLE_H*/