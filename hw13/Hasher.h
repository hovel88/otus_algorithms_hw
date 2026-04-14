#pragma once

#include <string>
#include <cstddef>

template<typename K>
struct Hash {
    size_t operator()(const K& key) const {
        return static_cast<size_t>(key);
    }
};

template<>
struct Hash<std::string> {
    size_t operator()(const std::string& s) const {
        // в статьях в интернете описывается как DJB2 алгоритм,
        // выдающий на практике хорошее распределение, при хорошей скорости.
        size_t hash = 5381;
        for (char c : s) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }
};
