#include "SingleSolver.h"

SingleSolver::SingleSolver(size_t n, size_t m) : BasicSolver(n, m) { }

SingleSolver::~SingleSolver() { }

double SingleSolver::Fork(ExtendedMacro &macro)
{
    auto last = macro.GetWidth() - 1;

    auto isLogic = true;
    while (isLogic)
    {
        isLogic = false;
        {
            if (macro.Info[0] == 1)
            {
                if (macro.Info[1] != MINE)
                {
                    isLogic = true;
                    macro.Info[1] = MINE;
                }
            }
            else if (macro.Info[0] == 0)
            {
                if (!macro.IsOpen(1))
                    return BasicSolver::Fork(macro, 1);
            }
        }
        {
            if (macro.Info[last] == 1)
            {
                if (macro.Info[last - 1] != MINE)
                {
                    isLogic = true;
                    macro.Info[last - 1] = MINE;
                }
            }
            else if (macro.Info[last] == 0)
            {
                if (!macro.IsOpen(last - 1))
                    return BasicSolver::Fork(macro, last - 1);
            }
        }
        for (size_t i = 1; i < last; i++)
        {
            if (macro.Info[i] == 2)
            {
                if (macro.Info[i - 1] != MINE)
                {
                    isLogic = true;
                    macro.Info[i - 1] = MINE;
                }
                if (macro.Info[i + 1] != MINE)
                {
                    isLogic = true;
                    macro.Info[i + 1] = MINE;
                }
            }
            else if (macro.Info[i] == 1)
            {
                if (macro.Info[i - 1] == MINE && !macro.IsOpen(i + 1))
                    return BasicSolver::Fork(macro, i + 1);
                if (macro.Info[i + 1] == MINE && !macro.IsOpen(i - 1))
                    return BasicSolver::Fork(macro, i - 1);
            }
            else if (macro.Info[i] == 0)
            {
                if (!macro.IsOpen(i - 1))
                    return BasicSolver::Fork(macro, i - 1);
                if (!macro.IsOpen(i + 1))
                    return BasicSolver::Fork(macro, i + 1);
            }
        }
    }

    if (macro.Size() == 1)
    {
        size_t cnt = 0;
        for (size_t i = 0; i <= last; i++)
            if (macro.Info[i] == UNKNOWN || macro.Info[i] == MINE)
                cnt++;
        if (cnt == m_M)
            return 1;
    }

    IncrementForks();

    double p = 0;
    size_t cnt = 0;
    for (size_t i = 0; i <= last; i++)
    {
        if (macro.Info[i] != UNKNOWN)
            continue;

        p += BasicSolver::Fork(macro, i);
        cnt++;
    }

    return macro.Prob = p / cnt;
}
