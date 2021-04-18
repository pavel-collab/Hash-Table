#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

//#define DEBUG

// структура элемента списка
typedef struct _List {
    char* key;                     // ключ
    char* value;                   // значение
    unsigned int hash;             // хэш
    struct _List* next;            // указатель на следующий элемент списка
} List;

// структура хэш-таблицы
typedef struct _HashTable {

    List** table;                  // таблица (указатель на массив списков)
    long long capacity;            // размер таблицы
    long long size;                // реальное количество элементов в таблице
    float fill_fact;               // филлфактор (процент заполненности таблицы)

} HashTable;

// сообщения ошибок
static char *ErrorNames[] = {
    "ERROR 1\n"
    "Pointer \'ht\' is NULL\n",

    "ERROR 2\n"
    "Pointer \'table\' is NULL\n",

    "ERROR 3\n"
    "size > capacity\n",

    "ERROR 4\n"
    "capacity = 0\n"
};

// номера ошибок
enum Errors {
    HT_IS_NULL = 1,     //!
    TABLE_IS_NULL,      //!
    OUT_OF_CAPACITY,
    EMPTY_HT
};

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

unsigned int rot13(char* str);

List* list_insert(List* head, char* key, char* value);

int list_free(List* lst);

int list_dump(List* lst, FILE* log);

int ht_dump(HashTable* ht, FILE* log);

int ht_init(HashTable* ht, long long capacity);

float fill_factor(HashTable* ht);

List* ht_lookup(HashTable* ht, char* key);

int ht_rewriting(HashTable* ht);

int ht_realloc(HashTable* ht);

int ht_insert(HashTable* ht, char* key, char* value);

int ht_search(HashTable* ht, char* key);

int ht_remove(HashTable* ht, char* key);

int ht_free(HashTable* ht);

int ht_verefication(HashTable* ht);

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#define DUMP(ht_name) {\
    FILE* log_txt = fopen("log.txt", "a"); \
    assert(log_txt != NULL); \
    ht_dump((ht_name), log_txt); \
    fclose(log_txt);\
}

#ifdef DEBUG

    #define HASH_TABLE_OK(ht) \
    {\
        int error_type = ht_verefication(ht); \
        FILE* log = fopen("log.txt", "a"); \
        switch(error_type) { \
            case HT_IS_NULL : \
            case TABLE_IS_NULL : {\
                fprintf(log, "error type: %s\n", ErrorNames[error_type - 1]); \
                printf("ERROR!\n information had been writen to the log file\n"); \
                exit(-1); \
            } \
            case OUT_OF_CAPACITY : \
            case EMPTY_HT : {\
                fprintf(log, "error type: %s\n", ErrorNames[error_type - 1]); \
                DUMP(ht); \
                printf("ERROR!\n information had been writen to the log file\n"); \
                exit(-1); \
            } \
            default : { \
                printf("NO ERROR\n"); \
            } \
        } \
        fclose(log); \
    }
#else
    #define HASH_TABLE_OK
#endif

#endif /*_HASH_TABLE_H*/