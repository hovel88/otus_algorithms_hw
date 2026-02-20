#pragma once

#include "chess_board.h"
#include "bit_count.h"
#include "chess_rook.h"
#include "chess_bishop.h"

class ChessQueen : public ChessBoard
{
public:
    virtual ~ChessQueen() = default;
    ChessQueen() = default;
    ChessQueen(const ChessQueen&) = default;
    ChessQueen(ChessQueen&&) = default;
    ChessQueen& operator=(const ChessQueen&) = default;
    ChessQueen& operator=(ChessQueen&&) = default;

    uint64_t move(uint32_t pos) const override {
        // Ферзь ходить по диагоналям, по горизонтали и по вертикали
        // на не ограниченное количество клеток.
        // фактически это комбинация Слона и Ладьи
        ChessRook rook;
        ChessBishop bishop;

        return (rook.move(pos) | bishop.move(pos));
    }

    uint32_t popcount(uint64_t x) const override {
        // используем алгоритм с кешированием по полубайтам
        return popcount_v4(x);
    }

    std::string literal() const override {
        // return std::string("Q");
        return std::string("♕");
    }
};
