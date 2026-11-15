#ifndef NX_EQUITY_H
#define NX_EQUITY_H

#include "nx_arena.h"
#include <stdbool.h>

// ساختار تنظیمات شمولیت و برابری برای فیلترینگ درخواست‌ها یا تخصیص منابع
typedef struct {
    bool enforce_fair_share;
    double max_weight_threshold;
} NxEquityPolicy;

// بررسی و اعمال قوانین برابری روی داده‌های ورودی ایجنت
bool nx_equity_validate_request(const NxEquityPolicy *policy, const char *payload);

#endif
