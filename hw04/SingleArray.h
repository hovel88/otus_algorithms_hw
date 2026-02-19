#pragma once

#include "IArray.h"

template <typename T>
class SingleArray : public IArray<T>
{
public:
    virtual ~SingleArray() {
        delete[] values_;
        values_ = nullptr;
        count_ = 0;
        capacity_ = 0;
    }
    SingleArray()
    :   IArray<T>("Массив фиксированного размера"),
        capacity_(0),
        count_(0),
        values_(nullptr)
    {}
    SingleArray(const SingleArray&) = delete;
    SingleArray(SingleArray&&) = delete;
    SingleArray& operator=(const SingleArray&) = delete;
    SingleArray& operator=(SingleArray&&) = delete;

    explicit SingleArray(size_t cap)
    :   SingleArray()
    {
        capacity_ = cap;
        values_ = new T[cap];
    }

    size_t capacity() const override {
        return capacity_;
    }
    size_t count() const override {
        return count_;
    }

    T get(size_t index, const T def) const override {
        if (index >= capacity()) return def;

        return values_[index];
    }

    void add(const T& item, size_t index = 0) override {
        if (index > count_) throw std::out_of_range("index out of range");

        // нет места
        if (count_ == capacity()) throw std::runtime_error("no space left");

        // сдвигаем элементы вправо
        for (size_t i = count_; i > index; --i) {
            values_[i] = values_[i - 1];
        }
        values_[index] = item;
        ++count_;
    }

    T remove(size_t index) {
        if (index > count_) throw std::out_of_range("index out of range");

        T removed = values_[index];
        // сдвигаем элементы влево
        for (size_t i = index; i < (count_ - 1); ++i) {
            values_[i] = values_[i + 1];
        }
        --count_;
        return removed;
    }

private:
    size_t capacity_{0};     // допустимая ёмкость массива
    size_t count_{0};        // количество элементов в массиве
    T*     values_{nullptr}; // динамические данные лежат тут
};
