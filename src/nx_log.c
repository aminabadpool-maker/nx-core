#include "nx_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static NxLogLevel current_level = NX_LOG_INFO;

void nx_log_set_level(NxLogLevel level) {
    current_level = level;
}

void nx_log_write(NxLogLevel level, const char *tag, const char *fmt, ...) {
    if (level < current_level) return;

    const char *level_strs[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(stderr, "[%02d:%02d:%02d] [%s] [%s] ", 
            t->tm_hour, t->tm_min, t->tm_sec, 
            level_strs[level], tag ? tag : "CORE");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}
