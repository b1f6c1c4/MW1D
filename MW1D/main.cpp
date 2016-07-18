#include "stdafx.h"
#include "BasicSolver.h"
#include "OptimalSolver.h"
#include "SingleSolver.h"
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <atomic>

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

std::unique_ptr<BasicSolver> slv;
std::mutex mtx;
std::condition_variable cv;
std::atomic_bool finished;
double result;

void WorkerThreadEntry()
{
    result = slv->Solve();

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished.store(true);
        cv.notify_all();
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        WriteUsage();
        return 0;
    }

#ifdef _WIN64
    auto n = std::stoull(argv[1]);
    auto m = std::stoull(argv[2]);
#else
    auto n = std::stoul(argv[1]);
    auto m = std::stoul(argv[2]);
#endif

    std::cout << "Solving 1x" << n << " Minesweeper game ..." << std::endl;

    slv = std::make_unique<SingleSolver>(n, m);
    finished.store(false);

    {
        std::unique_lock<std::mutex> lock(mtx);

        std::thread worker(WorkerThreadEntry);

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

        worker.join();
    }


    std::cout << std::endl << "Done" << std::endl;

    slv.reset();

    std::cout << "The OPTIMAL probability is:" << std::endl;
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout << std::fixed << result;
    std::cout << std::endl;

    return 0;
}
