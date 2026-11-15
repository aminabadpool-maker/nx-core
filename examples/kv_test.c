#include "nx_kv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("[*] Starting nx_kv database test...\n");

    const char *db_path = "test_store.db";
    size_t max_entries = 100;

    // باز کردن یا ساخت دیتابیس روی دیسک با mmap
    NxKvStore *store = nx_kv_open(db_path, max_entries);
    if (!store) {
        fprintf(stderr, "[ERROR] Failed to open nx_kv store.\n");
        return 1;
    }
    printf("[OK] Database opened successfully with capacity for %zu entries.\n", max_entries);

    // نوشتن چند کلید و مقدار نمونه
    printf("[*] Writing key-value pairs...\n");
    nx_kv_set(store, "agent_name", "nx-core-agent");
    nx_kv_set(store, "status", "active");
    nx_kv_set(store, "version", "0.1.0-beta");
    nx_kv_set(store, "storage", "mmap-persistent");
    printf("[OK] Data written successfully to memory-mapped file.\n");

    // خواندن و تست مقادیر ذخیره شده
    char value_buffer[256];
    printf("[*] Reading data back from database...\n");

    const char *keys_to_check[] = {"agent_name", "status", "version", "storage"};
    for (int i = 0; i < 4; i++) {
        if (nx_kv_get(store, keys_to_check[i], value_buffer, sizeof(value_buffer))) {
            printf("  -> Key: '%s' => Value: '%s'\n", keys_to_check[i], value_buffer);
        } else {
            printf("  -> Key: '%s' => [NOT FOUND]\n", keys_to_check[i]);
        }
    }

    // بستن دیتابیس و اعمال msync نهایی روی دیسک
    nx_kv_close(store);
    printf("[OK] Database closed and synced to disk cleanly.\n");

    return 0;
}
