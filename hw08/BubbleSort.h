#pragma once

#include "ISort.h"

class BubbleSort : public ISort
{
public:
    typedef enum {
        V1 = 0, // базовая реализация
        V2      // оптимизация с флагом отсортированности
    } version_t;

public:
    virtual ~BubbleSort() = default;
    BubbleSort() = delete;
    BubbleSort(const BubbleSort&) = delete;
    BubbleSort(BubbleSort&&) = delete;
    BubbleSort& operator=(const BubbleSort&) = delete;
    BubbleSort& operator=(BubbleSort&&) = delete;

    explicit BubbleSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("BubbleSort_v1") : ( (v == version_t::V2) ? ("BubbleSort_v2") : ("BubbleSort_v???") ) ),
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
        default:
            break;
        }
        return false;
    }

private:
    const version_t ver_{V1};

    bool sort_v1(int arr[], int n);
    bool sort_v2(int arr[], int n);
};
