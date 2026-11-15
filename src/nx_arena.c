#include "nx_arena.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGNMENT 8

NxArena* nx_arena_create(size_t capacity) {
    NxArena *arena = (NxArena*)malloc(sizeof(NxArena));
    if (!arena) return NULL;

    arena->buffer = (char*)mmap(NULL, capacity, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (arena->buffer == MAP_FAILED) {
        free(arena);
        return NULL;
    }

    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
}

void* nx_arena_alloc(NxArena *arena, size_t size) {
    if (!arena || size == 0) return NULL;

    size_t aligned_size = ALIGN_UP(size, ALIGNMENT);

    if (arena->offset + aligned_size > arena->capacity) {
        return NULL; // Out of memory in arena block
    }

    void *ptr = (void*)(arena->buffer + arena->offset);
    arena->offset += aligned_size;
    return ptr;
}

void nx_arena_reset(NxArena *arena) {
    if (arena) {
        arena->offset = 0;
    }
}

void nx_arena_destroy(NxArena *arena) {
    if (arena) {
        if (arena->buffer && arena->buffer != MAP_FAILED) {
            munmap(arena->buffer, arena->capacity);
        }
        free(arena);
    }
}
