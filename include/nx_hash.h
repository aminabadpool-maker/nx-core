#ifndef NX_HASH_H
#define NX_HASH_H

#include <stddef.h>
#include <stdint.h>

typedef struct NxHashEntry {
    char *key;
    void *value;
    struct NxHashEntry *next;
} NxHashEntry;

typedef struct {
    size_t capacity;
    size_t size;
    NxHashEntry **buckets;
} NxHashMap;

NxHashMap* nx_hash_create(size_t capacity);
void nx_hash_set(NxHashMap *map, const char *key, void *value);
void* nx_hash_get(NxHashMap *map, const char *key);
void nx_hash_destroy(NxHashMap *map);

#endif // NX_HASH_H
