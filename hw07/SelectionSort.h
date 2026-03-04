#pragma once

#include "ISort.h"

class SelectionSort : public ISort
{
public:
    typedef enum {
        V1 = 0 // базовая реализация
    } version_t;

public:
    virtual ~SelectionSort() = default;
    SelectionSort() = delete;
    SelectionSort(const SelectionSort&) = delete;
    SelectionSort(SelectionSort&&) = delete;
    SelectionSort& operator=(const SelectionSort&) = delete;
    SelectionSort& operator=(SelectionSort&&) = delete;

    explicit SelectionSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("SelectionSort_v1") : ("SelectionSort_v???") ),
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
};
