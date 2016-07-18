#include "FullSolver.h"
#include <algorithm>

FullSolver::FullSolver(size_t n, size_t m) : BasicSolver(n, m) { }

FullSolver::~FullSolver() { }

double FullSolver::Fork(ExtendedMacro &macro)
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
            return BasicSolver::Fork(macro, i);

    return ForkProb(macro, cnts);
}

double FullSolver::ForkProb(ExtendedMacro& macro, const std::vector<size_t> &cnts)
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
            p += BasicSolver::Fork(macro, i);
            cnt++;
        }

    return p / cnt;
}
