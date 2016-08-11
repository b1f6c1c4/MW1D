#include "stdafx.h"
#include "main.h"
#include <iostream>
#include <string>
#include <chrono>
#include "BaseSolver.h"
#include "SingleSolver.h"
#include "FullSolver.h"
#include "OptimalSolver.h"
#include "TotalMinesBuilder.h"
#include "ProbabilityBuilder.h"
#include "MineSweeper.h"

std::shared_ptr<BaseSolver> ParseSolver(const std::string &str)
{
    if (str == "sl")
        return std::make_shared<SingleSolver>(false);
    if (str == "sle")
        return std::make_shared<SingleSolver>(true);
    if (str.length() >= 2 && str.substr(0, 2) == "fl")
    {
        if (str.length() == 2)
            return std::make_shared<FullSolver>();
        if (str.length() > 3)
            return std::make_shared<FullSolver>(CreateHeuristic(str.substr(3)));
        throw std::runtime_error("format error");
    }
    if (str == "op")
        return std::make_shared<OptimalSolver>();
    throw std::runtime_error("format error");
}

#ifdef MW1D_DLL

#include <fstream>

void CoreSaveResult(prob &&result, const char *filePath)
{
    std::ofstream fout(filePath);
    fout << to_string(result);
    fout.close();
}

extern "C"
{
    DLL_API void CoreInterfaceT(__int32 n, __int32 m, bool nR, const char *solver, const char *filePath)
    {
        auto builder = std::make_shared<TotalMinesBuilder>(n, m);
        auto slv = ParseSolver(solver);
        MineSweeper mw(builder, nR, slv, nullptr, nullptr);
        mw.Run(-2);
        CoreSaveResult(mw.GetResult(), filePath);
    }

    DLL_API void CoreInterfaceP(__int32 n, __int32 pN, __int32 pD, bool nR, const char *solver, const char *filePath)
    {
        prob p = pN;
        p /= pD;
        auto builder = std::make_shared<ProbabilityBuilder>(n, p);
        auto slv = ParseSolver(solver);
        MineSweeper mw(builder, nR, slv, nullptr, nullptr);
        mw.Run(-2);
        CoreSaveResult(mw.GetResult(), filePath);
    }
}

#else
#include "boost/program_options.hpp"

size_t ParseSizeT(const std::string &str)
{
#ifdef _WIN64
    return std::stoull(str);
#else
    return std::stoul(str);
#endif
}

bool TryParseRational(const std::string &str, prob &res)
{
    for (auto ptr = str.begin();ptr != str.end();++ptr)
    {
        if (*ptr != '/')
            continue;
        res = ParseSizeT(str.substr(0, ptr - str.begin()));
        res /= ParseSizeT(str.substr(ptr - str.begin() + 1));
        return true;
    }
    return false;
}

std::shared_ptr<std::vector<block_t>> ParseFilter(const std::string &str)
{
    auto n = str.length();
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

std::shared_ptr<std::vector<bool>> ParseExtra(const std::string &str)
{
    auto n = str.length();
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
    auto verbosity = -2;
    size_t n;
    std::string MP;
    std::string strategy;
    std::string filterS;
    std::string extraS;

    std::shared_ptr<MicroSetBuilder> builder;
    std::shared_ptr<std::vector<block_t>> filter;
    std::shared_ptr<std::vector<bool>> extra;
    std::shared_ptr<BaseSolver> slv;

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Print help messages")
        ("not-rigorous,n", "First step no mine")
        (",v", po::value<int>(&verbosity), "Verbosity, -2 = raw, -1 = monitor")
        ("N", po::value<size_t>(&n)->required(), "length of board")
        ("MP", po::value<std::string>(&MP)->required(),
#ifdef USE_CAS
         "number of mines; probability of mine, num/den; `p' for symbolic calculation")
#else
         "number of mines; probability of mine, num/den")
#endif
        ("STRATEGY", po::value<std::string>(&strategy)->required(), "strategy: sl/sle/fl/op")
        ("FILTER", po::value<std::string>(&filterS), "string length <N> -:don't care; mM:mine; fF:not mine; 012: surrounding")
        ("EXTRA", po::value<std::string>(&extraS), "string length <N> -:closed; +:open");

    po::positional_options_description positionalOptions;
    positionalOptions.add("N", 1);
    positionalOptions.add("MP", 1);
    positionalOptions.add("STRATEGY", 1);
    positionalOptions.add("FILTER", 1);
    positionalOptions.add("EXTRA", 1);

    po::variables_map vm;
    try
    {
        po::store(
                  po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(positionalOptions).run(),
                  vm);
        if (vm.count("help"))
        {
#ifdef USE_CAS
            std::cout << "MW1D";
#else
            std::cout << "MW1D-double";
#endif
            std::cout << std::endl << desc << std::endl;
            return 0;
        }

        po::notify(vm);

#ifdef USE_CAS
        auto p = MakeSymbol("p");
        if (MP == "p")
            builder = std::make_shared<ProbabilityBuilder>(n, p);
#else
        prob p;
        if (MP == "p")
            throw std::runtime_error("cannot symbolic");
#endif
        else if (TryParseRational(MP, p))
            builder = std::make_shared<ProbabilityBuilder>(n, p);
        else
        {
            auto m = ParseSizeT(MP);
            builder = std::make_shared<TotalMinesBuilder>(n, m);
        }

        if (filterS.length() > 0)
            filter = ParseFilter(filterS);
        if (extraS.length() > 0)
            extra = ParseExtra(extraS);

        slv = ParseSolver(strategy);
    }
    catch (std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }

    if (verbosity != -2)
    {
        std::cout << "Solving 1x" << n << " Minesweeper game ";
        if (vm.count("not-rigorous") > 0)
            std::cout << "(Not Rigorous) ";
        std::cout << "using strategy " << slv->GetDescription() << std::endl;
    }

    MineSweeper mw(builder, vm.count("not-rigorous") > 0, slv, filter, extra);
    {
        using namespace std::chrono_literals;

        if (verbosity == -1)
            mw.RunAsync(200ms);
        else
            mw.Run(verbosity);
    }

    if (verbosity != -2)
    {
        std::cout << "The probability of winning the game with ";
        std::cout << slv->GetDescription() << " is:" << std::endl;
    }

    std::cout << to_string(mw.GetResult()) << std::endl;

#ifdef USE_RATIONAL
    if (verbosity != -2)
        std::cout << "= " << to_alias_string(mw.GetResult()) << std::endl;
#endif

    return 0;
}
#endif
