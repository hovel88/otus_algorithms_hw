#pragma once

#include <iostream>
#include <string>
#include <chrono>

struct SortStats {
    bool                     processed{false};  // обработали или скипнули?
    std::string              name{};            // название алгоритма сортировки
    uint64_t                 size{0};           // размер массива, на котором производилась сортировка
    uint64_t                 operations{0};     // это будут подсчеты/копирования
    uint64_t                 memory{0};         // сколько дополнительной памяти использовано
    std::chrono::nanoseconds duration{};        // длительность выполнения
};
