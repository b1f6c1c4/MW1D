#include "Solver.h"

void First(bits &set, size_t n, size_t m)
{
    set.resize(n, false);
    for (auto i = n - m; i < n; i++)
        set[i] = true;
}

bool Next(bits &set)
{
    auto cntM = 0;
    while (cntM < set.size())
    {
        if (!set[cntM])
            break;
        cntM++;
    }

    auto id = cntM;
    while (id < set.size())
    {
        if (set[id])
            break;
        id++;
    }

    if (id == set.size())
        return false;

    set[id] = false;

    for (auto i = id - 1 - cntM; i < id; i++)
        set[i] = true;

    for (auto i = 0; i < cntM; i++)
        set[i] = false;

    return true;
}

ProbMacro::ProbMacro(size_t width) : Macro(width), OptimalProb(NAN) { }

ProbMacro::ProbMacro(const ProbMacro& other, size_t id, block_t m) : Macro(other, id, m), OptimalProb(NAN) { }

ProbMacro::~ProbMacro() { }

Solver::Solver(size_t n, size_t m) : m_Root(std::make_unique<ProbMacro>(n))
{
    Micro micro;
    First(micro, n, m);
    while (true)
    {
        m_Root->Emplace(micro);
        if (!Next(micro))
            break;
    }
}

Solver::~Solver() { }

double Solver::Solve()
{
    return Fork(*m_Root);
}

double Solver::Fork(const ProbMacro &macro, size_t id)
{
    double p0 = 0;

    block_t lst[] = { MINE, 0, 1, 2 };
    for (auto m : lst)
    {
        ProbMacro newMacro(macro, id, m);
        if (newMacro.Size() > 0)
        {
            auto p = static_cast<double>(newMacro.Size()) / macro.Size();
            auto pf = Fork(newMacro);
            p0 += p * pf;
        }
    }

    return p0;
}

double Solver::Fork(ProbMacro &macro)
{
    double pMax = 0;
    for (size_t id = 0; id < macro.Size(); id++)
    {
        auto pf = Fork(macro);
        if (pf > pMax)
            pMax = pf;
    }
    return macro.OptimalProb = pMax;
}
