#include "stdafx.h"
#include "MineSweeper.h"

MineSweeper::MineSweeper(std::shared_ptr<MicroSetBuilder> builder, std::shared_ptr<BaseSolver> solver) : m_Builder(builder), m_Solver(solver) { }

MineSweeper::~MineSweeper() { }

void MineSweeper::Run(bool verbose)
{
    auto root = std::make_shared<ExtendedMacro>(m_Builder->GetN());
    m_Builder->Build(*root);
    m_Solver->LoadData(root, m_Builder->GetM());
    m_Result = m_Solver->Solve(verbose);
}

prob MineSweeper::GetResult() const
{
    return m_Result;
}

void MineSweeper::WorkerThreadEntry(bool verbose)
{
    Run(verbose);

    {
        std::lock_guard<std::mutex> lock(m_Mtx);
        m_Finished.store(true);
        m_CV.notify_all();
    }
}
