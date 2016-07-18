#include "BasicSolver.h"

template<typename T>
void First(T &set, size_t n, size_t m)
{
    set.resize(n, false);
    for (auto i = n - m; i < n; i++)
        set[i] = true;
}

template<typename T>
bool Next(T &set)
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

    for (auto i = 0; i < cntM; i++)
        set[i] = false;

    for (auto i = id - 1 - cntM; i < id; i++)
        set[i] = true;

    return true;
}

ExtendedMacro::ExtendedMacro(size_t width) : Macro(width), Prob(NAN), Info(width, UNKNOWN) { }

ExtendedMacro::ExtendedMacro(const ExtendedMacro& other, size_t id, block_t m) : Macro(other, id, m), Prob(NAN), Info(other.Info)
{
    Info[id] = m;
}

ExtendedMacro::~ExtendedMacro() { }

BasicSolver::BasicSolver(size_t n, size_t m) : m_Forks(0), m_Root(std::make_unique<ExtendedMacro>(n)), m_M(m)
{
    Micro micro;
    First(micro, n, m);
    while (true)
    {
        Micro microT;
        microT.DeepClone(micro);
        m_Root->Emplace(microT);

        if (!Next(micro))
            break;
    }
}

BasicSolver::~BasicSolver() { }

size_t BasicSolver::GetForks() const
{
    return m_Forks.load();
}

void BasicSolver::IncrementForks()
{
    ++m_Forks;
}

double BasicSolver::Solve()
{
    return Fork(*m_Root);
}

double BasicSolver::Fork(const ExtendedMacro &macro, size_t id)
{
    double p0 = 0;

    block_t lst[] = { 0, 1, 2 };
    for (auto m : lst)
    {
        ExtendedMacro newMacro(macro, id, m);
        if (newMacro.Size() > 0)
        {
            auto p = static_cast<double>(newMacro.Size()) / macro.Size();
            auto pf = Fork(newMacro);
            p0 += p * pf;
        }
    }

    return p0;
}
