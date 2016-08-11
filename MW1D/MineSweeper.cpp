#include "stdafx.h"
#include "MineSweeper.h"

MineSweeper::MineSweeper(std::shared_ptr<MicroSetBuilder> builder, bool notRigorous, std::shared_ptr<BaseSolver> solver, std::shared_ptr<std::vector<block_t>> filter, std::shared_ptr<std::vector<bool>> extra) : m_Builder(builder), m_NotRigorous(notRigorous), m_Solver(solver), m_Filter(filter), m_Extra(extra) { }

MineSweeper::~MineSweeper() { }

void MineSweeper::Run(int verbosity)
{
    auto root = std::make_shared<ExtendedMacro>(m_Builder->GetN());
    m_Builder->Build(*root);
    DoFilter(*root);
    DoExtra(*root);
    m_Solver->LoadData(root, m_Builder->GetM(), m_NotRigorous);
    m_Result = m_Solver->Solve(verbosity);
}

prob MineSweeper::GetResult() const
{
    return m_Result;
}

void MineSweeper::WorkerThreadEntry(int verbosity)
{
    Run(verbosity);

    {
        std::lock_guard<std::mutex> lock(m_Mtx);
        m_Finished.store(true);
        m_CV.notify_all();
    }
}

void MineSweeper::DoExtra(ExtendedMacro &macro) const
{
    if (m_Filter == nullptr)
        return;

    if (m_Extra == nullptr)
    {
        for (auto i = 0; i < m_Builder->GetN(); i++)
            if (m_Filter->at(i) > 0 || m_Filter->at(i) == MINE)
            {
                macro[i] = true;
                macro.Info[i] = m_Filter->at(i);
            }
    }
    else
    {
        for (auto i = 0; i < m_Builder->GetN(); i++)
        {
            macro[i] = m_Extra->at(i);

            if (m_Extra->at(i))
                macro.Info[i] = m_Filter->at(i);
        }
    }
}

void MineSweeper::DoFilter(Macro &macro) const
{
    if (m_Filter == nullptr)
        return;

    for (auto i = 0; i < m_Builder->GetN(); i++)
        if (m_Filter->at(i) != UNKNOWN)
            macro = Macro(macro, i, m_Filter->at(i));
}
