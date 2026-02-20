#pragma once

#include "chess_board.h"
#include "bit_count.h"

class ChessKnight : public ChessBoard
{
public:
    virtual ~ChessKnight() = default;
    ChessKnight() = default;
    ChessKnight(const ChessKnight&) = default;
    ChessKnight(ChessKnight&&) = default;
    ChessKnight& operator=(const ChessKnight&) = default;
    ChessKnight& operator=(ChessKnight&&) = default;

    uint64_t move(uint32_t pos) const override {
        uint64_t bitfield = pos_to_bitfield(pos);
        uint64_t moves = 0;

        // маски, чтобы исключить возможности движения из определенных клеток
        const uint64_t from_H_reject = 0xFEFEFEFEFEFEFEFE;  // ~0x0101010101010101  запрещаем телепортацию из H в A
        const uint64_t from_A_reject = 0x7F7F7F7F7F7F7F7F;  // ~0x8080808080808080  запрещаем телепортацию из A в H
        const uint64_t from_GH_reject = 0xFCFCFCFCFCFCFCFC; // ~0x0303030303030303  запрещаем телепортацию из G в A
        const uint64_t from_AB_reject = 0x3F3F3F3F3F3F3F3F; // ~0xC0C0C0C0C0C0C0C0  запрещаем телепортацию из B в H

        // Конь может делать 8 прыжков буквой Г
        moves |= (bitfield << 17) & from_H_reject;  // ход фигуры вверх-вверх-вправо (+2,+1)
        moves |= (bitfield << 15) & from_A_reject;  // ход фигуры вверх-вверх-влево (+2,-1)
        moves |= (bitfield << 10) & from_GH_reject; // ход фигуры вверх-вправо-вправо (+1,+2)
        moves |= (bitfield << 6)  & from_AB_reject; // ход фигуры вверх-влево-влево (+1,-2)
        moves |= (bitfield >> 6)  & from_GH_reject; // ход фигуры вниз-вправо-вправо (-1,+2)
        moves |= (bitfield >> 10) & from_AB_reject; // ход фигуры вниз-влево-влево (-1,-2)
        moves |= (bitfield >> 15) & from_H_reject;  // ход фигуры вниз-вниз-вправо (-2,+1)
        moves |= (bitfield >> 17) & from_A_reject;  // ход фигуры вниз-вниз-влево (-2,-1)

        return moves;
    }

    uint32_t popcount(uint64_t x) const override {
        // используем алгоритм Кернигана
        return popcount_v2(x);
    }

    std::string literal() const override {
        // return std::string("N");
        return std::string("♘");
    }
};
