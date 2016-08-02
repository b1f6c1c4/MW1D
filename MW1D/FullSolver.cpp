#include "stdafx.h"
#include "FullSolver.h"
#include <algorithm>

FullSolver::FullSolver(size_t n, size_t m) : BasicSolver(n, m) { }

FullSolver::~FullSolver() { }

double FullSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    if (macro.size() == 1)
        return 1;

    std::vector<size_t> cnts(macro.GetWidth(), 0);
    for (auto it = macro.begin(); it != macro.end(); ++it)
        for (size_t i = 0; i < macro.GetWidth(); i++)
            if ((*it)[i])
                cnts[i]++;

    IncrementForks();

    for (size_t i = 0; i < macro.GetWidth(); i++)
        if (cnts[i] == 0 && !macro.IsOpen(i))
            return BasicSolver::Fork(macro, i, depth);

    return ForkProb(macro, cnts, depth);
}

double FullSolver::ForkProb(ExtendedMacro &macro, const std::vector<size_t> &cnts, size_t depth)
{
    auto val = macro.size();

    for (size_t i = 0; i < macro.GetWidth(); i++)
        if (cnts[i] < val && !macro.IsOpen(i))
            val = cnts[i];

    double p = 0;
    size_t cnt = 0;
    for (size_t i = 0; i < macro.GetWidth(); i++)
        if (cnts[i] <= val && !macro.IsOpen(i))
        {
            p += BasicSolver::Fork(macro, i, depth);
            cnt++;
        }

    return p / cnt;
}
