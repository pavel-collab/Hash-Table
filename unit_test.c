#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash_table.h"

//! "-ftime-report"

int main() {

    HashTable ht = {NULL, 0, 0, 0};
    long long capacity = 1000;
    ht_init(&ht, capacity);

    char* box = (char*) calloc(50, sizeof(char));
    char* start_str = (char*) calloc(25, sizeof(char));
    char* count_str = (char*) calloc(25, sizeof(char));

    long long start = 0;
    long long count = 0;

    printf("Pleas, input /*start*/ and /*number*/: \nMASS ");
    gets(box);

    start_str = strtok(box, " \t\n");
    assert(start_str != NULL);
    count_str = strtok(NULL, " \t\n");
    assert(count_str != NULL);
    start = atoll(start_str);
    count = atoll(count_str);


    for (long long i = start; i < start + count; i++) {
        char key[100];
        char value[100];

        snprintf(key, 100, "key%lld", i);
        snprintf(value, 100, "value%lld", i);
        ht_insert(&ht, key, value);
    }

    DUMP(&ht);
    
    ht_free(&ht);
    free(box);
    free(start_str);
    free(count_str);
    return 0;
}