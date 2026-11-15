#include "nx_checksum.h"

uint32_t nx_checksum_crc32(const void *data, size_t length) {
    const uint8_t *p = (const uint8_t*)data;
    uint32_t crc = ~0U;
    for (size_t i = 0; i < length; i++) {
        crc ^= p[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (-(int)(crc & 1) & 0xEDB88320U);
        }
    }
    return ~crc;
}
