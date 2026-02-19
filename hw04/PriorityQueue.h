#pragma once

#include <iostream>
#include <exception>
#include <string>

template <typename T>
class PriorityQueue
{
private:
    // структура данных "Приоритетная очередь" построена на массиве списков.
    // ключевой момент: количество приоритетов - фиксированное (очень частый сценарий использования)
    // в массиве хранятся односвязные списки под каждый из приоритетов

    struct Node {
        T     data_;
        Node* next_{nullptr};

        Node(const T& value, Node* n = nullptr)
        :   data_(value),
            next_(n)
        {}
    };

    struct PriorityLevel {
        Node* head_{nullptr};
        Node* tail_{nullptr};

        PriorityLevel()
        :   head_(nullptr),
            tail_(nullptr)
        {}
    };

public:
    ~PriorityQueue() {
        for (int i = 0; i < MAX_PRIORITY; ++i) {
            Node* curr = levels_[i].head_;
            while (curr != nullptr) {
                Node* temp = curr;
                curr = curr->next_;
                delete temp;
            }
            levels_[i].head_ = nullptr;
            levels_[i].tail_ = nullptr;
        }
        delete[] levels_;
        levels_ = nullptr;
        count_ = 0;
        highest_priority_ = -1;
    }
    PriorityQueue() = delete;
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue(PriorityQueue&&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;
    PriorityQueue& operator=(PriorityQueue&&) = delete;

    explicit PriorityQueue(uint32_t max_priority)
    :   MAX_PRIORITY(static_cast<int>(max_priority)),
        highest_priority_(-1),
        count_(0)
    {
        levels_ = new PriorityLevel[MAX_PRIORITY];
    }

    size_t size() const {
        return count_;
    }
    bool empty() const {
        return count_ == 0;
    }

    void enqueue(int priority, const T& item) {
        if (priority < 0 || priority >= MAX_PRIORITY) throw std::out_of_range("priority out of range");

        Node* new_node = new Node(item);

        auto& level = levels_[priority];
        if (level.tail_ == nullptr) {
            // добавляем первый элемент
            level.head_ = new_node;
            level.tail_ = new_node;

            if (priority > highest_priority_) {
                highest_priority_ = priority;
            }
        } else {
            level.tail_->next_ = new_node;
            level.tail_        = new_node;
        }

        ++count_;
    }

    T dequeue() {
        if (count_ == 0) throw std::runtime_error("queue is empty");

        // извлечение элемента из очереди начинаем с наивысшего приоритета
        auto& level = levels_[highest_priority_];

        Node* removed = level.head_;
        T value = removed->data_;

        level.head_ = level.head_->next_;

        if (level.head_ == nullptr) {
            level.tail_ = nullptr;
            // если список стал пустым, то переносим highest_priority_ на более низкий приоритет
            int p = highest_priority_;
            highest_priority_ = -1;
            for (int i = p - 1; i >= 0; --i) {
                if (levels_[i].head_ != nullptr) {
                    highest_priority_ = i;
                    break;
                }
            }
        }

        delete removed;
        --count_;
        return value;
    }

    void print_stats() const {
        std::cout << "PriorityQueue (size=" << count_ << ", highest priority=" << highest_priority_ << "):" << std::endl;
        for (int i = MAX_PRIORITY - 1; i >= 0; --i) {
            if (levels_[i].head_ != nullptr) {
                int amount = 0;
                Node* n = levels_[i].head_;
                while (n != nullptr) {
                    ++amount;
                    n = n->next_;
                }
                std::cout << "  priority " << i << ": " << amount << " element(s)\n";
            }
        }
    }

    void print_debug() const {
        std::cout << "PriorityQueue (size=" << count_ << "):" << std::endl;
        for (int i = MAX_PRIORITY - 1; i >= 0; --i) {
            if (levels_[i].head_ != nullptr) {
                std::cout << "  priority " << i << ": ";
                Node* n = levels_[i].head_;
                while (n != nullptr) {
                    std::cout << "'" << n->data_ << "' ";
                    n = n->next_;
                }
                std::cout << std::endl;
            }
        }
    }

private:
    const int       MAX_PRIORITY{0};
    int             highest_priority_{0};
    size_t          count_{0};
    PriorityLevel*  levels_{nullptr};
};
