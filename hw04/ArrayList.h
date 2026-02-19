#pragma once

#include "IArray.h"

template <typename T>
class ArrayList : public IArray<T>
{
private:
    struct Node {
        T data_;
        Node* prev_{nullptr};
        Node* next_{nullptr};

        Node(const T& value, Node* p = nullptr, Node* n = nullptr)
        :   data_(value),
            prev_(p),
            next_(n)
        {}
    };

public:
    virtual ~ArrayList() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next_;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        count_ = 0;
    }
    ArrayList()
    :   IArray<T>("На основе двусвязного списка"),
        head_(nullptr),
        tail_(nullptr),
        count_(0)
    {}
    ArrayList(const ArrayList&) = delete;
    ArrayList(ArrayList&&) = delete;
    ArrayList& operator=(const ArrayList&) = delete;
    ArrayList& operator=(ArrayList&&) = delete;

    explicit ArrayList(size_t /*cap*/)
    :   ArrayList()
    {}

    size_t capacity() const override {
        // тут нет выделенной ёмкости контейнера, он всегда растет и уменьшается
        return count_;
    }
    size_t count() const override {
        return count_;
    }

    T get(size_t index, const T def) const override {
        if (index >= capacity()) return def;

        Node* found = nullptr;
        if (index < count_ / 2) {
            Node* n = head_;
            for (size_t i = 0; i < index; ++i) {
                n = n->next_;
            }
            found = n;
        } else {
            Node* n = tail_;
            for (size_t i = count_ - 1; i > index; --i) {
                n = n->prev_;
            }
            found = n;
        }

        if (found) return found->data_;
        throw std::runtime_error("can't find position");
    }

    void add(const T& item, size_t index = 0) override {
        if (index > count_) throw std::out_of_range("index out of range");

        Node* new_node = new Node(item);
        if (count_ == 0) {
            // вставка в пустой список
            head_ = new_node;
            tail_ = new_node;
        } else
        if (index == 0) {
            // вставка в начало списка
            new_node->next_ = head_;
            head_->prev_ = new_node;
            head_ = new_node;
        } else
        if (index == count_) {
            // вставка в конец списка
            new_node->prev_ = tail_;
            tail_->next_ = new_node;
            tail_ = new_node;
        } else {
            // вставка в середину списка
            Node* curr = nullptr;
            if (index < count_ / 2) {
                Node* n = head_;
                for (size_t i = 0; i < index; ++i) {
                    n = n->next_;
                }
                curr = n;
            } else {
                Node* n = tail_;
                for (size_t i = count_ - 1; i > index; --i) {
                    n = n->prev_;
                }
                curr = n;
            }
            Node* prev = curr->prev_;

            new_node->prev_ = prev;
            new_node->next_ = curr;

            prev->next_ = new_node;
            curr->prev_ = new_node;
        }

        // сохраняем информацию о том, что пришлось сделать перевыделение памяти
        IArray<T>::inc_number_of_reallocations();

        ++count_;
    }
    T remove(size_t index) {
        if (index > count_) throw std::out_of_range("index out of range");

        Node* node = nullptr;
        if (count_ == 1) {
            // удаление единственного элемента списка
            node  = head_;
            head_ = nullptr;
            tail_ = nullptr;
        } else
        if (index == 0) {
            // удаление из начала списка
            node = head_;
            head_ = head_->next_;
            head_->prev_ = nullptr;
        } else
        if (index == (count_ - 1)) {
            // удаление из конца списка
            node = tail_;
            tail_ = tail_->prev_;
            tail_->next_ = nullptr;
        } else {
            // удаление из середины
            if (index < count_ / 2) {
                Node* n = head_;
                for (size_t i = 0; i < index; ++i) {
                    n = n->next_;
                }
                node = n;
            } else {
                Node* n = tail_;
                for (size_t i = count_ - 1; i > index; --i) {
                    n = n->prev_;
                }
                node = n;
            }

            Node* prev = node->prev_;
            Node* next = node->next_;

            prev->next_ = next;
            next->prev_ = prev;
        }

        T removed = node->data_;
        delete node;
        --count_;

        return removed;
    }

private:
    Node*  head_{nullptr};   // ссылка, указывающая на начало массива
    Node*  tail_{nullptr};   // ссылка, указывающая на конец массива
    size_t count_{0};        // количество элементов в массиве
};
