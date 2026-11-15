#ifndef NX_MMAP_DB_H
#define NX_MMAP_DB_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int fd;
    size_t size;
    char *data;
} NxMmapDB;

NxMmapDB* nx_mmap_db_open(const char *filepath, size_t size);
bool nx_mmap_db_sync(NxMmapDB *db);
void nx_mmap_db_close(NxMmapDB *db);

#endif
