#pragma once

#include <iostream>
#include <exception>
#include <string>

template <typename T>
class IArray
{
public:
    virtual ~IArray() = default;
    IArray() = delete;
    IArray(const IArray&) = delete;
    IArray(IArray&&) = delete;
    IArray& operator=(const IArray&) = delete;
    IArray& operator=(IArray&&) = delete;

    explicit IArray(const char* name)
    :   IArray(std::string(name))
    {}
    explicit IArray(const std::string& name)
    :   name_(name),
        realloc_cnt_(0)
    {}

    const std::string& name() const {
        return name_;
    }
    int number_of_reallocations() const {
        return realloc_cnt_;
    }
    void inc_number_of_reallocations() {
        ++realloc_cnt_;
    }

    virtual size_t capacity() const = 0;
    virtual size_t count() const = 0;

    virtual T get(size_t index, const T def) const = 0;
    virtual void add(const T& item, size_t index = 0) = 0;
    virtual T remove(size_t index) = 0;

private:
    const std::string name_{};
    int               realloc_cnt_{0};  // количество выделений памяти
};
