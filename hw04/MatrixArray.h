#pragma once

#include "IArray.h"

template <typename T>
class MatrixArray : public IArray<T>
{
private:
    static const size_t BLOCK_SIZE = 10;

    struct Block {
        T* values_{nullptr};
        size_t size_{0};

        ~Block() {
            delete[] values_;
        }
        Block()
        :   values_(new T[BLOCK_SIZE]),
            size_(0)
        {}
        Block(const Block&) = delete;
        Block(Block&&) = delete;
        Block& operator=(const Block&) = delete;
        Block& operator=(Block&&) = delete;
    };

public:
    virtual ~MatrixArray() {
        for (size_t i = 0; i < blocks_count_; ++i) {
            delete blocks_[i];
        }
        delete[] blocks_;
        blocks_ = nullptr;
        blocks_count_ = 0;
        count_ = 0;
    }
    MatrixArray()
    :   IArray<T>("Свободный массив (по 10)"),
        count_(0),
        blocks_count_(0),
        blocks_(nullptr)
    {}
    MatrixArray(const MatrixArray&) = delete;
    MatrixArray(MatrixArray&&) = delete;
    MatrixArray& operator=(const MatrixArray&) = delete;
    MatrixArray& operator=(MatrixArray&&) = delete;

    explicit MatrixArray(size_t cap)
    :   MatrixArray()
    {
        if (cap <= BLOCK_SIZE) {
            blocks_count_ = 1;
        } else {
            blocks_count_ = (cap + (BLOCK_SIZE * 0.5)) / BLOCK_SIZE;
        }
        blocks_ = new Block*[blocks_count_];
        for (size_t i = 0; i < blocks_count_; ++i) {
            blocks_[i] = new Block();
        }
    }

    size_t capacity() const override {
        return blocks_count_ * BLOCK_SIZE;
    }
    size_t count() const override {
        return count_;
    }

    T get(size_t index, const T def) const override {
        if (index >= capacity()) return def;

        size_t border = index / BLOCK_SIZE;
        size_t pos_accum = 0;
        for (size_t i = 0; i <= border; ++i) {
            size_t b_size = blocks_[i]->size_;
            if (index < (pos_accum + b_size)) {
                size_t b_index = i;
                size_t v_index = index - pos_accum;
                return blocks_[b_index]->values_[v_index];
            }
            pos_accum += b_size;
        }
        throw std::runtime_error("can't find position");
    }

    void add(const T& item, size_t index = 0) override {
        if (index > count_) throw std::out_of_range("index out of range");

        if (index == count_) {

            // вставка в конец

            // блоков может быть не выделено или последний блок заполнен.
            // выделяем место под новый блок в конце и вставляем уже в него
            if (blocks_count_ == 0
            ||  blocks_[blocks_count_ - 1]->size_ == BLOCK_SIZE) {
                Block** new_blocks = new Block*[blocks_count_ + 1];
                for (size_t i = 0; i < blocks_count_; ++i) {
                    new_blocks[i] = blocks_[i];
                }
                new_blocks[blocks_count_] = new Block();

                // сохраняем информацию о том, что пришлось сделать перевыделение памяти
                IArray<T>::inc_number_of_reallocations();

                delete[] blocks_;
                blocks_ = new_blocks;
                ++blocks_count_;
            }

            size_t b_index = blocks_count_ - 1;
            size_t v_index = blocks_[b_index]->size_;
            blocks_[b_index]->values_[v_index] = item;
            ++blocks_[b_index]->size_;

        } else {

            // вставка в середину

            // блоки могут быть заполнены не полностью,
            // поэтому аккумулируем индекс позиции для вставки
            size_t pos_accum = 0;
            for (size_t b = 0; b < blocks_count_; ++b) {
                size_t b_size = blocks_[b]->size_;
                if (index > (pos_accum + b_size)) {
                    pos_accum += b_size;
                } else {
                    size_t pos = index - pos_accum;
                    if (b_size == BLOCK_SIZE) {
                        // если найденный блок для вставки заполнен,
                        // то создаем новый блок и вставку производим в него
                        Block** new_blocks = new Block*[blocks_count_ + 1];
                        for (size_t i = 0; i < b; ++i) {
                            new_blocks[i] = blocks_[i];
                        }
                        new_blocks[b] = new Block();
                        for (size_t i = b; i < blocks_count_; ++i) {
                            new_blocks[i + 1] = blocks_[i];
                        }

                        // сохраняем информацию о том, что пришлось сделать перевыделение памяти
                        IArray<T>::inc_number_of_reallocations();

                        delete[] blocks_;
                        blocks_ = new_blocks;
                        ++blocks_count_;

                        blocks_[b]->values_[0] = item;
                        ++blocks_[b]->size_;
                    } else {
                        // если найденный блок для вставки еще не заполнен,
                        // то вставляем в него и сдвигаем элементы в блоке
                        for (size_t i = b_size; i > pos; --i) {
                            blocks_[b]->values_[i] = blocks_[b]->values_[i - 1];
                        }
                        blocks_[b]->values_[pos] = item;
                        ++blocks_[b]->size_;
                    }
                    break;
                }
            }
        }

        ++count_;
    }

    T remove(size_t index) {
        if (index > count_) throw std::out_of_range("index out of range");

        T removed;
        size_t pos_accum = 0;

        for (size_t b = 0; b < blocks_count_; ++b) {
            // находим позицию элемента в блоках
            // блоки могут быть заполнены не полностью,
            // поэтому аккумулируем индекс позиции
            size_t b_size = blocks_[b]->size_;
            if (index < (pos_accum + b_size)) {
                size_t v_index = index - pos_accum;
                removed = blocks_[b]->values_[v_index];

                for (size_t i = v_index; i < (b_size - 1); ++i) {
                    blocks_[b]->values_[i] = blocks_[b]->values_[i + 1];
                }
                --blocks_[b]->size_;

                // если блок стал пустым, то спокойно удаляем его
                // и сдвигаем оставшиеся указатели блоков
                if (blocks_[b]->size_ == 0) {
                    delete blocks_[b];
                    for (size_t j = b; j < (blocks_count_ - 1); ++j) {
                        blocks_[j] = blocks_[j + 1];
                    }
                    // Block** new_blocks = new Block*[blocks_count_ - 1];
                    // for (int j = 0; j < (blocks_count_ - 1); ++j) {
                    //     new_blocks[j] = blocks_[j];
                    // }
                    // delete[] blocks_;
                    // blocks_ = new_blocks;
                    --blocks_count_;
                }
                break;
            }
            pos_accum += b_size;
        }

        --count_;
        return removed;
    }

private:
    size_t count_{0};         // количество элементов в массиве
    size_t blocks_count_{0};  // количество блоков
    Block** blocks_{nullptr}; // массив указателей на блоки
};
