#include "nx_metrics.h"

void nx_metrics_init(NxMetrics *m) {
    atomic_init(&(m->total_requests), 0);
    atomic_init(&(m->failed_requests), 0);
    atomic_init(&(m->processed_bytes), 0);
}

void nx_metrics_inc_req(NxMetrics *m) {
    atomic_fetch_add(&(m->total_requests), 1);
}

void nx_metrics_inc_fail(NxMetrics *m) {
    atomic_fetch_add(&(m->failed_requests), 1);
}

void nx_metrics_add_bytes(NxMetrics *m, long bytes) {
    atomic_fetch_add(&(m->processed_bytes), bytes);
}
