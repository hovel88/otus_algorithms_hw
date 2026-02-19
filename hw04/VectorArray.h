#pragma once

#include "IArray.h"

template <typename T>
class VectorArray : public IArray<T>
{
public:
    virtual ~VectorArray() {
        delete[] values_;
        values_ = nullptr;
        count_ = 0;
        capacity_ = 0;
    }
    VectorArray()
    :   IArray<T>("Динамический массив +1"),
        capacity_(0),
        count_(0),
        values_(nullptr)
    {}
    VectorArray(const VectorArray&) = delete;
    VectorArray(VectorArray&&) = delete;
    VectorArray& operator=(const VectorArray&) = delete;
    VectorArray& operator=(VectorArray&&) = delete;

    explicit VectorArray(size_t cap)
    :   VectorArray()
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

        // хотим добавить элемент, но массив уже заполнен
        // тогда изменяем размер массива
        if (count_ == capacity()) {
            // политика увеличения размера: +1
            size_t new_capacity = capacity_ + 1;
            T* new_values = new T[new_capacity];

            for (size_t i = 0; i < count_; ++i) {
                new_values[i] = values_[i];
            }

            // сохраняем информацию о том, что пришлось сделать перевыделение памяти
            IArray<T>::inc_number_of_reallocations();

            delete[] values_;
            values_ = new_values;
            capacity_ = new_capacity;
        }

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
