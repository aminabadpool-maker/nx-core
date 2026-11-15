#include "nx_kv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct NxKvStore {
    int fd;
    size_t capacity;
    NxKvEntry *entries;
    size_t map_size;
};

NxKvStore* nx_kv_open(const char *filepath, size_t max_entries) {
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd < 0) return NULL;

    size_t map_size = sizeof(NxKvEntry) * max_entries;
    
    // تنظیم اندازه فایل روی دیسک
    struct stat st;
    fstat(fd, &st);
    if ((size_t)st.st_size < map_size) {
        if (ftruncate(fd, map_size) < 0) {
            close(fd);
            return NULL;
        }
    }

    // مپ کردن فایل به حافظه (Memory-Mapped File)
    void *addr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    NxKvStore *store = (NxKvStore*)malloc(sizeof(NxKvStore));
    if (!store) {
        munmap(addr, map_size);
        close(fd);
        return NULL;
    }

    store->fd = fd;
    store->capacity = max_entries;
    store->entries = (NxKvEntry*)addr;
    store->map_size = map_size;

    return store;
}

bool nx_kv_set(NxKvStore *store, const char *key, const char *value) {
    if (!store || !key || !value) return false;

    // جستجو برای یافتن کلید موجود یا اسلات خالی
    for (size_t i = 0; i < store->capacity; i++) {
        if (!store->entries[i].active || strcmp(store->entries[i].key, key) == 0) {
            strncpy(store->entries[i].key, key, NX_KV_KEY_SIZE - 1);
            store->entries[i].key[NX_KV_KEY_SIZE - 1] = '\0';

            strncpy(store->entries[i].value, value, NX_KV_VAL_SIZE - 1);
            store->entries[i].value[NX_KV_VAL_SIZE - 1] = '\0';
            
            store->entries[i].active = true;
            
            // همگام‌سازی فوری با دیسک
            msync(&store->entries[i], sizeof(NxKvEntry), MS_SYNC);
            return true;
        }
    }
    return false; // دیتابیس پر است
}

bool nx_kv_get(NxKvStore *store, const char *key, char *value_out, size_t max_len) {
    if (!store || !key || !value_out) return false;

    for (size_t i = 0; i < store->capacity; i++) {
        if (store->entries[i].active && strcmp(store->entries[i].key, key) == 0) {
            strncpy(value_out, store->entries[i].value, max_len - 1);
            value_out[max_len - 1] = '\0';
            return true;
        }
    }
    return false; // کلید پیدا نشد
}

void nx_kv_close(NxKvStore *store) {
    if (!store) return;
    msync(store->entries, store->map_size, MS_SYNC);
    munmap(store->entries, store->map_size);
    close(store->fd);
    free(store);
}
