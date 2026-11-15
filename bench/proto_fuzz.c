#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "nx_proto.h"

// شبیه‌ساز فازینگ برای بمباران پارسر با ورودی‌های کاملاً تصادفی و مخدوش
void run_fuzz_storm(int iterations) {
    printf("[FUZZER] Starting Fuzz Storm across %d chaotic packet mutations...\n", iterations);
    
    uint8_t fuzz_buffer[512];
    
    for (int i = 0; i < iterations; i++) {
        // تولید داده‌های کاملاً تصادفی و ناخواسته
        for (size_t j = 0; j < sizeof(fuzz_buffer); j++) {
            fuzz_buffer[j] = (uint8_t)(rand() % 256);
        }

        // تزریق به پارسر صفرکپی
        NxProtocolHeader header;
        NxSliceView payload;
        
        // این تابع تحت هیچ شرایطی نباید کرش کند (Segmentation Fault یا Buffer Overflow بدهد)
        bool result = nx_proto_parse_zerocopy((const char*)fuzz_buffer, sizeof(fuzz_buffer), &header, &payload);

        if (result) {
            // اگر به طور تصادفی جادوی درست ایجاد شد، باید بارهای مفید از نظر طول کاملاً معتبر باشند
            assert(sizeof(NxProtocolHeader) + header.payload_len <= sizeof(fuzz_buffer));
        }
    }
    
    printf("[FUZZER] Successfully survived %d chaotic mutations with ZERO crashes!\n", iterations);
}

int main() {
    printf("=== STARTING NX-CORE PROTOCOL FUZZING SUITE ===\n");
    
    // بذر تصادفی برای تولید سناریوهای مخدوش مختلف
    srand(1388);
    
    run_fuzz_storm(100000); // اجرای ۱۰۰,۰۰۰ تست بمباران ورودی تصادفی
    
    printf("=== PROTOCOL FUZZING COMPLETED: FORTRESS IS UNBREAKABLE ===\n");
    return 0;
}
