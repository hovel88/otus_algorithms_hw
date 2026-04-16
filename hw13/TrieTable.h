#pragma once

#include <string>
#include <array>
#include <cassert>

#include "IHashTable.h"

template<typename V>
class TrieTable : public IHashTable<std::string, V>
{
    // XXX: можно добавить и такую штуку, чтобы заставить для корректной работы использовать беззнаковый вариант
    // static_assert(std::is_unsigned<char>::value, "WARNING: char is signed on this platform. Use -funsigned-char compiler flag");

private:
    static constexpr size_t ALPHABET_SIZE = 128;
    static size_t char_to_index_(char ch) {
        unsigned char uc = static_cast<unsigned char>(ch);
        assert(uc < ALPHABET_SIZE && "only ASCII characters (0-127) are supported!");
        if (uc >= ALPHABET_SIZE) throw std::invalid_argument("TrieTable :: non-ASCII character in key");
        return static_cast<size_t>(uc);
    }

    struct TrieNode {
        bool is_end_of_word{false};
        V value;
        std::array<TrieNode*, ALPHABET_SIZE> children_{};

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
        for (char ch : key) {
            size_t idx = char_to_index_(ch);
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
        for (char ch : key) {
            size_t idx = char_to_index_(ch);
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

        size_t idx = char_to_index_(key[depth]);
        TrieNode* child = curr->children_[idx];
        if (child == nullptr) return false;

        bool can_delete_child = remove_recursive(child, key, depth + 1, removed);
        if (can_delete_child) {
            delete child;
            curr->children_[idx] = nullptr;
            // возвращаем true, если текущий узел не является концом слова
            // и у него нет других детей. передаем в рекурсию выше и там
            // уже этот узел удалят
            return !curr->is_end_of_word && !curr->has_children();
        }
        return false;
    }
};
