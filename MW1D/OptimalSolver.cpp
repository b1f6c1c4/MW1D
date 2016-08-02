#include "stdafx.h"
#include "OptimalSolver.h"

OptimalSolver::OptimalSolver(size_t n, size_t m) : BasicSolver(n, m) { }

OptimalSolver::~OptimalSolver() { }

double OptimalSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    if (macro.size() == 1)
        return 1;

    IncrementForks();

    double pMax = 0;
    for (size_t id = 0; id < macro.GetWidth(); id++)
    {
        if (macro.IsOpen(id))
            continue;

        auto pf = BasicSolver::Fork(macro, id, depth);
        if (pf > pMax)
            pMax = pf;
    }
    return macro.Prob = pMax;
}
