#pragma once

#include <iostream>
#include <exception>
#include <string>

template <typename T>
class SpaceArray
{
private:
    struct Cell {
        size_t col_{0};
        T      value_;
        Cell* next_{nullptr};

        Cell(size_t c, const T& v, Cell* n = nullptr) 
        :   col_(c),
            value_(v),
            next_(n)
        {}
    };

    struct Row {
        size_t count_{0};
        Cell*  head_{nullptr};

        Row()
        :   count_(0),
            head_(nullptr)
        {}
        ~Row() {
            while (head_ != nullptr) {
                Cell* c = head_;
                head_ = head_->next_;
                delete c;
            }
        }
    };

public:
    ~SpaceArray() {
        delete[] non_defaults_;
    }
    SpaceArray() = delete;
    SpaceArray(const SpaceArray&) = delete;
    SpaceArray(SpaceArray&&) = delete;
    SpaceArray& operator=(const SpaceArray&) = delete;
    SpaceArray& operator=(SpaceArray&&) = delete;

    explicit SpaceArray(size_t r, size_t c, const T& def = T())
    :   rows_(r),
        cols_(c),
        default_value_(def)
    {
        non_defaults_ = new Row[r];
    }

    void set(size_t row, size_t col, const T& value) {
        if (row >= rows_ || col >= cols_) throw std::out_of_range("index out of range");

        Cell* prev = nullptr;
        Cell* curr = non_defaults_[row].head_;

        // находим место для вставки
        while (curr != nullptr && curr->col_ < col) {
            prev = curr;
            curr = curr->next_;
        }

        if (curr != nullptr && curr->col_ == col) {
            // ячейка уже была задана ранее,
            // поэтому проверяем каким значением мы ее устанавливаем.
            // если это дефолтное значение, то удаляем ячейку,
            // иначе обновляем её недефолтное значение
            if (value == default_value_) {
                if (prev == nullptr) {
                    non_defaults_[row].head_ = curr->next_;
                } else {
                    prev->next_ = curr->next_;
                }
                delete curr;
                --non_defaults_[row].count_;
            } else {
                curr->value_ = value;
            }
        } else {
            // ячейки в массиве нет,
            // но создаем только если пытаемся установить недефолтное значение
            if (value != default_value_) {
                Cell* new_cell = new Cell(col, value, curr);
                if (prev == nullptr) {
                    non_defaults_[row].head_ = new_cell;
                } else {
                    prev->next_ = new_cell;
                }
                ++non_defaults_[row].count_;
            }
        }
    }

    T get(size_t row, size_t col) const {
        if (row >= rows_ || col >= cols_) throw std::out_of_range("index out of range");

        Cell* cell = non_defaults_[row].head_;
        while (cell != nullptr && cell->col_ < col) {
            cell = cell->next_;
        }
        if (cell != nullptr && cell->col_ == col) {
            return cell->value_;
        }
        return default_value_;
    }

    size_t size() const {
        return rows_ * cols_;
    }
    size_t non_default_count() const {
        size_t count = 0;
        for (size_t i = 0; i < rows_; ++i) {
            count += non_defaults_[i].count_;
        }
        return count;
    }
    double density() const {
        return (non_default_count() * 100.0 / (rows_ * cols_));
    }

    void print_stats() const {
        std::cout << "SpaceArray: " << rows_ << "x" << cols_
                  << ", default-value=" << default_value_
                  << ", non-default-count=" << non_default_count()
                  << ", density=" << density() << "%" << std::endl;
    }

    void print_debug() const {
        for (size_t r = 0; r < rows_; ++r) {
            for (size_t c = 0; c < cols_; ++c) {
                std::cout << get(r, c) << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    size_t rows_{0};
    size_t cols_{0};
    T      default_value_;
    Row*   non_defaults_{nullptr}; // массив строк (списков), в ячейках которого хранятся недефолтные значения
};
