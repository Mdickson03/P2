
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000

// Initialize the hash map
HashMap* create_hash_map() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->table = (HashItem **)malloc(TABLE_SIZE * sizeof(HashItem *));
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
    return map;
}

// Insert key-value pair into the hash map
void insert(HashMap *map, const char *key, int value) {
    unsigned long index = hash(key);
    HashItem *new_item = (HashItem *)malloc(sizeof(HashItem));
    new_item->key = strdup(key);
    new_item->value = value;
    new_item->next = map->table[index];
    map->table[index] = new_item;
}

// Search for a key in the hash map
int search(HashMap *map, const char *key) {
    unsigned long index = hash(key);
    HashItem *item = map->table[index];
    while (item) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
        item = item->next;
    }
    return -1;  // Key not found
}

// Delete a key from the hash map
void delete(HashMap *map, const char *key) {
    unsigned long index = hash(key);
    HashItem *item = map->table[index];
    HashItem *prev = NULL;

    while (item) {
        if (strcmp(item->key, key) == 0) {
            if (prev) {
                prev->next = item->next;
            } else {
                map->table[index] = item->next;
            }
            free(item->key);
            free(item);
            return;
        }
        prev = item;
        item = item->next;
    }
}

// Free the memory of the hash map
void free_hash_map(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashItem *item = map->table[i];
        while (item) {
            HashItem *temp = item;
            item = item->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

// Main function to test the hash map
int main() {
    HashMap *map = create_hash_map();
    
    // Insert key-value pairs
    insert(map, "apple", 10);
    insert(map, "banana", 20);
    insert(map, "grape", 30);
    
    // Search for values
    printf("apple: %d\n", search(map, "apple"));
    printf("banana: %d\n", search(map, "banana"));
    printf("orange: %d\n", search(map, "orange"));  // Not found
    
    // Delete a key
    delete(map, "apple");
    printf("apple after deletion: %d\n", search(map, "apple"));  // Should print -1
    
    // Free the hash map
    free_hash_map(map);
    
    return 0;
}