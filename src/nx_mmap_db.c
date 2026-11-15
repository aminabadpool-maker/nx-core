#include "nx_mmap_db.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

NxMmapDB* nx_mmap_db_open(const char *filepath, size_t size) {
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd < 0) return NULL;

    if (ftruncate(fd, size) < 0) {
        close(fd);
        return NULL;
    }

    char *map = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    NxMmapDB *db = (NxMmapDB*)malloc(sizeof(NxMmapDB));
    if (!db) {
        munmap(map, size);
        close(fd);
        return NULL;
    }

    db->fd = fd;
    db->size = size;
    db->data = map;
    return db;
}

bool nx_mmap_db_sync(NxMmapDB *db) {
    if (!db) return false;
    return msync(db->data, db->size, MS_SYNC) == 0;
}

void nx_mmap_db_close(NxMmapDB *db) {
    if (db) {
        munmap(db->data, db->size);
        close(db->fd);
        free(db);
    }
}
