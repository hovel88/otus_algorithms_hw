#pragma once

#include "ISort.h"

class QuickSort : public ISort
{
public:
    typedef enum {
        V1 = 0 // базовая реализация
    } version_t;

public:
    virtual ~QuickSort() = default;
    QuickSort() = delete;
    QuickSort(const QuickSort&) = delete;
    QuickSort(QuickSort&&) = delete;
    QuickSort& operator=(const QuickSort&) = delete;
    QuickSort& operator=(QuickSort&&) = delete;

    explicit QuickSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("QuickSort_v1") : ("QuickSort_v???") ),
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

    int split(int arr[], int low, int high);
    void sort_recursive(int arr[], int low, int high);
};
