#ifndef NX_JSON_H
#define NX_JSON_H

#include "nx_arena.h"

// ساختار ساده برای نگهداری کلید و مقدار در پارسر JSON
typedef struct {
    const char *key;
    const char *value;
} NxJsonKV;

// پارس کردن ساده یک شیء JSON درون آرنا (Zero-copy)
int nx_json_parse_field(NxArena *arena, const char *json_str, const char *key, char *out_val, size_t max_len);

#endif
