#pragma once

#include <string>
#include <cstddef>

template<typename K>
struct Hash {
    size_t operator()(const K& key) const {
        return static_cast<size_t>(key);
    }
};
