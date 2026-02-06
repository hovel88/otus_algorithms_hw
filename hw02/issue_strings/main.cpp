#include "strings_algo.h"

int main(int argc, char** argv)
{
    std::string line;
    std::cout << "введите строку: ";
    std::getline(std::cin, line);
    std::cout << "длина введенной строки: " << count_length(line) << std::endl;

    return 0;
}
