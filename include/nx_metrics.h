#ifndef NX_METRICS_H
#define NX_METRICS_H

#include <stdint.h>
#include <stdatomic.h>

typedef struct {
    atomic_long total_requests;
    atomic_long failed_requests;
    atomic_long processed_bytes;
} NxMetrics;

void nx_metrics_init(NxMetrics *m);
void nx_metrics_inc_req(NxMetrics *m);
void nx_metrics_inc_fail(NxMetrics *m);
void nx_metrics_add_bytes(NxMetrics *m, long bytes);

#endif // NX_METRICS_H
