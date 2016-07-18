#include "stdafx.h"
#include "Solver.h"
#include <iostream>
#include <string>
#include <limits>

void WriteUsage()
{
    std::cout << "Usage: MW1D <N> <M>" << std::endl;
    std::cout << "Return:" << std::endl;
    std::cout << "  The probability of winning a 1xN Minesweeper game with the OPTIMAL strategy, "
        << "conditioned that:" << std::endl;
    std::cout << "     - One may lose on the first click" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameters:" << std::endl;
    std::cout << "  <N> : length of board" << std::endl;
    std::cout << "  <M> : number of mines" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3)
        WriteUsage();

#ifdef _WIN64
    auto n = std::stoull(argv[1]);
    auto m = std::stoull(argv[2]);
#else
    auto n = std::stoul(argv[1]);
    auto m = std::stoul(argv[2]);
#endif

    std::cout << "Solving 1x" << n << " Minesweeper game ...";

    Solver slv(n, m);
    auto result = slv.Solve();

    std::cout << " Done" << std::endl;
    std::cout << "The OPTIMAL probability is:" << std::endl;
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout << std::fixed << result;
    std::cout << std::endl;

    return 0;
}
