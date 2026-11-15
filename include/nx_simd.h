#ifndef NX_SIMD_H
#define NX_SIMD_H

#include <stddef.h>
#include <stdint.h>

// جمع سریع برداری آرایه‌های بایت یا ممیز شناور
void nx_simd_vector_add(const float *a, const float *b, float *out, size_t n);

#endif
