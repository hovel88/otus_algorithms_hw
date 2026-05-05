#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
class Stack
{
private:
    struct Node {
        T                     data{};
        std::unique_ptr<Node> next{nullptr};

        Node(const T& value)
        :   data(value), next(nullptr) {}
        Node(T&& value)
        :   data(std::move(value)), next(nullptr) {}
    };

    std::unique_ptr<Node> top_{nullptr};
    size_t                count_{0};

public:
    ~Stack() = default;
    Stack() = default;
    Stack(const Stack& other) {
        if (other.empty()) return;

        auto* current = other.top_.get();
        auto* new_top = new Node(current->data);
        top_.reset(new_top);

        // последовательно перекопируем все элементы стека
        auto* new_current = new_top;
        current = current->next.get();
        while (current) {
            auto* new_node = new Node(current->data);
            new_current->next.reset(new_node);
            new_current = new_node;
            current = current->next.get();
        }

        count_ = other.count_;
    }
    Stack(Stack&& other) noexcept
    :   top_(std::move(other.top_)),
        count_(other.count_)
    {
        other.count_ = 0;
    }
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack temp(other);
            swap(temp); // пользуем концепцию swap, она noexcept
        }
        return *this;
    }
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            top_ = std::move(other.top_);
            count_ = other.count_;
            other.count_ = 0;
        }
        return *this;
    }

    void swap(Stack& other) noexcept {
        top_.swap(other.top_);
        std::swap(count_, other.count_);
    }
    void clear() {
        top_.reset();
        count_ = 0;
    }
    size_t size() const { return count_; }
    bool empty() const  { return count_ == 0; }

    //
    // основные операции контейнера стек, ради чего всё и затевалось
    //

    T& top() {
        if (empty()) throw std::runtime_error("Stack is empty!");
        return top_->data;
    }

    const T& top() const {
        if (empty()) throw std::runtime_error("Stack is empty!");
        return top_->data;
    }

    void pop() {
        if (empty()) throw std::runtime_error("Stack is empty!");
        top_ = std::move(top_->next);
        count_--;
    }

    void push(const T& value) {
        auto new_node = std::make_unique<Node>(value);

        new_node->next = std::move(top_);
        top_ = std::move(new_node);
        count_++;
    }

    void push(T&& value) {
        auto new_node = std::make_unique<Node>(std::move(value));

        new_node->next = std::move(top_);
        top_ = std::move(new_node);
        count_++;
    }
};
