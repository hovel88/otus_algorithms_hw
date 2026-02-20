#pragma once

#include <iostream>
#include <exception>
#include <string>

namespace Color {
// Сброс
    const std::string RESET             = "\033[0m";

    const std::string BLACK             = "\033[30m";
    const std::string RED               = "\033[31m";
    const std::string GREEN             = "\033[32m";
    const std::string YELLOW            = "\033[33m";
    const std::string BLUE              = "\033[34m";
    const std::string MAGENTA           = "\033[35m";
    const std::string CYAN              = "\033[36m";
    const std::string WHITE             = "\033[37m";

    const std::string BG_BLACK          = "\033[40m";
    const std::string BG_RED            = "\033[41m";
    const std::string BG_GREEN          = "\033[42m";
    const std::string BG_YELLOW         = "\033[43m";
    const std::string BG_BLUE           = "\033[44m";
    const std::string BG_MAGENTA        = "\033[45m";
    const std::string BG_CYAN           = "\033[46m";
    const std::string BG_WHITE          = "\033[47m";

    const std::string BG_BRIGHT_BLACK   = "\033[100m";
    const std::string BG_BRIGHT_RED     = "\033[101m";
    const std::string BG_BRIGHT_GREEN   = "\033[102m";
    const std::string BG_BRIGHT_YELLOW  = "\033[103m";
    const std::string BG_BRIGHT_BLUE    = "\033[104m";
    const std::string BG_BRIGHT_MAGENTA = "\033[105m";
    const std::string BG_BRIGHT_CYAN    = "\033[106m";
    const std::string BG_BRIGHT_WHITE   = "\033[107m";
}

class ChessBoard
{
public:
    virtual ~ChessBoard() = default;
    ChessBoard() = default;
    ChessBoard(const ChessBoard&) = default;
    ChessBoard(ChessBoard&&) = default;
    ChessBoard& operator=(const ChessBoard&) = default;
    ChessBoard& operator=(ChessBoard&&) = default;

    // Используется следующий bitboard:
    //
    // 8:   56  57  58  59  60  61  62  63
    // 7:   48  49  50  51  52  53  54  55
    // 6:   40  41  42  43  44  45  46  47
    // 5:   32  33  34  35  36  37  38  39
    // 4:   24  25  26  27  28  29  30  31
    // 3:   16  17  18  19  20  21  22  23
    // 2:   08  09  10  11  12  13  14  15
    // 1:   00  01  02  03  04  05  06  07
    //      --  --  --  --  --  --  --  --
    //      A   B   C   D   E   F   G   H

    // перевод номера клетки (0-63) в позицию бита
    static uint64_t pos_to_bitfield(uint32_t pos) {
        return (uint64_t)1 << (pos);
    }

    // конвертация номера клетки (0-63) в координаты на доске
    static void pos_to_coords(uint32_t pos, uint32_t& row, uint32_t& col) {
        if (pos > 63) return;
        row = pos / 8; // 0-7 (0 = 1-й ряд)
        col = pos % 8; // 0-7 (0 = колонка A)
    }

    // конвертация координат на доске в номер клетки (0-63)
    static uint32_t coords_to_pos(uint32_t row, uint32_t col) {
        return row * 8 + col;
    }

    // является ли клетка четной (для шахматной раскраски)
    static bool is_light_cell(uint32_t row, uint32_t col) {
        // А1 (row=0, col=0) - темная
        // B1 (row=0, col=1) - светлая
        // (row + col) % 2 == 0 для темных клеток (четное)
        // (row + col) % 2 == 1 для светлых клеток (нечетное)
        return (row + col) % 2 == 1;
    }

    static char col_to_char(uint32_t col) {
        return 'A' + col;
    }
    static char row_to_char(uint32_t row) {
        return '1' + row;
    }
    // получение названия клетки (например, "E4")
    static std::string cell_name(uint32_t pos) {
        uint32_t row = 0;
        uint32_t col = 0;
        pos_to_coords(pos, row, col);
        return std::string(1, col_to_char(col)) + std::string(1, row_to_char(row));
    }

    static void print_result(uint32_t curr_pos, uint64_t moves_bitboard, uint32_t moves_count) {
        std::cout << "позиция: " << curr_pos << " (" << cell_name(curr_pos) << ")" << std::endl;
        std::cout << "количество ходов: " << moves_count << std::endl;
        std::cout << "код ходов: " << moves_bitboard << std::endl;
    }

    static void draw_result(uint32_t curr_pos, uint64_t moves_bitboard, const std::string& literal) {
        for (int row = 7; row >= 0; --row) {
            std::cout << (row + 1) << ": ";
            for (int col = 0; col < 8; ++col) {
                uint32_t pos     = coords_to_pos(row, col);
                uint64_t pos_bit = pos_to_bitfield(pos);

                if (pos == curr_pos) {
                    std::cout << Color::BG_GREEN << Color::BLACK << " " << literal << " " << Color::RESET;
                } else if (moves_bitboard & pos_bit) {
                    std::cout << Color::BG_YELLOW << Color::BLACK << " • " << Color::RESET;
                } else {
                    if (!is_light_cell(row, col)) {
                        std::cout << "   ";
                    } else {
                        std::cout << "███";
                    }
                }
            }
            std::cout << std::endl;
        }
        std::cout << "   ";
        for (int col = 0; col < 8; ++col) {
            std::cout << " " << col_to_char(col) << " ";
        }
        std::cout << std::endl;
    }

    // метод для вычисления вариантов перемещения фигуры по доске,
    // начиная с указанного номера клетки (1-64).
    // каждый класс будет вычислять своим алгоритмом
    virtual uint64_t move(uint32_t pos) const = 0;

    // подсчет количества битов в числе сделаем интерфейсной функцией.
    // каждый класс будет вычислять своим алгоритмом
    virtual uint32_t popcount(uint64_t x) const = 0;

    // возвращает символ шахматной фигуры.
    // каждый класс предоставляет свой символ
    virtual std::string literal() const = 0;
};
