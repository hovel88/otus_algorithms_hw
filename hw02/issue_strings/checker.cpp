#include "checker_internals.h"
#include "strings_algo.h"

int main(int argc, char** argv)
{
    const auto test_dir = std::filesystem::current_path() / std::filesystem::path("0.String");
    checker::tested_func func = [](const std::string &str) -> std::string {
        return std::to_string(count_length(str));
    };

    checker::run_tests(func, checker::prepare_tests(test_dir.string()));

    return 0;
}
