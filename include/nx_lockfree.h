#ifndef NX_LOCKFREE_H
#define NX_LOCKFREE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char *buffer;
    size_t capacity;
    atomic_size_t head;
    atomic_size_t tail;
} NxLockFreeQueue;

NxLockFreeQueue* nx_lockfree_create(size_t capacity);
bool nx_lockfree_push(NxLockFreeQueue *q, const char *data, size_t size);
bool nx_lockfree_pop(NxLockFreeQueue *q, char *data, size_t *size);
void nx_lockfree_destroy(NxLockFreeQueue *q);

#endif
