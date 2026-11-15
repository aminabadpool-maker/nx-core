#ifndef NX_RINGBUF_H
#define NX_RINGBUF_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    bool full;
} NxRingBuf;

NxRingBuf* nx_ringbuf_create(size_t capacity);
bool nx_ringbuf_push(NxRingBuf *rb, const char *data, size_t size);
bool nx_ringbuf_pop(NxRingBuf *rb, char *data, size_t *size);
void nx_ringbuf_destroy(NxRingBuf *rb);

#endif // NX_RINGBUF_H
