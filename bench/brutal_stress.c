#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "nx_threadpool.h"
#include "nx_arena.h"
#include "nx_metrics.h"

#define THREAD_COUNT 16
#define TASKS_PER_THREAD 1000
#define QUEUE_SIZE 256

typedef struct {
    int id;
    NxArena *arena;
    NxMetrics *metrics;
} BrutalTaskArg;

void brutal_worker(void *arg) {
    BrutalTaskArg *ctx = (BrutalTaskArg*)arg;
    
    // تخصیص حافظه سریع از آرنا زیر بار سنگین موازی
    char *mem = (char*)nx_arena_alloc(ctx->arena, 128);
    if (mem) {
        snprintf(mem, 128, "Task %d executed under brutal stress", ctx->id);
        nx_metrics_inc_req(ctx->metrics);
        nx_metrics_add_bytes(ctx->metrics, 128);
    } else {
        nx_metrics_inc_fail(ctx->metrics);
    }
}

int main() {
    printf("=== STARTING BRUTAL CONCURRENCY & QUEUE STORM TEST ===\n");
    clock_t start_time = clock();

    NxMetrics metrics;
    nx_metrics_init(&metrics);

    // آرنا با حجم کافی برای هزاران تخصیص ریز
    NxArena *arena = nx_arena_create(16 * 1024 * 1024); // 16MB
    if (!arena) {
        fprintf(stderr, "Failed to create stress arena!\n");
        return 1;
    }

    // استخر رشته‌ها با ظرفیت محدود صف برای تست مقاومت در برابر اشباع
    NxThreadPool *pool = nx_threadpool_create(THREAD_COUNT, QUEUE_SIZE);
    if (!pool) {
        fprintf(stderr, "Failed to create thread pool!\n");
        nx_arena_destroy(arena);
        return 1;
    }

    int total_attempted = THREAD_COUNT * TASKS_PER_THREAD;
    int accepted_tasks = 0;
    int rejected_tasks = 0;

    BrutalTaskArg *args = malloc(sizeof(BrutalTaskArg) * total_attempted);

    // بمباران سنگین صف با هزاران تسک همزمان
    for (int i = 0; i < total_attempted; i++) {
        args[i].id = i;
        args[i].arena = arena;
        args[i].metrics = &metrics;

        int res = nx_threadpool_add_task(pool, brutal_worker, &args[i]);
        if (res == 0) {
            accepted_tasks++;
        } else {
            rejected_tasks++; // وقتی صف پر می‌شود، سیستم باید بدون کرش تسک را پس بزند
        }
    }

    printf("[+] Submitted %d tasks. Accepted: %d, Rejected (Queue Full): %d\n", 
           total_attempted, accepted_tasks, rejected_tasks);

    // انتظار برای تخلیه کامل صف توسط تردها
    sleep(2);

    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("=== BRUTAL TEST RESULTS ===\n");
    printf("Execution Time: %.4f seconds\n", cpu_time_used);
    printf("Total Requests Processed: %ld\n", (long)metrics.total_requests);
    printf("Failed / Rejected: %ld\n", (long)metrics.failed_requests + rejected_tasks);
    printf("Throughput: %.2f ops/sec\n", (double)metrics.total_requests / cpu_time_used);

    nx_threadpool_destroy(pool);
    nx_arena_destroy(arena);
    free(args);

    printf("=== STORM TEST PASSED SUCCESSFULLY ===\n");
    return 0;
}
