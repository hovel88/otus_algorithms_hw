#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
class Queue {
private:
    struct Node {
        T                     data{};
        std::unique_ptr<Node> next{nullptr};

        Node(const T& value)
        :   data(value), next(nullptr) {}
        Node(T&& value)
        :   data(std::move(value)), next(nullptr) {}
    };

    std::unique_ptr<Node> front_{nullptr};
    Node*                 back_{nullptr}; // это "сырой" указатель, но он не владеет памятью,
                                          // а только для удобства доступа. поэтому не требует
                                          // особенного деструктора
    size_t                count_{0};

public:
    ~Queue() = default;
    Queue() = default;
    Queue(const Queue& other) {
        if (other.empty()) return;

        auto new_node = std::make_unique<Node>(other.front_->data);
        front_ = std::move(new_node);
        back_ = front_.get();

        // последовательно перекопируем все элементы очереди
        auto* current = other.front_->next.get();
        while (current) {
            auto* new_node = new Node(current->data);
            back_->next.reset(new_node);
            back_ = new_node;
            current = current->next.get();
        }
        count_ = other.count_;
    }
    Queue(Queue&& other) noexcept
    :   front_(std::move(other.front_)),
        back_(other.back_),
        count_(other.count_)
    {
        other.back_ = nullptr;
        other.count_ = 0;
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            Queue temp(other);
            swap(temp); // пользуем концепцию swap, она noexcept
        }
        return *this;
    }
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            front_ = std::move(other.front_);
            back_ = other.back_;
            count_ = other.count_;
            other.back_ = nullptr;
            other.count_ = 0;
        }
        return *this;
    }

    void swap(Queue& other) noexcept {
        front_.swap(other.front_);
        std::swap(back_, other.back_);
        std::swap(count_, other.count_);
    }
    void clear() {
        front_.reset();
        back_ = nullptr;
        count_ = 0;
    }
    size_t size() const { return count_; }
    bool empty() const  { return count_ == 0; }

    //
    // основные операции контейнера очередь, ради чего всё и затевалось
    //

    T& front() {
        if (empty()) throw std::runtime_error("Queue is empty!");
        return front_->data;
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("Queue is empty!");
        return front_->data;
    }

    T& back() {
        if (empty()) throw std::runtime_error("Queue is empty!");
        return back_->data;
    }

    const T& back() const {
        if (empty()) throw std::runtime_error("Queue is empty!");
        return back_->data;
    }

    void pop() {
        if (empty()) throw std::runtime_error("Queue is empty!");
        front_ = std::move(front_->next);
        count_--;

        if (empty()) back_ = nullptr;
    }

    void push(const T& value) {
        auto new_node = std::make_unique<Node>(value);
        Node* raw_ptr = new_node.get();

        if (empty()) {
            front_ = std::move(new_node);
            back_ = raw_ptr;
        } else {
            back_->next = std::move(new_node);
            back_ = raw_ptr;
        }
        count_++;
    }

    void push(T&& value) {
        auto new_node = std::make_unique<Node>(std::move(value));
        Node* raw_ptr = new_node.get();

        if (empty()) {
            front_ = std::move(new_node);
            back_ = raw_ptr;
        } else {
            back_->next = std::move(new_node);
            back_ = raw_ptr;
        }
        count_++;
    }
};
