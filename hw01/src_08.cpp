#include <iostream>

int main(int argc, char** argv)
{
    const int N = 25;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            if (x == 0 || y == 0) std::cout << "# ";
            else                  std::cout << ". ";
        }
        std::cout << std::endl;
    }

    return 0;
}
