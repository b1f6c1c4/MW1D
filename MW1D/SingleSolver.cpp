#include "stdafx.h"
#include "SingleSolver.h"
#include <sstream>

SingleSolver::SingleSolver(bool extended) : m_Extended(extended) { }

SingleSolver::~SingleSolver() { }

std::string SingleSolver::GetDescription() const
{
    if (m_Extended)
        return "Single Logic Extended";
    return "Single Logic";
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
        if (toFork < macro.GetN())
            break;

        if (m_Extended && m_M >= 0)
        {
            size_t cnt = 0;
            for (size_t i = 0; i <= last; i++)
                if (macro.Info[i] == MINE)
                    cnt++;

            if (last >= 2 && macro.Info[0] >= 0 && macro.Info[0] + cnt - (macro.Info[1] == MINE ? 1 : 0) == m_M)
            {
                for (size_t j = 2; j <= last; j++)
                    if (!macro.IsOpen(j) && macro.Info[j] != MINE)
                    {
                        toFork = j;
                        break;
                    }
                if (toFork < macro.GetN())
                    break;
            }
            if (last >= 2 && macro.Info[last] >= 0 && macro.Info[last] + cnt - (macro.Info[last - 1] == MINE ? 1 : 0) == m_M)
            {
                for (size_t j = 0; j <= last - 2; j++)
                    if (!macro.IsOpen(j) && macro.Info[j] != MINE)
                    {
                        toFork = j;
                        break;
                    }
                if (toFork < macro.GetN())
                    break;
            }
            for (size_t i = 1; i < last; i++)
            {
                if (macro.Info[i] >= 0 && macro.Info[i] + cnt - (macro.Info[i - 1] == MINE ? 1 : 0) - (macro.Info[i + 1] == MINE ? 1 : 0) == m_M)
                {
                    if (i >= 2)
                        for (size_t j = 0; j <= i - 2; j++)
                            if (!macro.IsOpen(j) && macro.Info[j] != MINE)
                            {
                                toFork = j;
                                break;
                            }
                    if (toFork < macro.GetN())
                        break;
                    if (last >= i + 2)
                        for (size_t j = i + 2; j <= last; j++)
                            if (!macro.IsOpen(j) && macro.Info[j] != MINE)
                            {
                                toFork = j;
                                break;
                            }
                    if (toFork < macro.GetN())
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
