#pragma once

#include <cstdint>

// итеративный алгоритм с маской
uint32_t popcount_v1(uint64_t num);

// алгоритм Кернигана
uint32_t popcount_v2(uint64_t num);

// алгоритм с кешированием по байтам
uint32_t popcount_v3(uint64_t num);

// алгоритм с кешированием по полубайтам
uint32_t popcount_v4(uint64_t num);
