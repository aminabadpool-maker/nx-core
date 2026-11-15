#include "nx_threadpool.h"
#include <stdlib.h>
#include <stdio.h>

static void* thread_routine(void *arg) {
    NxThreadPool *pool = (NxThreadPool*)arg;
    while (1) {
        pthread_mutex_lock(&(pool->lock));
        while (pool->count == 0 && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }
        if (pool->shutdown && pool->count == 0) {
            pthread_mutex_unlock(&(pool->lock));
            break;
        }
        NxTask task = pool->queue[pool->head];
        pool->head = (pool->head + 1) % pool->queue_capacity;
        pool->count--;
        pthread_mutex_unlock(&(pool->lock));

        if (task.function) {
            task.function(task.arg);
        }
    }
    return NULL;
}

NxThreadPool* nx_threadpool_create(size_t thread_count, size_t queue_capacity) {
    NxThreadPool *pool = (NxThreadPool*)malloc(sizeof(NxThreadPool));
    if (!pool) return NULL;

    pool->thread_count = thread_count;
    pool->queue_capacity = queue_capacity;
    pool->head = 0;
    pool->tail = 0;
    pool->count = 0;
    pool->shutdown = 0;

    pool->queue = (NxTask*)malloc(sizeof(NxTask) * queue_capacity);
    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (size_t i = 0; i < thread_count; i++) {
        pthread_create(&(pool->threads[i]), NULL, thread_routine, (void*)pool);
    }
    return pool;
}

int nx_threadpool_add_task(NxThreadPool *pool, void (*function)(void*), void *arg) {
    pthread_mutex_lock(&(pool->lock));
    if (pool->count == pool->queue_capacity || pool->shutdown) {
        pthread_mutex_unlock(&(pool->lock));
        return -1;
    }
    pool->queue[pool->tail].function = function;
    pool->queue[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % pool->queue_capacity;
    pool->count++;
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
    return 0;
}

void nx_threadpool_destroy(NxThreadPool *pool) {
    if (!pool) return;
    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    for (size_t i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool->queue);
    free(pool->threads);
    free(pool);
}
