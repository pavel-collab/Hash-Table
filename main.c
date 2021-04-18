#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

void print_instruction() {
    printf("\n");
    printf("Exit                  -- to exit\n");
    printf("MENU                  -- to show menu\n");
    printf("SET /*key*/ /*value*/ -- set pair: key-calue\n");
    printf("GET /*key*/           -- find value by key\n");
    printf("HASH /*string*/       -- get hash of string\n");
    printf("LISL /*idx*/          -- get list [number]\n");
    printf("DUMP                  -- make a dump to log\n\n");
}

#define EXIT {\
    if (strcmp(cmd, "Exit") == 0) {\
        break;\
    }\
}

#define MENU {\
    if (strcmp(cmd, "MENU") == 0) {\
        print_instruction();\
    }\
}

#define SET {\
    if (strcmp(cmd, "SET") == 0) {\
        key = strtok(NULL, " \t\n");\
        value = strtok(NULL, " \t\n");\
        if ((key == NULL) || (value == NULL)) {\
            printf("Error, try again.\n\n");\
            print_instruction();\
        }\
        else {\
            ht_insert(&ht, key, value);\
        }\
    }\
}

#define GET {\
    if (strcmp(cmd, "GET") == 0) {\
        key = strtok(NULL, " \t\n");\
        if (key == NULL) {\
            printf("Error, try again.\n\n");\
            print_instruction();\
        }\
        else {\
            ht_search(&ht, key);\
        }\
    }\
}

#define HASH {\
    if (strcmp(cmd, "HASH") == 0) {\
        key = strtok(NULL, " \t\n");\
        if (key == NULL) {\
            printf("Error, try again.\n\n");\
            print_instruction();\
        }\
        else {\
            unsigned int hash = rot13(key);\
            printf("hash = [%x]\n", hash);\
        }\
    }\
}

#define LIST {\
    if (strcmp(cmd, "LIST") == 0) {\
        char* idx_s = (char*) calloc(10, sizeof(char));\
        idx_s = strtok(NULL, " \t\n");\
        if (idx_s == NULL) {\
            printf("Error, try again.\n\n");\
            print_instruction();\
        }\
        else {\
            long long idx = atoll(idx_s);\
            if (idx < ht.capacity) {\
                List* lst = ht.table[idx];\
                while (lst != NULL) {\
                    printf("(key {%s} %s) -> ", lst->key, lst->value);\
                    lst = lst->next;\
                }\
                free(lst);\
                printf("NULL\n");\
            }\
            else {\
                printf("Error, idx > capaciry. Try again.\n\n");\
                print_instruction();\
            }\
        }\
    }\
}

#define HT_DUMP {\
    if (strcmp(cmd, "DUMP") == 0) {\
        DUMP(&ht);\
    }\
}

int main() {

    //printf("start of program\n");

    HashTable ht = {NULL, 0, 0, 0};
    long long capacity = 10;
    ht_init(&ht, capacity);

    char* select_act = (char*) calloc(20, sizeof(char));
    char* cmd = (char*) calloc(10, sizeof(char));
    char* key = (char*) calloc(10, sizeof(char));
    char* value = (char*) calloc(10, sizeof(char));

    print_instruction();

    while (1) {
        printf("\nEnter action: ");
        gets(select_act);
        cmd = strtok(select_act, " \t\n");
        if (cmd == NULL) {
            printf("Error, try again.\n\n");
            print_instruction();
        }
        EXIT;
        MENU;
        SET;
        GET;
        HASH;
        LIST;
        HT_DUMP;
    }

    DUMP(&ht);
    ht_free(&ht);
    //printf("\nend of program\n");

    return 0;
}

//* ht struct
//* hash table construct
//* hash table distruct
//* верефикатор (проверка, все строки в списке имеют один хэш)
//TODO: hash functions (one more)
//* fillfactor           |
//* ht realloc up        |
//* поиск элемента в таблице по ключу

//TODO: извлечение всей цепочки с одним хэшом

//* dump


//TODO: вариант решения коллизий спомощью открытой адресации

//TODO: unit test
//TODO: интерфейс

//####################################################################
// макет интерфейса

// Do you want want to open data base? (y/n)
// Please enter file name: /* file name */

// MENU: ...

// comands list:
//    SET /* key, value */ (положить значение с ключом в таблицу)
//    GET /* key */ (найти значение по ключу)
//??? STAT (получение статистики)
//--------------------------------------------------------------------

//    HASH /* key */ (получить хэш ключа)
//    GET LIST /* hash */ (получить список с заданным значнием хэша)

//####################################################################