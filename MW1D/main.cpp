#include "stdafx.h"
#include "BaseSolver.h"
#include "SingleSolver.h"
#include "FullSolver.h"
#include "OptimalSolver.h"
#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include "MicroSetBuilder.h"
#include "TotalMinesBuilder.h"
#include "MineSweeper.h"

using namespace std::chrono_literals;

void WriteUsage()
{
    std::cout << R"(Usage: MW1D <N> <M> <STRATEGY> [-v | -vv]
Return:
  The probability of winning a 1xN Minesweeper game with the specified strategy
  Attention: One may lose on the first click
    std::cout << std::endl;
Parameters:
  <N> : length of board
  <M> : number of mines
  <STRATEGY> : one of the following:
    - sl: Single Logic
    - fl: Full Logic - Lowest Probability
    - op: Optimal
Options:
  -v : verbose 
  -vv : more verbose )" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 4 && argc != 5)
    {
        WriteUsage();
        return 0;
    }

    auto verbose = false;
    auto moreVerbose = false;
    if (argc == 5)
        if (strcmp(argv[4], "-v") == 0)
            verbose = true;
        else if (strcmp(argv[4], "-vv") == 0)
            moreVerbose = true;
        else
        {
            WriteUsage();
            return 0;
        }

    std::shared_ptr<MicroSetBuilder> builder;
    size_t n, m;
    try
    {
#ifdef _WIN64
        n = std::stoull(argv[1]);
        m = std::stoull(argv[2]);
#else
        n = std::stoul(argv[1]);
        m = std::stoul(argv[2]);
#endif
        builder = std::make_shared<TotalMinesBuilder>(n, m);
    }
    catch (...)
    {
        WriteUsage();
        return 0;
    }

    std::shared_ptr<BaseSolver> slv;
    std::string strategyName;
    if (strcmp(argv[3], "sl") == 0)
    {
        slv = std::make_unique<SingleSolver>();
        strategyName = "Single Strategy";
    }
    else if (strcmp(argv[3], "fl") == 0)
    {
        slv = std::make_unique<FullSolver>();
        strategyName = "Full Logic - Lowest Probability";
    }
    else if (strcmp(argv[3], "op") == 0)
    {
        slv = std::make_unique<OptimalSolver>();
        strategyName = "Optimal";
    }
    else
    {
        WriteUsage();
        return 0;
    }

    if (verbose)
    {
        std::cout << "Solving 1x" << n << " Minesweeper game ";
        std::cout << "using strategy " << strategyName << std::endl;
    }

    MineSweeper mw(builder, slv);
    if (verbose)
        mw.RunAsync(200ms);
    else
        mw.Run(moreVerbose);

    if (verbose)
    {
        std::cout << "The probability of winning the game with ";
        std::cout << strategyName << " is:" << std::endl;
    }
    std::cout.precision(std::numeric_limits<prob>::max_digits10);
    std::cout << std::fixed << mw.GetResult();
    std::cout << std::endl;

    return 0;
}
