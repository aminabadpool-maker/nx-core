#ifndef NX_LOG_H
#define NX_LOG_H

typedef enum {
    NX_LOG_DEBUG = 0,
    NX_LOG_INFO,
    NX_LOG_WARN,
    NX_LOG_ERROR
} NxLogLevel;

void nx_log_set_level(NxLogLevel level);
void nx_log_write(NxLogLevel level, const char *tag, const char *fmt, ...);

#define NX_LOG_D(tag, ...) nx_log_write(NX_LOG_DEBUG, tag, __VA_ARGS__)
#define NX_LOG_I(tag, ...) nx_log_write(NX_LOG_INFO, tag, __VA_ARGS__)
#define NX_LOG_W(tag, ...) nx_log_write(NX_LOG_WARN, tag, __VA_ARGS__)
#define NX_LOG_E(tag, ...) nx_log_write(NX_LOG_ERROR, tag, __VA_ARGS__)

#endif
