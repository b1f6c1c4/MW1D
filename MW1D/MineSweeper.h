#pragma once
#include "stdafx.h"
#include <iostream>
#include <condition_variable>
#include "BaseSolver.h"
#include "MicroSetBuilder.h"

class MineSweeper final
{
public:
    MineSweeper(std::shared_ptr<MicroSetBuilder> builder, std::shared_ptr<BaseSolver> solver);
    ~MineSweeper();

    NO_COPY(MineSweeper);
    NO_MOVE(MineSweeper);

    void Run(bool verbose = false);
    template <typename Rep, typename Period>
    void RunAsync(std::chrono::duration<Rep, Period> interval);

    prob GetResult() const;

private:
    std::shared_ptr<MicroSetBuilder> m_Builder;
    std::shared_ptr<BaseSolver> m_Solver;

    std::mutex m_Mtx;
    std::condition_variable m_CV;
    std::atomic_bool m_Finished;
    prob m_Result;

    void WorkerThreadEntry(bool verbose);
};

template <typename Rep, typename Period>
void MineSweeper::RunAsync(std::chrono::duration<Rep, Period> interval)
{
    m_Finished.store(false);

    {
        std::unique_lock<std::mutex> lock(m_Mtx);

        std::thread worker(&MineSweeper::WorkerThreadEntry, this, false);

        while (true)
        {
            auto res = m_CV.wait_for(lock, interval, [&]()
            {
                return m_Finished.load();
            });

            std::cout << '\r' << m_Solver->GetForks() << " forks" << std::flush;
            if (res)
                break;
        }

        worker.join();
    }

    std::cout << std::endl << "Done" << std::endl;
}
