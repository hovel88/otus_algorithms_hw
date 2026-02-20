#include "chess_rook.h"

int main(int argc, char** argv)
{
    std::cout << "программа расчета ходов шахматной фигуры Ладья" << std::endl;
    std::string N_str;
    std::cout << "введите номер клетки (0-63): "; std::getline(std::cin, N_str);

    try {
        uint64_t N = std::stoull(N_str);
        if (N > 63) {
            std::cout << "значение N должно быть в диапазоне [0..63]: введено " << N << std::endl;
        } else {
            ChessRook rook;
            uint64_t moves = rook.move(N);
            uint32_t count = rook.popcount(moves);
            ChessBoard::print_result(N, moves, count);
            ChessBoard::draw_result(N, moves, rook.literal());
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "неверный аргумент: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "значение вне диапазона: " << e.what() << std::endl;
    }

    return 0;
}
