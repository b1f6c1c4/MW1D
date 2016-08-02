#include "stdafx.h"
#include "OptimalSolver.h"

OptimalSolver::OptimalSolver(std::shared_ptr<ExtendedMacro> root, size_t m) : BasicSolver(root, m) { }

OptimalSolver::~OptimalSolver() { }

prob OptimalSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    if (macro.size() == 1)
        return 1;

    IncrementForks();

    prob pMax = 0;
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
