#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nx_arena.h"
#include "nx_log.h"
#include "nx_threadpool.h"
#include "nx_proto.h"
#include "nx_metrics.h"

// ساختار داده‌ای برای درخواست پرامپت LLM
typedef struct {
    int request_id;
    const char *prompt;
    NxArena *arena;
    NxMetrics *metrics;
} LlmTaskContext;

// تابع پردازش موازی پرامپت LLM توسط تردپول
void llm_worker_routine(void *arg) {
    LlmTaskContext *ctx = (LlmTaskContext*)arg;
    
    NX_LOG(NX_LOG_INFO, "[LLM AGENT] Processing Request #%d | Prompt: \"%s\"", ctx->request_id, ctx->prompt);

    // شبیه‌سازی تولید پاسخ LLM و تخصیص حافظه خطی در آرنا
    size_t response_capacity = 256;
    char *response_buffer = (char*)nx_arena_alloc(ctx->arena, response_capacity);

    if (response_buffer) {
        snprintf(response_buffer, response_capacity, 
                 "ACK: LLM Engine processed prompt [%s] with zero-copy arena allocation.", ctx->prompt);
        
        NX_LOG(NX_LOG_INFO, "[LLM AGENT] Response generated: %s", response_buffer);
        
        nx_metrics_inc_req(ctx->metrics);
        nx_metrics_add_bytes(ctx->metrics, strlen(response_buffer));
    } else {
        NX_LOG(NX_LOG_ERROR, "[LLM AGENT] Arena out of memory for Request #%d", ctx->request_id);
        nx_metrics_inc_fail(ctx->metrics);
    }
}

int main() {
    // ۱. راه‌اندازی لاگ سیستم
    nx_log_init(NULL);
    NX_LOG(NX_LOG_INFO, "=== INITIALIZING NX-CORE LLM AGENT PIPELINE ===");

    // ۲. مقداردهی متریک‌ها
    NxMetrics metrics;
    nx_metrics_init(&metrics);

    // ۳. ایجاد آرنا الکیتور اختصاصی برای ایجنت (۱ مگابایت حافظه ثابت)
    NxArena *arena = nx_arena_create(1024 * 1024);
    if (!arena) {
        NX_LOG(NX_LOG_ERROR, "Failed to create LLM Arena!");
        return 1;
    }
    NX_LOG(NX_LOG_INFO, "[+] LLM Arena allocated (1MB mmap block)");

    // ۴. ایجاد استخر رشته‌ها (۴ ترد فعال برای پردازش موازی پرامپت‌ها)
    NxThreadPool *pool = nx_threadpool_create(4, 32);
    NX_LOG(NX_LOG_INFO, "[+] LLM Worker ThreadPool initialized (4 threads)");

    // ۵. ارسال چندین پرامپت همزمان به ایجنت
    const char *prompts[] = {
        "Optimize Linux kernel scheduling in C",
        "Explain ARM64 NEON SIMD intrinsics",
        "Verify zero-copy memory buffers",
        "Calculate token throughput via ringbuf"
    };

    LlmTaskContext contexts[4];
    for (int i = 0; i < 4; i++) {
        contexts[i].request_id = i + 1;
        contexts[i].prompt = prompts[i];
        contexts[i].arena = arena;
        contexts[i].metrics = &metrics;

        nx_threadpool_add_task(pool, llm_worker_routine, &contexts[i]);
    }

    // مکث کوتاه برای اتمام تسک‌های تردپوول
    usleep(200000);

    // ۶. گزارش‌گیری متریک‌های نهایی
    NX_LOG(NX_LOG_INFO, "=== LLM PIPELINE METRICS ===");
    NX_LOG(NX_LOG_INFO, "Total Requests Processed: %ld", (long)metrics.total_requests);
    NX_LOG(NX_LOG_INFO, "Failed Requests: %ld", (long)metrics.failed_requests);
    NX_LOG(NX_LOG_INFO, "Total Bytes Processed: %ld bytes", (long)metrics.processed_bytes);

    // ۷. پاکسازی منابع
    nx_threadpool_destroy(pool);
    nx_arena_destroy(arena);
    NX_LOG(NX_LOG_INFO, "=== LLM AGENT PIPELINE SHUTDOWN CLEAN ===");

    nx_log_close();
    return 0;
}
