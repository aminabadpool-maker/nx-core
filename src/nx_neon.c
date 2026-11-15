#include "nx_neon.h"

#ifdef __aarch64__
#include <arm_neon.h>
#endif

void nx_neon_vector_add(float *dst, const float *a, const float *b, size_t n) {
#ifdef __aarch64__
    size_t i = 0;
    // پردازش هم‌زمان ۴ عدد اعشاری (float32) در هر ثبات ۱۲۸ بیتی NEON
    for (; i + 3 < n; i += 4) {
        float32x4_t va = vld1q_f32(a + i);
        float32x4_t vb = vld1q_f32(b + i);
        float32x4_t vres = vaddq_f32(va, vb);
        vst1q_f32(dst + i, vres);
    }
    // پردازش باقی‌مانده عناصر (اگر تعداد کل بردار ضریبی از ۴ نباشد)
    for (; i < n; i++) {
        dst[i] = a[i] + b[i];
    }
#else
    // Fallback برای معماری‌های غیر ARM64
    for (size_t i = 0; i < n; i++) {
        dst[i] = a[i] + b[i];
    }
#endif
}
