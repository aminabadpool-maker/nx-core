#ifndef NX_ARENA_H
#define NX_ARENA_H

#include <stddef.h>
#include <stdint.h>

#define NX_ALIGNMENT 64

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} NxArena;

NxArena* nx_arena_create(size_t capacity);
void* nx_arena_alloc(NxArena *arena, size_t size);
void nx_arena_reset(NxArena *arena);
void nx_arena_free(NxArena *arena);

#endif
