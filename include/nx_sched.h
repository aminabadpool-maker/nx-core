#ifndef NX_SCHED_H
#define NX_SCHED_H

#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>

typedef void (*NxTaskFunc)(void *arg);

typedef struct {
    NxTaskFunc func;
    void *arg;
} NxTask;

#define NX_QUEUE_CAPACITY 256

typedef struct {
    NxTask tasks[NX_QUEUE_CAPACITY];
    atomic_size_t head;
    atomic_size_t tail;
} NxWorkQueue;

typedef struct {
    int num_threads;
    int running;
} NxScheduler;

NxScheduler* nx_sched_init(int num_threads);
bool nx_sched_push(NxScheduler *sched, NxTaskFunc func, void *arg);
void nx_sched_wait(NxScheduler *sched);
void nx_sched_destroy(NxScheduler *sched);

#endif
