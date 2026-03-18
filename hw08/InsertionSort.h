#pragma once

#include "ISort.h"

class InsertionSort : public ISort
{
public:
    typedef enum {
        V1 = 0, // базовая реализация
        V2,     // оптимизация со сдвигом, вместо последовательного обмена
        V3      // оптимизация с бинарным поиском места вставки
    } version_t;

public:
    virtual ~InsertionSort() = default;
    InsertionSort() = delete;
    InsertionSort(const InsertionSort&) = delete;
    InsertionSort(InsertionSort&&) = delete;
    InsertionSort& operator=(const InsertionSort&) = delete;
    InsertionSort& operator=(InsertionSort&&) = delete;

    explicit InsertionSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("InsertionSort_v1") : ( (v == version_t::V2) ? ("InsertionSort_v2") : ( (v == version_t::V3) ? ("InsertionSort_v3") : ("InsertionSort_v???") ) ) ),
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

    int binary_search_pos(int arr[], int left, int right, int value);
};
