#include "nx_simd.h"

#if defined(__aarch64__)
#include <arm_neon.h>
#elif defined(__x86_64__)
#include <immintrin.h>
#endif

void nx_simd_vector_add(const float *a, const float *b, float *out, size_t n) {
    size_t i = 0;
#if defined(__aarch64__)
    for (; i + 4 <= n; i += 4) {
        float32x4_t va = vld1q_f32(a + i);
        float32x4_t vb = vld1q_f32(b + i);
        float32x4_t vres = vaddq_f32(va, vb);
        vst1q_f32(out + i, vres);
    }
#elif defined(__x86_64__)
    for (; i + 8 <= n; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);
        __m256 vres = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(out + i, vres);
    }
#endif
    // باقیمانده‌ها
    for (; i < n; i++) {
        out[i] = a[i] + b[i];
    }
}
