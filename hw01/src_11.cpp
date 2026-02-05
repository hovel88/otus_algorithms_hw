#include <iostream>

int main(int argc, char** argv)
{
    const int N = 25;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            if (x == (1) || x == (N - 2)
            ||  y == (1) || y == (N - 2)) std::cout << "# ";
            else                          std::cout << ". ";
        }
        std::cout << std::endl;
    }

    return 0;
}
