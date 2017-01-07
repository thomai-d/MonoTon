#pragma once
#include "../Hardware/HAL.h"

#define MIN(a, b) (a > b ? a : b)
#define SUB(a, b) (b >= a ? 0 : a - b)
#define DELTA(a, b) (a > b ? a - b : b - a)
#define TIME_DELTA_MS(x, y) (x > y ? (uint32_t)x - (uint32_t)y : (uint32_t)y - (uint32_t)x)

