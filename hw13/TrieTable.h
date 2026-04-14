#pragma once

#include <string>
#include <array>

#include "IHashTable.h"

template<typename V>
class TrieTable : public IHashTable<std::string, V>
{
private:
    struct TrieNode {
        bool is_end_of_word{false};
        V value;
        std::array<TrieNode*, 128> children_{};

        ~TrieNode() {
            for (TrieNode* child : children_) {
                delete child;
            }
        }
        TrieNode() = default;
        TrieNode(const TrieNode&) = delete;
        TrieNode& operator=(const TrieNode&) = delete;

        bool has_children() const {
            for (TrieNode* child : children_) {
                if (child != nullptr) return true;
            }
            return false;
        }
    };

    TrieNode* root_{nullptr};
    size_t    elements_{0};

public:
    ~TrieTable() {
        delete root_;
    }
    TrieTable() {
        root_ = new TrieNode();
    }
    TrieTable(const TrieTable&) = delete;
    TrieTable& operator=(const TrieTable&) = delete;

    void insert(const std::string& key, const V& value) {
        if (key.empty()) throw std::invalid_argument("кey cannot be empty");

        TrieNode* curr = root_;
        // двигаясь посимвольно внутри слова - проходим по детям
        // префиксного дерева, пока не спустимся до конечного узла
        for (const char idx : key) {
            if (curr->children_[idx] == nullptr) {
                curr->children_[idx] = new TrieNode();
            }
            curr = curr->children_[idx];
        }

        // вставили слово целиком, помечем последний узел, как конец слова
        if (!curr->is_end_of_word) {
            curr->is_end_of_word = true;
            ++elements_;
        }
        curr->value = value;
    }

    std::optional<V> get(const std::string& key) const override {
        if (key.empty()) return std::nullopt;

        TrieNode* curr = root_;
        // двигаясь посимвольно внутри слова - проходим по детям
        // префиксного дерева.
        // ищем узел, помещенный как конец слова
        for (const char idx : key) {
            if (curr->children_[idx] == nullptr) return std::nullopt;
            curr = curr->children_[idx];
        }
        if (curr->is_end_of_word) return curr->value;
        return std::nullopt;
    }

    bool remove(const std::string& key) override {
        if (key.empty()) return false;

        bool removed = false;
        remove_recursive(root_, key, 0, removed);
        return removed;
    }

    size_t size() const override { return elements_; }
    bool empty() const override  { return elements_ == 0; }
    void clear() override {
        delete root_;
        root_ = new TrieNode();
        elements_ = 0;
    }
    std::string name() const override {
        return std::string("TrieTable (std::array)");
    }

    size_t collision_count() const override { return 0; }   // для Trie нет коллизий в классическом понимании
    size_t rehashes_count() const override  { return 0; }   // для Trie не определено
    double load_factor() const override     { return 0.0; } // для Trie не определен

    void debug_print() const override {}

private:
    bool remove_recursive(TrieNode* curr, const std::string& key, size_t depth, bool& removed) {
        if (!curr) return false;

        if (depth == key.length()) {
            if (curr->is_end_of_word) {
                curr->is_end_of_word = false;
                --elements_;
                removed = true;
                // можно удалить этот узел, если у него нет детей.
                // рекурсия на шаг выше об этом будет извещена и удалит узел
                return !curr->has_children();
            }
            return false;
        }

        char idx = key[depth];
        if (curr->children_[idx] == nullptr) return false;

        bool can_delete_child = remove_recursive(curr->children_[idx], key, depth + 1, removed);
        if (can_delete_child) {
            delete curr->children_[idx];
            curr->children_[idx] = nullptr;
            // возвращаем true, если текущий узел не является концом слова
            // и у него нет других детей. передаем в рекурсию выше и там
            // уже этот узел удалят
            return !curr->is_end_of_word && !curr->has_children();
        }

        return false;
    }
};
