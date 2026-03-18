#pragma once

#include "ISort.h"

class MergeSort : public ISort
{
public:
    typedef enum {
        V1 = 0 // базовая реализация
    } version_t;

public:
    virtual ~MergeSort() = default;
    MergeSort() = delete;
    MergeSort(const MergeSort&) = delete;
    MergeSort(MergeSort&&) = delete;
    MergeSort& operator=(const MergeSort&) = delete;
    MergeSort& operator=(MergeSort&&) = delete;

    explicit MergeSort(version_t v)
    :   ISort( (v == version_t::V1) ? ("MergeSort_v1") : ("MergeSort_v???") ),
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
    std::unique_ptr<int[]> temp_{nullptr};

    bool sort_v1(int arr[], int n);

    void merge(int arr[], int left, int mid, int right);
    void sort_recursive(int arr[], int left, int right);
};
