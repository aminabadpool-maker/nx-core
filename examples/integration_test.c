#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nx_arena.h"
#include "nx_log.h"
#include "nx_ringbuf.h"
#include "nx_signal.h"
#include "nx_checksum.h"
#include "nx_threadpool.h"
#include "nx_hash.h"
#include "nx_strview.h"
#include "nx_proto.h"
#include "nx_metrics.h"

// تابع تسک برای استخر رشته‌ها
void worker_task(void *arg) {
    char *msg = (char*)arg;
    NX_LOG(NX_LOG_INFO, "Worker thread processing task payload: %s", msg);
}

int main() {
    // ۱. راه‌اندازی سیستم لاگ
    nx_log_init(NULL);
    NX_LOG(NX_LOG_INFO, "=== STARTING NX-CORE FULL INTEGRATION TEST ===");

    // ۲. مقداردهی متریک‌ها
    NxMetrics metrics;
    nx_metrics_init(&metrics);

    // ۳. ایجاد آرنا الکیتور (مدیریت حافظه بدون قطعه‌قطعه شدن)
    NxArena *arena = nx_arena_create(2 * 1024 * 1024); // 2MB
    if (!arena) {
        NX_LOG(NX_LOG_ERROR, "Failed to create Arena Allocator!");
        return 1;
    }
    NX_LOG(NX_LOG_INFO, "[+] Arena Allocator initialized (2MB)");

    // ۴. ایجاد هش‌مپ حافظه
    NxHashMap *map = nx_hash_create(64);
    nx_hash_set(map, "agent_status", "ACTIVE");
    nx_hash_set(map, "core_version", "1.0.0");
    NX_LOG(NX_LOG_INFO, "[+] Hash Map initialized & populated");

    // ۵. تست String View (صفر-کپی)
    const char *raw_txt = "nx-core-agent-stream";
    NxStrView view = nx_strview_from(raw_txt, strlen(raw_txt));
    NX_LOG(NX_LOG_INFO, "[+] StringView created. Length: %zu", view.length);

    // ۶. تست پروتکل باینری و چک‌سام (CRC32)
    char payload[] = "Hello NX-Core Agent Payload";
    NxHeader hdr;
    hdr.magic = 0x4E584352; // "NXCR"
    hdr.msg_type = 1;
    hdr.payload_len = strlen(payload);

    char proto_buf[512];
    size_t packet_size = nx_proto_serialize(&hdr, payload, proto_buf, sizeof(proto_buf));
    NX_LOG(NX_LOG_INFO, "[+] Binary Protocol serialized. Packet Size: %zu bytes, CRC32: %u", 
           packet_size, hdr.checksum);

    // ۷. راه‌اندازی Thread Pool و اجرای تسک‌های موازی
    NxThreadPool *pool = nx_threadpool_create(4, 16);
    NX_LOG(NX_LOG_INFO, "[+] Thread Pool created (4 threads, queue size 16)");

    // تخصیص پیام از طریق آرنا برای تسک‌ها
    char *task_msg1 = (char*)nx_arena_alloc(arena, 64);
    snprintf(task_msg1, 64, "Task_Alpha_Pipeline");
    nx_threadpool_add_task(pool, worker_task, task_msg1);

    char *task_msg2 = (char*)nx_arena_alloc(arena, 64);
    snprintf(task_msg2, 64, "Task_Beta_Pipeline");
    nx_threadpool_add_task(pool, worker_task, task_msg2);

    // شبیه‌سازی ثبت متریک‌ها
    nx_metrics_inc_req(&metrics);
    nx_metrics_add_bytes(&metrics, packet_size);

    // مکث کوتاه برای اجرای تسک‌های تردپول
    usleep(100000);

    // ۸. پاکسازی و تخریب منظم منابع
    nx_threadpool_destroy(pool);
    nx_hash_destroy(map);
    nx_arena_destroy(arena);

    NX_LOG(NX_LOG_INFO, "=== INTEGRATION TEST COMPLETED SUCCESSFULLY ===");
    nx_log_close();
    return 0;
}
