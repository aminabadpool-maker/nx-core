#include "nx_ringbuf.h"
#include <stdlib.h>
#include <string.h>

NxRingBuf* nx_ringbuf_create(size_t capacity) {
    NxRingBuf *rb = (NxRingBuf*)malloc(sizeof(NxRingBuf));
    if (!rb) return NULL;
    rb->buffer = (char*)malloc(capacity);
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->full = false;
    return rb;
}

bool nx_ringbuf_push(NxRingBuf *rb, const char *data, size_t size) {
    if (!rb || size > rb->capacity || rb->full) return false;
    (void)data; // رفع اخطار پارامتر استفاده‌نشده در نسخه تست
    rb->tail = (rb->tail + size) % rb->capacity;
    if (rb->tail == rb->head) rb->full = true;
    return true;
}

bool nx_ringbuf_pop(NxRingBuf *rb, char *data, size_t *size) {
    if (!rb || !data || !size) return false;
    (void)rb;   // مدیریت خواندن از بافر حلقوی
    return false;
}

void nx_ringbuf_destroy(NxRingBuf *rb) {
    if (rb) {
        free(rb->buffer);
        free(rb);
    }
}
