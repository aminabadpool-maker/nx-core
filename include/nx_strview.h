#ifndef NX_STRVIEW_H
#define NX_STRVIEW_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const char *data;
    size_t length;
} NxStrView;

NxStrView nx_strview_from(const char *str, size_t length);
bool nx_strview_equals(NxStrView a, NxStrView b);

#endif // NX_STRVIEW_H
