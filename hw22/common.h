#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <unistd.h>

inline std::string extract_ip(const std::string& line) {
    // логи следующего вида
    //   kgtyk4.kj.yamagata-u.ac.jp - - [01/Aug/1995:00:00:22 -0400] "GET /images/WORLD-logosmall.gif HTTP/1.0" 304 0
    //   133.43.96.45 - - [01/Aug/1995:00:00:22 -0400] "GET /images/KSC-logosmall.gif HTTP/1.0" 200 1204
    // формат: "ip - - [date] \"request\" status size"
    // IP - это первый токен до пробела, там может быть как чистый IP, так и host
    size_t pos = line.find(' ');
    if (pos == std::string::npos) return "";
    return line.substr(0, pos);
}


inline size_t get_memory_usage() {
    size_t val_size     = 0;
    size_t val_resident = 0;
    size_t val_share    = 0;
    size_t val_text     = 0;
    size_t val_lib      = 0;
    size_t val_data     = 0;
    size_t val_dt       = 0;
    // извлечение статистики по используемой памяти в Linux
    std::ifstream statm("/proc/self/statm");
    if (statm >> val_size >> val_resident >> val_share >> val_text >> val_lib >> val_data >> val_dt) {
        return val_resident * sysconf(_SC_PAGESIZE);
    }
    return 0;
}

inline void print_memory_usage(const std::string& label) {
    size_t mem = get_memory_usage();
    if (mem > 0) {
        std::cout << label << ": " << std::fixed << std::setprecision(3)
                  << (mem / 1024.0 / 1024.0) << " MB" << std::endl;
    }
}


class Timer {
public:
    Timer()
    :   start_(std::chrono::high_resolution_clock::now())
    {}

    void reset() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    double elapsed_seconds() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start_).count();
    }

    void print(const std::string& label) const {
        std::cout << label << ": " << std::fixed << std::setprecision(3)
                  << elapsed_seconds() << " сек" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};


// для хеширования в вероятностных алгоритмах очень важно подобрать
// алгоритмы, которые сочетают в себе с одной стороны высокую скорость
// работы (т.к. используются на очень больших данных), так и выдающие очень
// качественный результат перемешивания бит, в том смысле, что даже изменение
// одного исходного бита должно привести к генерации такого хеша, который
// будет скорее похож на случайное число. также, желательно чтобы сами хеши
// были достаточно большими, чтобы минимизировались коллизии.
// В связи с этим, нам не подходят хеш-функции типа CRC32 (маленький и не достаточно
// быстрый) и SHA-1 (хороший крупный, но медленный) - у них и предназначение другое.
// нам нужно много независимых хеш-функций (например если посмотреть на алгоритм CMS),
// с числами большого размера (хотя бы 64-бит). в Интернете не рекомендуют использовать
// std::hash, и формировать несколько хешей, добавляя к ключу "соль", т.к. в итоге
// это медленно и гарантирует независимости результата.
// в Интернете было найдено несколько вариантов, которые могут подойти для этого.
// это алгоритмы некриптографических хеш-функций (т.е. не криптостойкие, но очень быстрые):
// - xxHash64 (считается самым лучшим и быстрым)
// - CityHash64 (тоже очень быстрый, но реализация достаточно крупная в функции)
// - MurmurHash3 (популярный, надежный, но реализация достаточно большая)
// - SplitMix64 (по факту это детерминированная очень быстрая псевдослучайная функция).
// я взял SplitMix64. в ней много "магических чисел", часть из которых была подобрана
// экспериментальным путем, а также есть число, являющееся представлением "золотого сечения".
// есть операции XOR и правые сдвиги для эффективного перемешивания битов. в общем, почему
// всё сделано именно так - я не понимаю, но зато реализация в коде очень простая, чем она
// меня и подкупила, и её оказалось быстро найти
inline uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

inline uint64_t hash64(const std::string& key, uint64_t seed = 0) {
    uint64_t hash = 0;
    for (char c : key) {
        hash = hash * 0x100000001b3ULL + static_cast<unsigned char>(c);
    }
    return splitmix64(hash + seed + 0x9e3779b97f4a7c15ULL);
}
