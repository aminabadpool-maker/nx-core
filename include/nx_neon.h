#ifndef NX_NEON_H
#define NX_NEON_H

#include <stddef.h>

// هسته محاسباتی موازی با NEON برای جمع سریع بردارها
void nx_neon_vector_add(float *dst, const float *a, const float *b, size_t n);

#endif
