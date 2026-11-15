#include "nx_lockfree.h"
#include <stdlib.h>
#include <string.h>

NxLockFreeQueue* nx_lockfree_create(size_t capacity) {
    NxLockFreeQueue *q = (NxLockFreeQueue*)malloc(sizeof(NxLockFreeQueue));
    if (!q) return NULL;
    q->buffer = (char*)malloc(capacity);
    if (!q->buffer) {
        free(q);
        return NULL;
    }
    q->capacity = capacity;
    atomic_init(&q->head, 0);
    atomic_init(&q->tail, 0);
    return q;
}

bool nx_lockfree_push(NxLockFreeQueue *q, const char *data, size_t size) {
    size_t head = atomic_load_explicit(&q->head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&q->tail, memory_order_acquire);
    
    if ((tail + size + 1) % q->capacity == head) return false; // صف پر است

    for (size_t i = 0; i < size; i++) {
        q->buffer[(tail + i) % q->capacity] = data[i];
    }
    atomic_store_explicit(&q->tail, (tail + size) % q->capacity, memory_order_release);
    return true;
}

bool nx_lockfree_pop(NxLockFreeQueue *q, char *data, size_t *size) {
    size_t head = atomic_load_explicit(&q->head, memory_order_acquire);
    size_t tail = atomic_load_explicit(&q->tail, memory_order_relaxed);

    if (head == tail) return false; // صف خالی است

    // پیاده‌سازی نمونه SPSC برای خواندن بایت‌ها
    if (*size > 0) {
        data[0] = q->buffer[head];
        *size = 1;
        atomic_store_explicit(&q->head, (head + 1) % q->capacity, memory_order_release);
        return true;
    }
    return false;
}

void nx_lockfree_destroy(NxLockFreeQueue *q) {
    if (q) {
        free(q->buffer);
        free(q);
    }
}
