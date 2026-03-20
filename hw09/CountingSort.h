#pragma once

#include <exception>
#include "SortStatistic.h"

class CountingSort
{
public:
    ~CountingSort() = default;
    CountingSort() = delete;
    CountingSort(const CountingSort&) = delete;
    CountingSort(CountingSort&&) = delete;
    CountingSort& operator=(const CountingSort&) = delete;
    CountingSort& operator=(CountingSort&&) = delete;

    explicit CountingSort(int min, int max)
    :   name_(std::string("CountingSort:{") + std::to_string(min) + std::string("..") + std::to_string(max) + std::string("}")),
        min_value_(min),
        max_value_(max)
    {
        if (min_value_ < 0) throw std::invalid_argument("left edge value of range should be positive");
        if (max_value_ < 0) throw std::invalid_argument("right edge value of range should be positive");
        if (max_value_ < min_value_) throw std::length_error("left edge value of range should be greater than right edge value of range");
    }

    const std::string& name() const {
        return name_;
    }

    SortStats sort(int arr[], int n = 0);
    SortStats sort(const std::string& in_file, const std::string& out_file);

private:
    const std::string name_{};
    const int min_value_{0};
    const int max_value_{0};

    SortStats make_stats() const {
        SortStats st{};
        st.name = name();
        return st;
    }
};
