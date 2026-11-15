#ifndef NX_KV_H
#define NX_KV_H

#include <stdbool.h>
#include <stddef.h>

#define NX_KV_KEY_SIZE 64
#define NX_KV_VAL_SIZE 256

typedef struct NxKvEntry {
    char key[NX_KV_KEY_SIZE];
    char value[NX_KV_VAL_SIZE];
    bool active;
} NxKvEntry;

typedef struct NxKvStore NxKvStore;

// باز کردن یا ساخت پایگاه داده روی دیسک با قابلیت mmap
NxKvStore* nx_kv_open(const char *filepath, size_t max_entries);

// ذخیره یا به‌روزرسانی کلید-مقدار
bool nx_kv_set(NxKvStore *store, const char *key, const char *value);

// دریافت مقدار با کلید
bool nx_kv_get(NxKvStore *store, const char *key, char *value_out, size_t max_len);

// بستن و همگام‌سازی نهایی روی دیسک
void nx_kv_close(NxKvStore *store);

#endif
