#include "checker_internals.h"
#include "chess_king.h"
#include "chess_knight.h"
#include "chess_rook.h"
#include "chess_bishop.h"
#include "chess_queen.h"

using func_under_test_t = std::function<uint32_t(uint64_t N, uint64_t& M)>;
checker::test_result_t scoped_func(func_under_test_t func_under_test,
                                   const std::string &expected_count,
                                   const std::string &expected_moves,
                                   const std::string &N_str)
{
        checker::test_result_t res;
        try {
            uint64_t N = std::stoull(N_str);
            if (N < 64) {
                auto start = std::chrono::high_resolution_clock::now();
                uint64_t moves = 0;
                uint32_t count = func_under_test(N, moves);
                auto stop = std::chrono::high_resolution_clock::now();
                res.elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

                auto moves_str = std::to_string(moves);
                auto count_str = std::to_string(count);
                res.fail_details.append("ожидалось count='").append(expected_count).append("' получено count='").append(count_str).append("' ");
                res.fail_details.append("ожидалось moves='").append(expected_moves).append("' получено moves='").append(moves_str).append("'");

                if (expected_moves != moves_str
                ||  expected_count != count_str) {
                    res.success = false;
                } else {
                    res.success = true;
                }
            }
        } catch (const std::invalid_argument& e) {
            res.fail_details.append("неверный аргумент: ").append(e.what());
        } catch (const std::out_of_range& e) {
            res.fail_details.append("значение вне диапазона: ").append(e.what());
        }
        return res;
}

uint32_t algo_king(uint64_t N, uint64_t& moves)
{
    ChessKing king;
    moves = king.move(N);
    return king.popcount(moves);
}
uint32_t algo_knight(uint64_t N, uint64_t& moves)
{
    ChessKnight knight;
    moves = knight.move(N);
    return knight.popcount(moves);
}
uint32_t algo_rook(uint64_t N, uint64_t& moves)
{
    ChessRook rook;
    moves = rook.move(N);
    return rook.popcount(moves);
}
uint32_t algo_bishop(uint64_t N, uint64_t& moves)
{
    ChessBishop bishop;
    moves = bishop.move(N);
    return bishop.popcount(moves);
}
uint32_t algo_queen(uint64_t N, uint64_t& moves)
{
    ChessQueen queen;
    moves = queen.move(N);
    return queen.popcount(moves);
}

int main(int argc, char** argv)
{
    checker::tested_1in_2out_func func1 = [func_under_test=algo_king](const std::string &expected_count, const std::string &expected_moves, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected_count, expected_moves, N_str);
    };
    checker::tested_1in_2out_func func2 = [func_under_test=algo_knight](const std::string &expected_count, const std::string &expected_moves, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected_count, expected_moves, N_str);
    };
    checker::tested_1in_2out_func func3 = [func_under_test=algo_rook](const std::string &expected_count, const std::string &expected_moves, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected_count, expected_moves, N_str);
    };
    checker::tested_1in_2out_func func4 = [func_under_test=algo_bishop](const std::string &expected_count, const std::string &expected_moves, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected_count, expected_moves, N_str);
    };
    checker::tested_1in_2out_func func5 = [func_under_test=algo_queen](const std::string &expected_count, const std::string &expected_moves, const std::string &N_str) -> checker::test_result_t {
        return scoped_func(func_under_test, expected_count, expected_moves, N_str);
    };

    std::unordered_map<std::string, checker::test_map_t> results;
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.BITS/1.Bitboard - Король");
        const auto tests = checker::prepare_tests(test_dir.string());
        std::cout << "\nРезультаты теста Короля\n" << std::endl;
        results["king"] = checker::run_tests(func1, tests);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.BITS/2.Bitboard - Конь");
        const auto tests = checker::prepare_tests(test_dir.string());
        std::cout << "\nРезультаты теста Коня\n" << std::endl;
        results["knight"] = checker::run_tests(func2, tests);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.BITS/3.Bitboard - Ладья");
        const auto tests = checker::prepare_tests(test_dir.string());
        std::cout << "\nРезультаты теста Ладьи\n" << std::endl;
        results["rook"] = checker::run_tests(func3, tests);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.BITS/4.Bitboard - Слон");
        const auto tests = checker::prepare_tests(test_dir.string());
        std::cout << "\nРезультаты теста Слона\n" << std::endl;
        results["bishop"] = checker::run_tests(func4, tests);
    }
    {
        const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.BITS/5.Bitboard - Ферзь");
        const auto tests = checker::prepare_tests(test_dir.string());
        std::cout << "\nРезультаты теста Ферзя\n" << std::endl;
        results["queen"] = checker::run_tests(func5, tests);
    }

    std::cout << "\nСравнение затраченного времени на выполнение (нс)\n" << std::endl;
    checker::elapsed_time_table(results);

    return 0;
}
