#include "nx_sched.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static NxWorkQueue global_queue = { .head = 0, .tail = 0 };
static pthread_t *worker_threads = NULL;

static void* worker_routine(void *arg) {
    NxScheduler *sched = (NxScheduler*)arg;
    while (sched->running) {
        size_t h = atomic_load(&global_queue.head);
        size_t t = atomic_load(&global_queue.tail);

        if (h != t) {
            if (atomic_compare_exchange_weak(&global_queue.head, &h, (h + 1) % NX_QUEUE_CAPACITY)) {
                NxTask task = global_queue.tasks[h];
                if (task.func) {
                    task.func(task.arg);
                }
                continue;
            }
        }
        usleep(1000);
    }
    return NULL;
}

NxScheduler* nx_sched_init(int num_threads) {
    NxScheduler *sched = (NxScheduler*)malloc(sizeof(NxScheduler));
    if (!sched) return NULL;

    sched->num_threads = num_threads;
    sched->running = 1;

    worker_threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&worker_threads[i], NULL, worker_routine, sched);
    }

    return sched;
}

bool nx_sched_push(NxScheduler *sched, NxTaskFunc func, void *arg) {
    (void)sched;
    size_t t = atomic_load(&global_queue.tail);
    size_t h = atomic_load(&global_queue.head);

    size_t next_tail = (t + 1) % NX_QUEUE_CAPACITY;
    if (next_tail == h) {
        return false;
    }

    global_queue.tasks[t].func = func;
    global_queue.tasks[t].arg = arg;
    atomic_store(&global_queue.tail, next_tail);
    return true;
}

void nx_sched_wait(NxScheduler *sched) {
    (void)sched;
    while (atomic_load(&global_queue.head) != atomic_load(&global_queue.tail)) {
        usleep(1000);
    }
}

void nx_sched_destroy(NxScheduler *sched) {
    if (!sched) return;
    sched->running = 0;
    for (int i = 0; i < sched->num_threads; i++) {
        pthread_join(worker_threads[i], NULL);
    }
    free(worker_threads);
    free(sched);
}
