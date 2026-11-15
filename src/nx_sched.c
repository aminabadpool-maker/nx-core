#include "nx_sched.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#define QUEUE_CAPACITY 256

typedef struct {
    NxTaskFunc func;
    void *arg;
} Task;

struct NxScheduler {
    Task tasks[QUEUE_CAPACITY];
    _Atomic int head;
    _Atomic int tail;
    int num_threads;
    pthread_t *threads;
    _Atomic bool running;
};

static void* worker_thread(void *arg) {
    NxScheduler *sched = (NxScheduler*)arg;
    while (atomic_load(&sched->running)) {
        int h = atomic_load(&sched->head);
        int t = atomic_load(&sched->tail);
        
        if (h != t) {
            Task task = sched->tasks[h % QUEUE_CAPACITY];
            if (atomic_compare_exchange_weak(&sched->head, &h, (h + 1) % QUEUE_CAPACITY)) {
                if (task.func) {
                    task.func(task.arg);
                }
            }
        } else {
            struct timespec ts = {0, 1000000}; // 1ms
            nanosleep(&ts, NULL);
        }
    }
    return NULL;
}

NxScheduler* nx_sched_init(int num_threads) {
    NxScheduler *sched = (NxScheduler*)malloc(sizeof(NxScheduler));
    if (!sched) return NULL;

    atomic_init(&sched->head, 0);
    atomic_init(&sched->tail, 0);
    atomic_store(&sched->running, true);
    sched->num_threads = num_threads;
    
    sched->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&sched->threads[i], NULL, worker_thread, sched);
    }

    return sched;
}

bool nx_sched_push(NxScheduler *sched, NxTaskFunc func, void *arg) {
    int h = atomic_load(&sched->head);
    int t = atomic_load(&sched->tail);

    if ((t + 1) % QUEUE_CAPACITY == (h % QUEUE_CAPACITY)) {
        return false;
    }

    sched->tasks[t % QUEUE_CAPACITY] = (Task){func, arg};
    atomic_store(&sched->tail, (t + 1) % QUEUE_CAPACITY);
    return true;
}

void nx_sched_wait(NxScheduler *sched) {
    while (atomic_load(&sched->head) != atomic_load(&sched->tail)) {
        struct timespec ts = {0, 1000000};
        nanosleep(&ts, NULL);
    }
}

void nx_sched_destroy(NxScheduler *sched) {
    if (!sched) return;
    atomic_store(&sched->running, false);
    
    for (int i = 0; i < sched->num_threads; i++) {
        pthread_join(sched->threads[i], NULL);
    }

    free(sched->threads);
    free(sched);
}
