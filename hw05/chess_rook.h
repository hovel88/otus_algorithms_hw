#pragma once

#include "chess_board.h"
#include "bit_count.h"

class ChessRook : public ChessBoard
{
public:
    virtual ~ChessRook() = default;
    ChessRook() = default;
    ChessRook(const ChessRook&) = default;
    ChessRook(ChessRook&&) = default;
    ChessRook& operator=(const ChessRook&) = default;
    ChessRook& operator=(ChessRook&&) = default;

    uint64_t move(uint32_t pos) const override {
        uint64_t moves = 0;
        uint32_t row = 0;
        uint32_t col = 0;
        pos_to_coords(pos, row, col);

        // Ладья ходит по горизонтали и по вертикали
        // на не ограниченное количество клеток

        // с текущей позиции вправо
        for (int c = col + 1; c < 8; c++) {
            moves |= (uint64_t)1 << (row * 8 + c);
        }
        // с текущей позиции влево
        for (int c = col - 1; c >= 0; c--) {
            moves |= (uint64_t)1 << (row * 8 + c);
        }
        // с текущей позиции вверх
        for (int r = row + 1; r < 8; r++) {
            moves |= (uint64_t)1 << (r * 8 + col);
        }
        // с текущей позиции вниз
        for (int r = row - 1; r >= 0; r--) {
            moves |= (uint64_t)1 << (r * 8 + col);
        }

        return moves;
    }

    uint32_t popcount(uint64_t x) const override {
        // используем алгоритм с кешированием по байтам
        return popcount_v3(x);
    }

    std::string literal() const override {
        // return std::string("R");
        return std::string("♖");
    }
};
