#include "stdafx.h"
#include "main.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "BaseSolver.h"
#include "SingleSolver.h"
#include "FullSolver.h"
#include "OptimalSolver.h"
#include "TotalMinesBuilder.h"
#include "ProbabilityBuilder.h"
#include "MineSweeper.h"

using namespace std::chrono_literals;

void WriteUsage()
{
    std::cout << R"(MW1D:
Usage: MW1D <N> <M> <STRATEGY> [-v | -vv] [<FILTER> [<EXTRA>]]
Return:
  The probability of winning a 1xN Minesweeper game with the specified strategy
  Attention: One may lose on the first click
Parameters:
  <N> : length of board
  <M> : number of mines
  <STRATEGY> : one of the following:
    - sl: Single Logic
    - fl: Full Logic - Lowest Probability
    - op: Optimal
  <FILTER> : string with length of <N>:
    - -: don't care
    - m|M: is mine
    - f|F: is not mine
    - 0|1|2 : x mines surrounding
  <EXTRA> : string with length of <N>:
    - -: closed
    - +: open
Options:
  -v : verbose 
  -vv : more verbose )" << std::endl;
}

#ifdef MW1D_DLL

void CoreSaveResult(prob &&result, const char *filePath)
{
    std::ofstream fout(filePath);
    fout << to_string(result);
    fout.close();
}

extern "C"
{
    DLL_API void CoreInterfaceT(__int32 n, __int32 m, const char *solver, const char *filePath)
    {
        auto builder = std::make_shared<TotalMinesBuilder>(n, m);
        auto slv = ParseSolver(solver);
        MineSweeper mw(builder, slv);
        mw.Run();
        CoreSaveResult(mw.GetResult(), filePath);
    }

    DLL_API void CoreInterfaceP(__int32 n, __int32 pN, __int32 pD, const char *solver, const char *filePath)
    {
        prob p = pN;
        p /= pD;
        auto builder = std::make_shared<ProbabilityBuilder>(n, p);
        auto slv = ParseSolver(solver);
        MineSweeper mw(builder, slv);
        mw.Run();
        CoreSaveResult(mw.GetResult(), filePath);
    }
}

#else

size_t ParseSizeT(const char *str)
{
#ifdef _WIN64
    return std::stoull(str);
#else
    return std::stoul(str);
#endif
}

bool TryParseRational(const char *str, prob &res)
{
    auto ptr = str;
    while (*ptr != '\0')
    {
        if (*ptr == '/')
        {
            auto tmp = new char[ptr - str + 1];
            memcpy(tmp, str, sizeof(char) * (ptr - str));
            tmp[ptr - str] = '\0';
            res = ParseSizeT(tmp);
            res /= ParseSizeT(ptr + 1);
            return true;
        }
        ptr++;
    }
    return false;
}

std::shared_ptr<BaseSolver> ParseSolver(const char *str)
{
    if (strcmp(str, "sl") == 0)
        return std::make_shared<SingleSolver>();
    if (strcmp(str, "fl") == 0)
        return std::make_shared<FullSolver>();
    if (strcmp(str, "op") == 0)
        return std::make_shared<OptimalSolver>();
    return nullptr;
}

enum class Verbosity
{
    None,
    Async,
    Tree
};

bool ParseVerbosity(const char *str, Verbosity &out)
{
    if (str == nullptr)
        out = Verbosity::None;
    else if (strcmp(str, "-v") == 0)
        out = Verbosity::Async;
    else if (strcmp(str, "-vv") == 0)
        out = Verbosity::Tree;
    else
    {
        out = Verbosity::None;
        return false;
    }

    return true;
}

std::shared_ptr<std::vector<block_t>> ParseFilter(const char *str)
{
    auto n = strlen(str);
    auto res = std::make_shared<std::vector<block_t>>(n, UNKNOWN);

    for (auto i = 0; i < n; i++)
        switch (str[i])
        {
        case 'm':
        case 'M':
            res->at(i) = MINE;
            break;
        case 'f':
        case 'F':
            res->at(i) = NOMINE;
            break;
        case '0':
            res->at(i) = 0;
            break;
        case '1':
            res->at(i) = 1;
            break;
        case '2':
            res->at(i) = 2;
            break;
        case '-':
        default:
            break;
        }

    return res;
}

std::shared_ptr<std::vector<bool>> ParseExtra(const char *str)
{
    auto n = strlen(str);
    auto res = std::make_shared<std::vector<bool>>(n, false);

    for (auto i = 0; i < n; i++)
        switch (str[i])
        {
        case '+':
            res->at(i) = true;
            break;
        case '-':
        default:
            break;
        }

    return res;
}

int main(int argc, char **argv)
{
    if (argc < 4 || argc > 7)
    {
        WriteUsage();
        return 0;
    }

    Verbosity verbosity;
    std::shared_ptr<MicroSetBuilder> builder;
    std::shared_ptr<std::vector<block_t>> filter;
    std::shared_ptr<std::vector<bool>> extra;
    size_t n;
    try
    {
        auto isVerb = ParseVerbosity(argc >= 5 ? argv[4] : nullptr, verbosity);

        n = ParseSizeT(argv[1]);

        prob p;
        if (TryParseRational(argv[2], p))
            builder = std::make_shared<ProbabilityBuilder>(n, p);
        else
        {
            auto m = ParseSizeT(argv[2]);
            builder = std::make_shared<TotalMinesBuilder>(n, m);
        }

        auto filterId = isVerb ? 5 : 4;
        if (argc > filterId)
        {
            filter = ParseFilter(argv[filterId]);
            if (argc > filterId + 1)
            {
                extra = ParseExtra(argv[filterId + 1]);
                if (argc >= filterId + 2)
                    throw std::runtime_error("format error");
            }
        }
    }
    catch (...)
    {
        WriteUsage();
        return 0;
    }

    auto slv = ParseSolver(argv[3]);
    if (slv == nullptr)
    {
        WriteUsage();
        return 0;
    }

    if (verbosity != Verbosity::None)
    {
        std::cout << "Solving 1x" << n << " Minesweeper game ";
        std::cout << "using strategy " << slv->GetDescription() << std::endl;
    }

    MineSweeper mw(builder, slv, filter, extra);
    if (verbosity == Verbosity::Async)
        mw.RunAsync(200ms);
    else
        mw.Run(verbosity == Verbosity::Tree);

    if (verbosity != Verbosity::None)
    {
        std::cout << "The probability of winning the game with ";
        std::cout << slv->GetDescription() << " is:" << std::endl;
    }

    std::cout << to_string(mw.GetResult()) << std::endl;

#ifdef USE_RATIONAL
    if (verbosity != Verbosity::None)
        std::cout << "= " << to_alias_string(mw.GetResult()) << std::endl;
#endif

    return 0;
}
#endif
