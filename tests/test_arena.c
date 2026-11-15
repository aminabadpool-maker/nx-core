#include "nx_arena.h"
#include "nx_sched.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

static void sample_task(void *arg) {
    int *val = (int*)arg;
    (*val)++;
}

int main() {
    printf("--- Testing NxArena & NxSched ---\n");
    
    // تست آرنا
    NxArena *arena = nx_arena_create(1024);
    assert(arena != NULL);
    void *p1 = nx_arena_alloc(arena, 128);
    assert(p1 != NULL);
    assert(((uintptr_t)p1 % 64) == 0);
    printf("[OK] Arena allocation & 64-byte alignment passed.\n");
    nx_arena_free(arena);

    // تست زمان‌بند
    NxScheduler *sched = nx_sched_init(2);
    assert(sched != NULL);

    int counter = 0;
    bool pushed = nx_sched_push(sched, sample_task, &counter);
    assert(pushed);

    nx_sched_wait(sched);
    assert(counter == 1);
    printf("[OK] Scheduler task execution passed (counter = %d).\n", counter);

    nx_sched_destroy(sched);
    printf("--- All Tests Passed Successfully! ---\n");
    return 0;
}
