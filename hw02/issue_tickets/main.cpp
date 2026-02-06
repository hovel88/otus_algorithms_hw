#include "tickets_algo.h"

int main(int argc, char** argv)
{
    std::string line;
    std::cout << "Программа подсчета количества счастливых билетов с четным числом цифр" << std::endl;
    std::cout << "Цифр в билете 2N, где N=[1..10]. Введите N: ";
    std::getline(std::cin, line);
    std::cout << "Количество счастливых билетов: " << count_lucky_tickets(line) << std::endl;

    return 0;
}
