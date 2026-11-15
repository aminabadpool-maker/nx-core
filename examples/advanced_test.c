#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "nx_lockfree.h"
#include "nx_simd.h"
#include "nx_mmap_db.h"
#include "nx_epoll.h"

int main() {
    printf("=== STARTING NX-CORE ADVANCED MODULES TEST ===\n");

    // ۱. تست ماژول SIMD
    float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    float out[4] = {0};
    
    nx_simd_vector_add(a, b, out, 4);
    printf("[SIMD] Vector Add Result: [%.1f, %.1f, %.1f, %.1f]\n", out[0], out[1], out[2], out[3]);
    assert(out[0] == 11.0f && out[3] == 44.0f);

    // ۲. تست صف قفل-آزاد
    NxLockFreeQueue *lf_q = nx_lockfree_create(1024);
    assert(lf_q != NULL);
    bool pushed = nx_lockfree_push(lf_q, "NX", 2);
    printf("[LOCKFREE] Push status: %s\n", pushed ? "SUCCESS" : "FAILED");
    assert(pushed);
    nx_lockfree_destroy(lf_q);

    // ۳. تست پایگاه داده mmap
    const char *db_file = "test_mmap.db";
    NxMmapDB *db = nx_mmap_db_open(db_file, 4096);
    assert(db != NULL);
    snprintf(db->data, 64, "Hello from nx-core zero-copy mmap db!");
    nx_mmap_db_sync(db);
    printf("[MMAP DB] Written & Synced data: %s\n", db->data);
    nx_mmap_db_close(db);
    unlink(db_file);

    // ۴. تست ری‌اکتور Epoll
    NxEpollReactor *reactor = nx_epoll_create(10);
    assert(reactor != NULL);
    printf("[EPOLL] Reactor created successfully on fd: %d\n", reactor->epoll_fd);
    nx_epoll_destroy(reactor);

    printf("=== ALL ADVANCED MODULES PASSED SUCCESSFULLY! ===\n");
    return 0;
}
