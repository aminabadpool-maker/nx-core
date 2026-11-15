#include "nx_arena.h"
#include <stdio.h>
#include <assert.h>

int main() {
    printf("--- Testing NxArena ---\n");
    
    NxArena *arena = nx_arena_create(1024);
    assert(arena != NULL);
    
    void *p1 = nx_arena_alloc(arena, 128);
    assert(p1 != NULL);
    assert(((uintptr_t)p1 % 64) == 0);
    printf("[OK] Allocated 128 bytes with 64-byte alignment at %p\n", p1);

    void *p2 = nx_arena_alloc(arena, 256);
    assert(p2 != NULL);
    printf("[OK] Allocated 256 bytes at %p\n", p2);

    nx_arena_reset(arena);
    assert(arena->offset == 0);
    printf("[OK] Arena reset successfully (offset = 0)\n");

    nx_arena_free(arena);
    printf("--- All Arena Tests Passed! ---\n");
    return 0;
}
