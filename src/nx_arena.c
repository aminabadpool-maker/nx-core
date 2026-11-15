#include "nx_arena.h"
#include <stdlib.h>
#include <string.h>

NxArena* nx_arena_create(size_t capacity) {
    NxArena *arena = (NxArena*)malloc(sizeof(NxArena));
    if (!arena) return NULL;

    int result = posix_memalign((void**)&arena->buffer, NX_ALIGNMENT, capacity);
    if (result != 0) {
        free(arena);
        return NULL;
    }

    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
}

void* nx_arena_alloc(NxArena *arena, size_t size) {
    size_t aligned_size = (size + (NX_ALIGNMENT - 1)) & ~(NX_ALIGNMENT - 1);
    
    if (arena->offset + aligned_size > arena->capacity) {
        return NULL;
    }

    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}

void nx_arena_reset(NxArena *arena) {
    arena->offset = 0;
}

void nx_arena_free(NxArena *arena) {
    if (arena) {
        free(arena->buffer);
        free(arena);
    }
}
