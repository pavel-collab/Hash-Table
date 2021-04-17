#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

int main() {

    printf("start of program\n");

    HashTable ht = {NULL, 0, 0, 0};
    long long capacity = 3;
    ht_init(&ht, capacity);

    char* key = (char*) calloc(10, sizeof(char));
    char* value = (char*) calloc(10, sizeof(char));
    for (int i = 0; i < 5; i++) {
        printf("input key: ");
        gets(key);
        printf("input value: ");
        gets(value);

        ht_insert(&ht, key, value);
    }

    HASH_TABLE_OK(&ht);
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