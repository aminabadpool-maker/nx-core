#include "nx_neon.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main() {
    printf("=== Testing ARM64 NEON SIMD Kernel (`nx_neon`) ===\n");

    const size_t size = 8;
    float a[8] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    float b[8] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f};
    float dst[8] = {0};

    nx_neon_vector_add(dst, a, b, size);

    for (size_t i = 0; i < size; i++) {
        printf("dst[%zu] = %.1f (Expected: %.1f)\n", i, dst[i], a[i] + b[i]);
        assert(dst[i] == a[i] + b[i]);
    }

    printf("[OK] ARM64 NEON Vector Add Test Passed Successfully!\n");
    return 0;
}
