#include "nx_hash.h"
#include <stdlib.h>
#include <string.h>

static uint32_t hash_func(const char *key, size_t capacity) {
    uint32_t hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // djb2
    }
    return hash % capacity;
}

NxHashMap* nx_hash_create(size_t capacity) {
    NxHashMap *map = (NxHashMap*)malloc(sizeof(NxHashMap));
    if (!map) return NULL;
    map->capacity = capacity;
    map->size = 0;
    map->buckets = (NxHashEntry**)calloc(capacity, sizeof(NxHashEntry*));
    return map;
}

void nx_hash_set(NxHashMap *map, const char *key, void *value) {
    uint32_t idx = hash_func(key, map->capacity);
    NxHashEntry *entry = map->buckets[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    NxHashEntry *new_entry = (NxHashEntry*)malloc(sizeof(NxHashEntry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = map->buckets[idx];
    map->buckets[idx] = new_entry;
    map->size++;
}

void* nx_hash_get(NxHashMap *map, const char *key) {
    uint32_t idx = hash_func(key, map->capacity);
    NxHashEntry *entry = map->buckets[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void nx_hash_destroy(NxHashMap *map) {
    if (!map) return;
    for (size_t i = 0; i < map->capacity; i++) {
        NxHashEntry *entry = map->buckets[i];
        while (entry) {
            NxHashEntry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}
