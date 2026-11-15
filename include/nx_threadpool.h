#ifndef NX_THREADPOOL_H
#define NX_THREADPOOL_H

#include <pthread.h>

typedef struct {
    void (*function)(void* arg);
    void *arg;
} NxTask;

typedef struct {
    pthread_t *threads;
    size_t thread_count;
    NxTask *queue;
    size_t queue_capacity;
    size_t head;
    size_t tail;
    size_t count;
    pthread_mutex_t lock;
    pthread_cond_t notify;
    int shutdown;
} NxThreadPool;

NxThreadPool* nx_threadpool_create(size_t thread_count, size_t queue_capacity);
int nx_threadpool_add_task(NxThreadPool *pool, void (*function)(void*), void *arg);
void nx_threadpool_destroy(NxThreadPool *pool);

#endif // NX_THREADPOOL_H
