#pragma once

#include <exception>
#include "SortStatistic.h"

class RadixSort
{
public:
    ~RadixSort() = default;
    RadixSort() = delete;
    RadixSort(const RadixSort&) = delete;
    RadixSort(RadixSort&&) = delete;
    RadixSort& operator=(const RadixSort&) = delete;
    RadixSort& operator=(RadixSort&&) = delete;

    explicit RadixSort(int b, int max)
    :   name_(std::string("RadixSort:{base=") + std::to_string(b) + std::string("}")),
        base_(b),
        max_value_(max)
    {
        if (base_ <= 0) throw std::invalid_argument("value of base should be greater than 0");
        if (max_value_ < 0) throw std::invalid_argument("maximum possible value should be positive");
    }

    const std::string& name() const {
        return name_;
    }

    SortStats sort(int arr[], int n = 0);
    SortStats sort(const std::string& in_file, const std::string& out_file);

private:
    const std::string name_{};
    const int base_{0};
    const int max_value_{0};

    SortStats make_stats() const {
        SortStats st{};
        st.name = name();
        return st;
    }
    SortStats _sort_iteration(const std::string& in_file, const std::string& out_file, int exp);
};
