#include "nx_arena.h"
#include "nx_sched.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#define ITERATIONS 100000

static uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

static void bench_task(void *arg) {
    volatile int *counter = (int*)arg;
    (*counter)++;
}

int main() {
    printf("=== Starting nx-core Performance Benchmarks ===\n");

    // ۱. بنچمارک NxArena (تخصیص حافظه)
    NxArena *arena = nx_arena_create(1024 * 1024); // 1MB Arena
    assert(arena != NULL);

    uint64_t start_arena = get_time_ns();
    for (int i = 0; i < ITERATIONS; i++) {
        void *p = nx_arena_alloc(arena, 64);
        if (!p) {
            nx_arena_reset(arena);
            p = nx_arena_alloc(arena, 64);
        }
    }
    uint64_t end_arena = get_time_ns();
    uint64_t duration_arena = end_arena - start_arena;
    
    printf("[BENCH] NxArena: %d allocations took %llu ns (Avg: %.2f ns/alloc)\n", 
           ITERATIONS, (unsigned long long)duration_arena, (double)duration_arena / ITERATIONS);
    nx_arena_free(arena);

    // ۲. بنچمارک NxScheduler (زمان‌بند قفل‌آزاد)
    NxScheduler *sched = nx_sched_init(4);
    assert(sched != NULL);

    int shared_counter = 0;
    uint64_t start_sched = get_time_ns();
    
    int tasks_pushed = 0;
    for (int i = 0; i < 200; i++) {
        if (nx_sched_push(sched, bench_task, &shared_counter)) {
            tasks_pushed++;
        }
    }

    nx_sched_wait(sched);
    uint64_t end_sched = get_time_ns();
    uint64_t duration_sched = end_sched - start_sched;

    printf("[BENCH] NxScheduler: Executed %d tasks across threads in %llu ns (Avg: %.2f ns/task)\n", 
           tasks_pushed, (unsigned long long)duration_sched, (double)duration_sched / (tasks_pushed ? tasks_pushed : 1));

    nx_sched_destroy(sched);
    printf("=== Benchmarks Completed Successfully! ===\n");
    return 0;
}
