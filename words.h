#ifndef _WORDS_H
#define _WORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000

// Struct for items in the dict
typedef struct DictItem {
    char *key;
    int value;
    struct DictItem *next;
} DictItem;

// Struct for the dict 
typedef struct Dictionary {
    DictItem **items;
} Dictionary;

// Hash function to map keys to bucket indices
unsigned int hash(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    return hash % TABLE_SIZE;
}

// Create a new dict
Dictionary *create_dict() {
    Dictionary *dict = malloc(sizeof(Dictionary));
    dict->items = calloc(TABLE_SIZE, sizeof(DictItem *));
    return dict;
}

// Insert key-value pair into the dictionary
void dict_insert(Dictionary *dict, const char *key, int value) {
    unsigned int index = hash(key);
    DictItem *new_item = malloc(sizeof(DictItem));
    new_item->key = strdup(key);
    new_item->value = value;
    new_item->next = dict->items[index];
    dict->items[index] = new_item;
}

// Search for a key in the dictionary and return its value
// Returns 1 if found and sets `result` to the value, otherwise returns 0
int dict_search(Dictionary *dict, const char *key, int *result) {
    unsigned int index = hash(key);
    DictItem *item = dict->items[index];
    while (item) {
        if (strcmp(item->key, key) == 0) {
            *result = item->value;
            return 1;
        }
        item = item->next;
    }
    return 0;  // Key not found
}

// Free the memory used by the dictionary
void free_dict(Dictionary *dict) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        DictItem *item = dict->items[i];
        while (item) {
            DictItem *next = item->next;
            free(item->key);
            free(item);
            item = next;
        }
    }
    free(dict->items);
    free(dict);
}

#endif