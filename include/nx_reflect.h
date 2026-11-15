#ifndef NX_REFLECT_H
#define NX_REFLECT_H

#include "nx_arena.h"

typedef enum {
    NX_TYPE_INT,
    NX_TYPE_STRING,
    NX_TYPE_STRUCT
} NxTypeKind;

typedef struct {
    const char *name;
    NxTypeKind kind;
    size_t offset;
    size_t size;
} NxFieldInfo;

#endif
