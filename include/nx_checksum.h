#ifndef NX_CHECKSUM_H
#define NX_CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

uint32_t nx_checksum_crc32(const void *data, size_t length);

#endif // NX_CHECKSUM_H
