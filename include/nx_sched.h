#ifndef NX_SCHED_H
#define NX_SCHED_H

#include <stdbool.h>

typedef void (*NxTaskFunc)(void *arg);

// تعریف ساختار زمان‌بند به صورت مات (Opaque یا کامل)
typedef struct NxScheduler NxScheduler;

NxScheduler* nx_sched_init(int num_threads);
bool nx_sched_push(NxScheduler *sched, NxTaskFunc func, void *arg);
void nx_sched_wait(NxScheduler *sched);
void nx_sched_destroy(NxScheduler *sched);

#endif
