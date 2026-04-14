#pragma once

#include <string>

// реализация класса префиксного дерева (Trie),
// сделанная по требованиям с сайта
// https://leetcode.com/problems/implement-trie-prefix-tree/submissions/1977301260/
// Runtime: 20ms    beats 61.09%
// Memory:  51.41MB beats 21.74%

class Trie {
private:
    struct TrieNode {
        bool is_end_of_word{false};
        TrieNode* children_[26];

        TrieNode() : is_end_of_word(false) {
            for (int i = 0; i < 26; ++i) {
                children_[i] = nullptr;
            }
        }
        ~TrieNode() {
            for (int i = 0; i < 26; ++i) {
                delete children_[i];
                children_[i] = nullptr;
            }
        }
    };

    TrieNode* root_{nullptr};

    void clear(TrieNode* node) {
        delete node;
    }

public:
    ~Trie() {
        clear(root_);
    }
    Trie() {
        root_ = new TrieNode();
    }

    void insert(std::string word) {
        TrieNode* curr = root_;
        // двигаясь посимвольно внутри слова - проходим по детям
        // префиксного дерева, пока не спустимся до конечного узла
        for (const char ch : word) {
            auto idx = ch - 'a';
            if (curr->children_[idx] == nullptr) {
                curr->children_[idx] = new TrieNode();
            }
            curr = curr->children_[idx];
        }

        // вставили слово целиком, помечем последний узел, как конец слова
        curr->is_end_of_word = true;
    }

    bool search(std::string word) {
        TrieNode* curr = root_;
        // двигаясь посимвольно внутри слова - проходим по детям
        // префиксного дерева.
        // ищем узел, помещенный как конец слова
        for (const char ch : word) {
            auto idx = ch - 'a';
            if (curr->children_[idx] == nullptr) return false;
            curr = curr->children_[idx];
        }
        return curr->is_end_of_word;
    }

    bool startsWith(std::string prefix) {
        TrieNode* curr = root_;
        // двигаясь посимвольно внутри слова - проходим по детям
        // префиксного дерева.
        // для префикса нет необходимости искать узел, помеченный
        // именно как конец слова, главное что есть начало слова
        for (const char ch : prefix) {
            auto idx = ch - 'a';
            if (curr->children_[idx] == nullptr) return false;
            curr = curr->children_[idx];
        }
        return true;
    }
};
