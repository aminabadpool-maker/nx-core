#ifndef NX_ARENA_H
#define NX_ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char *buffer;
    size_t capacity;
    size_t offset;
} NxArena;

NxArena* nx_arena_create(size_t capacity);
void* nx_arena_alloc(NxArena *arena, size_t size);
void nx_arena_reset(NxArena *arena);
void nx_arena_destroy(NxArena *arena);

#endif // NX_ARENA_H
