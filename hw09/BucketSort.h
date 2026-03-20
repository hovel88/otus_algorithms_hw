#pragma once

#include <exception>
#include "SortStatistic.h"

class BucketSort
{
public:
    ~BucketSort() = default;
    BucketSort() = delete;
    BucketSort(const BucketSort&) = delete;
    BucketSort(BucketSort&&) = delete;
    BucketSort& operator=(const BucketSort&) = delete;
    BucketSort& operator=(BucketSort&&) = delete;

    explicit BucketSort(int num, int min, int max)
    :   name_(std::string("BucketSort:{buckets=") + std::to_string(num) + std::string("}")),
        num_buckets_(num),
        min_value_(min),
        max_value_(max)
    {
        if (num_buckets_ <= 0) throw std::invalid_argument("number of buckets should be greater than 0");
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
    const int num_buckets_{0};
    const int min_value_{0};
    const int max_value_{0};

    SortStats make_stats() const {
        SortStats st{};
        st.name = name();
        return st;
    }
};
