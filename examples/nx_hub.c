#include "nx_arena.h"
#include "nx_json.h"
#include "nx_mcp.h"
#include "nx_sched.h"
#include "nx_equity.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void hub_worker_task(void *arg) {
    char *msg = (char*)arg;
    printf("[Hub Worker] Processing distributed agent task: %s\n", msg);
}

int main() {
    printf("=== Starting NxCore Comprehensive Agent Hub ===\n");

    // ۱. تخصیص حافظه با Arena
    NxArena *arena = nx_arena_create(4096);
    assert(arena != NULL);

    // ۲. تعریف سیاست‌های شمولیت و برابری
    NxEquityPolicy policy = { .enforce_fair_share = true, .max_weight_threshold = 1.0 };

    // ۳. شبیه‌سازی دریافت یک پکت JSON-RPC / MCP
    const char *incoming_json = "{\"jsonrpc\":\"2.0\",\"id\":100,\"method\":\"agent/process\",\"payload\":\"balanced_workload\"}";
    
    // ۴. اعتبارسنجی از طریق ماژول برابری
    bool is_fair = nx_equity_validate_request(&policy, incoming_json);
    assert(is_fair);
    printf("[OK] Equity & Inclusion policy validated incoming request successfully.\n");

    // ۵. پارس کردن پکت MCP و JSON
    NxMcpRequest req;
    int parse_res = nx_mcp_parse_request(arena, incoming_json, &req);
    assert(parse_res == 0);
    printf("[OK] MCP Parsed -> ID: %d, Method: %s\n", req.id, req.method);

    // ۶. ارسال به زمان‌بند قفل‌آزاد (nx_sched) برای پردازش موازی
    NxScheduler *sched = nx_sched_init(2);
    assert(sched != NULL);

    bool pushed = nx_sched_push(sched, hub_worker_task, (void*)req.method);
    assert(pushed);

    nx_sched_wait(sched);
    nx_sched_destroy(sched);
    nx_arena_free(arena);

    printf("=== All Hub Integrations Passed Successfully! ===\n");
    return 0;
}
