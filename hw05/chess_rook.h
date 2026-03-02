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

        static const uint64_t ROW_MASKS[8] = {
            0x00000000000000FF, // row 0
            0x000000000000FF00,
            0x0000000000FF0000,
            0x00000000FF000000,
            0x000000FF00000000,
            0x0000FF0000000000,
            0x00FF000000000000,
            0xFF00000000000000  // row 7
        };
        static const uint64_t COL_MASKS[8] = {
            0x0101010101010101, // column 0 (A)
            0x0202020202020202,
            0x0404040404040404,
            0x0808080808080808,
            0x1010101010101010,
            0x2020202020202020,
            0x4040404040404040,
            0x8080808080808080  // column 7 (H)
        };

        // Ладья ходит по горизонтали и по вертикали
        // на не ограниченное количество клеток

        // получаем всю строку и весь столбец для текущей позиции,
        // и убираем саму текущую позицию из маски
        moves  = ROW_MASKS[row];
        moves |= COL_MASKS[col];
        moves &= ~((uint64_t)1 << pos);

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
