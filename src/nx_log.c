#include "nx_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

static FILE *log_file = NULL;
static const char *level_strings[] = { "DEBUG", "INFO", "WARN", "ERROR" };

void nx_log_init(const char *filepath) {
    if (filepath) {
        log_file = fopen(filepath, "a");
    } else {
        log_file = stdout;
    }
}

void nx_log_write(NxLogLevel level, const char *file, int line, const char *fmt, ...) {
    if (!log_file) log_file = stdout;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(log_file, "[%02d:%02d:%02d] [%s] (%s:%d) ", 
            t->tm_hour, t->tm_min, t->tm_sec, level_strings[level], file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
}

void nx_log_close(void) {
    if (log_file && log_file != stdout) {
        fclose(log_file);
        log_file = NULL;
    }
}
