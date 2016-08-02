#include "stdafx.h"
#include "FullSolver.h"

FullSolver::FullSolver() { }

FullSolver::~FullSolver() { }

std::string FullSolver::GetDescription() const
{
    return "Full Logic - Lowest Probability";
}

prob FullSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    if (macro.size() == 1)
        return 1;

    std::vector<size_t> cnts(macro.GetN(), 0);
    for (auto it = macro.begin(); it != macro.end(); ++it)
        for (size_t i = 0; i < macro.GetN(); i++)
            if ((*it)[i])
                cnts[i]++;

    IncrementForks();

    for (size_t i = 0; i < macro.GetN(); i++)
        if (cnts[i] == 0 && !macro.IsOpen(i))
            return BaseSolver::Fork(macro, i, depth);

    return ForkProb(macro, cnts, depth);
}

prob FullSolver::ForkProb(ExtendedMacro &macro, const std::vector<size_t> &cnts, size_t depth)
{
    auto val = macro.size();

    for (size_t i = 0; i < macro.GetN(); i++)
        if (cnts[i] < val && !macro.IsOpen(i))
            val = cnts[i];

    prob p = 0;
    size_t cnt = 0;
    for (size_t i = 0; i < macro.GetN(); i++)
        if (cnts[i] <= val && !macro.IsOpen(i))
        {
            p += BaseSolver::Fork(macro, i, depth);
            cnt++;
        }

    return p / cnt;
}
