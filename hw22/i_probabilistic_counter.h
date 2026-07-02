#pragma once

#include "common.h"

// обобщенный интерфейс для нескольких вероятностных алгоритмов
class IProbabilisticCounter {
public:
    virtual ~IProbabilisticCounter() = default;

    virtual std::string name() const = 0;           // названеи алгоритма
    virtual size_t get_memory_usage() const = 0;    // получаем занятую память
    virtual void print_stats() const = 0;           // дебажный вывод
    virtual void add(const std::string& key) = 0;   // добавляет новый элемент
};
