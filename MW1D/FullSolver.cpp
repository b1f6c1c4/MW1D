#include "stdafx.h"
#include "FullSolver.h"

FullSolver::FullSolver() : m_Heuristic(nullptr) { }

FullSolver::FullSolver(std::shared_ptr<FLHeuristic> heu) : m_Heuristic(heu) { }

FullSolver::~FullSolver() { }

std::string FullSolver::GetDescription() const
{
    if (m_Heuristic != nullptr)
        return "Full Logic - " + m_Heuristic->GetDescription();

    return "Full Logic - No Heuristic";
}

prob FullSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    auto n = macro.GetN();
    std::vector<bool> possible(n, false);
    for (auto it = macro.begin(); it != macro.end(); ++it)
        for (size_t i = 0; i < n; i++)
            if ((*it)[i])
                possible[i] = true;

    IncrementForks();

    for (size_t i = 0; i < n; i++)
        if (!possible[i] && !macro.IsOpen(i))
            return BaseSolver::Fork(macro, i, depth);

    return Heuristic(macro, depth);
}

prob FullSolver::Heuristic(ExtendedMacro &macro, size_t depth)
{
    auto n = macro.GetN();
    std::vector<bool> possibleN(n, false);
    for (auto it = macro.begin(); it != macro.end(); ++it)
        for (size_t i = 0; i < n; i++)
            if (!(*it)[i])
                possibleN[i] = true;

    std::vector<size_t> target;
    for (size_t i = 0; i < n; i++)
        if (possibleN[i] && !macro.IsOpen(i))
            target.push_back(i);

    if (m_Heuristic != nullptr)
        m_Heuristic->Process(macro, target);

    prob p = 0;
    for (size_t i = 0; i < target.size(); i++)
        p += BaseSolver::Fork(macro, target[i], depth);

    return p / target.size();
}
