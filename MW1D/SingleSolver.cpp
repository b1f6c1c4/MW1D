#include "stdafx.h"
#include "SingleSolver.h"
#include <sstream>

SingleSolver::SingleSolver() { }

SingleSolver::~SingleSolver() { }

std::string SingleSolver::GetDescription() const
{
    return "Single";
}

prob SingleSolver::Fork(ExtendedMacro &macro, size_t depth)
{
    auto last = macro.GetN() - 1;

    Log(depth, [&]()
        {
            std::stringstream ss;
            for (size_t i = 0; i <= last; i++)
                switch (macro.Info[i])
                {
                case UNKNOWN:
                    ss << '-';
                    break;
                case MINE:
                    ss << 'M';
                    break;
                default:
                    ss << static_cast<char>('0' + macro.Info[i]);
                }
            return "BEFORE:  " + ss.str();
        });

    auto isLogic = true;
    auto toFork = macro.GetN();

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
                {
                    toFork = 1;
                    break;
                }
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
                {
                    toFork = last - 1;
                    break;
                }
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
                {
                    toFork = i + 1;
                    break;
                }
                if (macro.Info[i + 1] == MINE && !macro.IsOpen(i - 1))
                {
                    toFork = i - 1;
                    break;
                }
                if (macro.Info[i - 1] != MINE && macro.IsOpen(i - 1) && macro.Info[i + 1] != MINE)
                {
                    isLogic = true;
                    macro.Info[i + 1] = MINE;
                }
                if (macro.Info[i + 1] != MINE && macro.IsOpen(i + 1) && macro.Info[i - 1] != MINE)
                {
                    isLogic = true;
                    macro.Info[i - 1] = MINE;
                }
            }
            else if (macro.Info[i] == 0)
            {
                if (!macro.IsOpen(i - 1))
                {
                    toFork = i - 1;
                    break;
                }
                if (!macro.IsOpen(i + 1))
                {
                    toFork = i + 1;
                    break;
                }
            }
        }
    }

    Log(depth, [&]()
        {
            std::stringstream ss;
            for (size_t i = 0; i <= last; i++)
                switch (macro.Info[i])
                {
                case UNKNOWN:
                    ss << '-';
                    break;
                case MINE:
                    ss << 'M';
                    break;
                default:
                    ss << static_cast<char>('0' + macro.Info[i]);
                }
            return "AFTER:   " + ss.str();
        });

    if (toFork < macro.GetN())
        return BaseSolver::Fork(macro, toFork, depth);

    IncrementForks();

    prob p = 0;
    size_t cnt = 0;
    for (size_t i = 0; i <= last; i++)
    {
        if (macro.Info[i] != UNKNOWN)
            continue;

        p += BaseSolver::Fork(macro, i, depth);
        cnt++;
    }

    if (cnt == 0)
        return 0;

    Log(depth, [&]()
        {
            return "p=" + to_string(p / cnt);
        });

    return macro.Prob = p / cnt;
}
