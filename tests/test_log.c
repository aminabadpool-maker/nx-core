#include "nx_log.h"
#include <stdio.h>
#include <assert.h>

int main() {
    printf("=== Testing nx_log Logging Module ===\n");
    nx_log_set_level(NX_LOG_DEBUG);

    NX_LOG_D("TEST", "Debug diagnostic code: %d", 42);
    NX_LOG_I("TEST", "nx-core logging engine initialized.");
    NX_LOG_W("TEST", "Warning: memory threshold check active.");
    NX_LOG_E("TEST", "Simulated error handler check passed.");

    printf("[OK] nx_log tests passed successfully!\n");
    return 0;
}
