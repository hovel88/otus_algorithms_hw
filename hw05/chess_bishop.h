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

        // маски для основных диагоналей
        static const uint64_t A1H8_DIAG = 0x8040201008040201ULL;
        static const uint64_t A8H1_DIAG = 0x0102040810204080ULL;

        // Слон ходить по диагоналям
        // на не ограниченное количество клеток

        // сдвигаем диагональ A1H8 так, чтобы её (0,0) совпал с нашей диагональю,
        // сдвигаем диагональ A8H1 так, чтобы её (0,7) совпал с нашей диагональю,
        // и убираем саму текущую позицию из маски
        int a1h8_shift = (row - col) * 8;
        int a8h1_shift = (row + col - 7) * 8;
        moves  = (a1h8_shift >= 0)
               ? (A1H8_DIAG << a1h8_shift)
               : (A1H8_DIAG >> -a1h8_shift);
        moves |= (a8h1_shift >= 0)
               ? (A8H1_DIAG << a8h1_shift)
               : (A8H1_DIAG >> -a8h1_shift);
        moves &= ~((uint64_t)1 << pos);

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
