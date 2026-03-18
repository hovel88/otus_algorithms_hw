#pragma once

#include "ISort.h"

class HeapSort : public ISort
{
public:
    typedef enum {
        V1 = 0 // базовая реализация
    } version_t;

public:
    virtual ~HeapSort() = default;
    HeapSort() = delete;
    HeapSort(const HeapSort&) = delete;
    HeapSort(HeapSort&&) = delete;
    HeapSort& operator=(const HeapSort&) = delete;
    HeapSort& operator=(HeapSort&&) = delete;

    explicit HeapSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("HeapSort_v1") : ("HeapSort_v???") ),
        ver_(v)
    {}

    bool sort(int arr[], int n = 0) override {
        switch (ver_) {
        case version_t::V1:
            return ISort::scoped_sort([this](int _arr[], int _n) -> bool {
                return sort_v1(_arr, _n);
            }, arr, n);
        default:
            break;
        }
        return false;
    }

private:
    const version_t ver_{V1};

    bool sort_v1(int arr[], int n);

    void _heapify(int arr[], int n, int i);
};
