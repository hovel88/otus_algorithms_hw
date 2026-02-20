#include "chess_king.h"

int main(int argc, char** argv)
{
    std::cout << "программа расчета ходов шахматной фигуры Король" << std::endl;
    std::string N_str;
    std::cout << "введите номер клетки (0-63): "; std::getline(std::cin, N_str);

    try {
        uint64_t N = std::stoull(N_str);
        if (N > 63) {
            std::cout << "значение N должно быть в диапазоне [0..63]: введено " << N << std::endl;
        } else {
            ChessKing king;
            uint64_t moves = king.move(N);
            uint32_t count = king.popcount(moves);
            ChessBoard::print_result(N, moves, count);
            ChessBoard::draw_result(N, moves, king.literal());
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
