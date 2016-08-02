#include "stdafx.h"
#include "OptimalSolver.h"

OptimalSolver::OptimalSolver() { }

OptimalSolver::~OptimalSolver() { }

std::string OptimalSolver::GetDescription() const
{
    return "Optimal";
}

prob OptimalSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    if (macro.size() == 1)
        return 1;

    IncrementForks();

    prob pMax = 0;
    for (size_t id = 0; id < macro.GetN(); id++)
    {
        if (macro.IsOpen(id))
            continue;

        auto pf = BaseSolver::Fork(macro, id, depth);
        if (pf > pMax)
            pMax = pf;
    }
    return macro.Prob = pMax;
}
