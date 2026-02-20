#pragma once

#include "chess_board.h"
#include "bit_count.h"

class ChessBishop : public ChessBoard
{
public:
    virtual ~ChessBishop() = default;
    ChessBishop() = default;
    ChessBishop(const ChessBishop&) = default;
    ChessBishop(ChessBishop&&) = default;
    ChessBishop& operator=(const ChessBishop&) = default;
    ChessBishop& operator=(ChessBishop&&) = default;

    uint64_t move(uint32_t pos) const override {
        uint64_t moves = 0;
        uint32_t row = 0;
        uint32_t col = 0;
        pos_to_coords(pos, row, col);

        // Слон ходить по диагоналям
        // на не ограниченное количество клеток

        // с текущей позиции на северо-восток (++row, ++col)
        for (int r = row + 1, c = col + 1; r < 8 && c < 8; r++, c++) {
            moves |= (uint64_t)1 << (r * 8 + c);
        }
        // с текущей позиции на северо-запад (++row, --col)
        for (int r = row + 1, c = col - 1; r < 8 && c >= 0; r++, c--) {
            moves |= (uint64_t)1 << (r * 8 + c);
        }
        // с текущей позиции на юго-восток (--row, ++col)
        for (int r = row - 1, c = col + 1; r >= 0 && c < 8; r--, c++) {
            moves |= (uint64_t)1 << (r * 8 + c);
        }
        // с текущей позиции на юго-запад (--row, --col)
        for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; r--, c--) {
            moves |= (uint64_t)1 << (r * 8 + c);
        }

        return moves;
    }

    uint32_t popcount(uint64_t x) const override {
        // используем алгоритм с кешированием по полубайтам
        return popcount_v4(x);
    }

    std::string literal() const override {
        // return std::string("B");
        return std::string("♗");
    }
};
