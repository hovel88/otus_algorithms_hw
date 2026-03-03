#pragma once

#include "ISort.h"

class ShellSort : public ISort
{
public:
    typedef enum {
        V1 = 0, // базовая реализация
        V2,     // оптимизация со сдвигом, вместо последовательного обмена
        V3      // оптимизация с бинарным поиском места вставки
    } version_t;

public:
    virtual ~ShellSort() = default;
    ShellSort() = delete;
    ShellSort(const ShellSort&) = delete;
    ShellSort(ShellSort&&) = delete;
    ShellSort& operator=(const ShellSort&) = delete;
    ShellSort& operator=(ShellSort&&) = delete;

    explicit ShellSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("ShellSort_v1") : ( (v == version_t::V2) ? ("ShellSort_v2") : ( (v == version_t::V3) ? ("ShellSort_v3") : ("ShellSort_v???") ) ) ),
        ver_(v)
    {}

    bool sort(int arr[], int n = 0) override {
        switch (ver_) {
        case version_t::V1:
            return ISort::scoped_sort([this](int _arr[], int _n) -> bool {
                return sort_v1(_arr, _n);
            }, arr, n);
        case version_t::V2:
            return ISort::scoped_sort([this](int _arr[], int _n) -> bool {
                return sort_v2(_arr, _n);
            }, arr, n);
        case version_t::V3:
            return ISort::scoped_sort([this](int _arr[], int _n) -> bool {
                return sort_v3(_arr, _n);
            }, arr, n);
        default:
            break;
        }
        return false;
    }

private:
    const version_t ver_{V1};

    bool sort_v1(int arr[], int n);
    bool sort_v2(int arr[], int n);
    bool sort_v3(int arr[], int n);
};
