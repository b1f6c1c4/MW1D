#include "stdafx.h"
#include "BasicSolver.h"
#include "SingleSolver.h"
#include "FullSolver.h"
#include "OptimalSolver.h"
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include "MicroSetBuilder.h"
#include "TotalMinesBuilder.h"

void WriteUsage()
{
    std::cout << "Usage: MW1D <N> <M> <STRATEGY> [-v | -vv]" << std::endl;
    std::cout << "Return:" << std::endl;
    std::cout << "  The probability of winning a 1xN Minesweeper game with the specified strategy" << std::endl;
    std::cout << "  Attention: One may lose on the first click" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameters:" << std::endl;
    std::cout << "  <N> : length of board" << std::endl;
    std::cout << "  <M> : number of mines" << std::endl;
    std::cout << "  <STRATEGY> : one of the following:" << std::endl;
    std::cout << "    - sl: Single Logic" << std::endl;
    std::cout << "    - fl: Full Logic - Lowest Probability" << std::endl;
    std::cout << "    - op: Optimal" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v : verbose " << std::endl;
    std::cout << "  -vv : more verbose " << std::endl;
}

std::unique_ptr<BasicSolver> slv;
std::mutex mtx;
std::condition_variable cv;
std::atomic_bool finished;
prob result;

void WorkerThreadEntry(bool mv)
{
    result = slv->Solve(mv);

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished.store(true);
        cv.notify_all();
    }
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

    std::unique_ptr<MicroSetBuilder> builder;
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
        builder = std::make_unique<TotalMinesBuilder>(n, m);
    }
    catch (...)
    {
        WriteUsage();
        return 0;
    }

    std::shared_ptr<ExtendedMacro> root = std::make_shared<ExtendedMacro>(n);
    builder->Build(*root);

    std::string strategyName;
    if (strcmp(argv[3], "sl") == 0)
    {
        slv = std::make_unique<SingleSolver>(root, m);
        strategyName = "Single Strategy";
    }
    else if (strcmp(argv[3], "fl") == 0)
    {
        slv = std::make_unique<FullSolver>(root, m);
        strategyName = "Full Logic - Lowest Probability";
    }
    else if (strcmp(argv[3], "op") == 0)
    {
        slv = std::make_unique<OptimalSolver>(root, m);
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
    finished.store(false);

    {
        std::unique_lock<std::mutex> lock(mtx);

        std::thread worker(WorkerThreadEntry, moreVerbose);

        if (verbose)
            while (true)
            {
                using namespace std::chrono_literals;
                auto res = cv.wait_for(lock, 200ms, [&]()
                                       {
                                           return finished.load();
                                       });

                std::cout << '\r' << slv->GetForks() << " forks" << std::flush;
                if (res)
                    break;
            }
        else
        {
            lock.unlock();
        }

        worker.join();
    }


    if (verbose)
        std::cout << std::endl << "Done" << std::endl;

    slv.reset();

    if (verbose)
    {
        std::cout << "The probability of winning the game with ";
        std::cout << strategyName << " is:" << std::endl;
    }
    std::cout.precision(std::numeric_limits<prob>::max_digits10);
    std::cout << std::fixed << result;
    std::cout << std::endl;

    return 0;
}
