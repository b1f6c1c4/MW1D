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

#ifdef USE_CAS
    auto pMax = std::make_shared<MergedExpression>("Max");
#else
    prob pMax = 0;
#endif
    for (size_t id = 0; id < macro.GetN(); id++)
    {
        if (macro.IsOpen(id))
            continue;

        auto pf = BaseSolver::Fork(macro, id, depth);
#ifdef USE_CAS
        pMax->push_back(pf);
#else
        if (pf > pMax)
            pMax = pf;
#endif
    }
#ifdef USE_CAS
    return macro.Prob = std::shared_ptr<IExpression>(pMax);
#else
    return macro.Prob = pMax;
#endif
}
