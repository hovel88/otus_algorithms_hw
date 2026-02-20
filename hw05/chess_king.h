#pragma once

#include "chess_board.h"
#include "bit_count.h"

class ChessKing : public ChessBoard
{
public:
    virtual ~ChessKing() = default;
    ChessKing() = default;
    ChessKing(const ChessKing&) = default;
    ChessKing(ChessKing&&) = default;
    ChessKing& operator=(const ChessKing&) = default;
    ChessKing& operator=(ChessKing&&) = default;

    uint64_t move(uint32_t pos) const override {
        uint64_t bitfield = pos_to_bitfield(pos);
        uint64_t moves = 0;

        // маски, чтобы исключить возможности движения из определенных клеток
        const uint64_t from_H_reject = 0xFEFEFEFEFEFEFEFE; // ~0x0101010101010101  запрещаем телепортацию из H в A
        const uint64_t from_A_reject = 0x7F7F7F7F7F7F7F7F; // ~0x8080808080808080  запрещаем телепортацию из A в H

        // Король может двигаться на одну клетку в любые 4 стороны и диагонали
        moves |= (bitfield << 1) & from_H_reject;   // ход фигуры вправо
        moves |= (bitfield >> 1) & from_A_reject;   // ход фигуры влево
        moves |= (bitfield << 8);                   // ход фигуры вверх (безопасен за счет переполнения)
        moves |= (bitfield >> 8);                   // ход фигуры вниз (безопасен за счет переполнения)
        moves |= (bitfield << 9) & from_H_reject;   // ход фигуры вверх-вправо
        moves |= (bitfield << 7) & from_A_reject;   // ход фигуры вверх-влево
        moves |= (bitfield >> 7) & from_H_reject;   // ход фигуры вниз-вправо
        moves |= (bitfield >> 9) & from_A_reject;   // ход фигуры вниз-влево

        return moves;
    }

    uint32_t popcount(uint64_t x) const override {
        // используем алгоритм итеративного подсчета через маску
        return popcount_v1(x);
    }

    std::string literal() const override {
        // return std::string("K");
        return std::string("♔");
    }
};
