#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

typedef struct _List {
    char* key;                     // ключ
    char* value;                   // значение
    unsigned int hash;             // хэш
    struct _List* next;            // указатель на следующий элемент списка
} List;

typedef struct _HashTable {

    List** table;                  // таблица (указатель на массив списков)
    long long capacity;            // размер таблицы
    long long size;                // реальное количество элементов в таблице
    float fill_fact;               // филлфактор (процент заполненности таблицы)

} HashTable;

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

unsigned int rot13(char* str);

List* list_insert(List* head, char* key, char* value);

int list_free(List* lst);

int list_dump(List* lst, FILE* log);

int ht_dump(HashTable* ht, FILE* log);

int ht_init(HashTable* ht, long long capacity);

float fill_factor(HashTable* ht);

List* ht_lookup(HashTable* ht, char* key, char* value);

int ht_rewriting(HashTable* ht);

int ht_realloc(HashTable* ht);

int ht_insert(HashTable* ht, char* key, char* value);

int ht_search(HashTable* ht, char* key);

int ht_free(HashTable* ht);

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#define DUMP(ht_name) {\
    FILE* log_txt = fopen("log.txt", "a"); \
    assert(log_txt != NULL); \
    ht_dump((ht_name), log_txt); \
    fclose(log_txt);\
}

#endif /*_HASH_TABLE_H*/