#ifndef NX_LOG_H
#define NX_LOG_H

typedef enum {
    NX_LOG_DEBUG,
    NX_LOG_INFO,
    NX_LOG_WARN,
    NX_LOG_ERROR
} NxLogLevel;

void nx_log_init(const char *filepath);
void nx_log_write(NxLogLevel level, const char *file, int line, const char *fmt, ...);
void nx_log_close(void);

#define NX_LOG(level, ...) nx_log_write(level, __FILE__, __LINE__, __VA_ARGS__)

#endif // NX_LOG_H
