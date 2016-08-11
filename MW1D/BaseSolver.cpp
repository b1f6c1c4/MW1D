#include "stdafx.h"
#include "BaseSolver.h"
#include <iostream>

ExtendedMacro::ExtendedMacro(size_t width) : Macro(width), Prob(-1), Info(width, UNKNOWN) { }

ExtendedMacro::ExtendedMacro(const ExtendedMacro &other, size_t id, block_t m) : Macro(other, id, m), Prob(-1), Info(other.Info)
{
    Info[id] = m;
}

ExtendedMacro::~ExtendedMacro() { }

BaseSolver::BaseSolver() : m_Root(nullptr), m_M(UNCERTAIN), m_NotRigorous(false), m_Forks(0), m_Verbosity(-2) { }

BaseSolver::~BaseSolver() { }

size_t BaseSolver::GetForks() const
{
    return m_Forks.load();
}

void BaseSolver::LoadData(std::shared_ptr<ExtendedMacro> root, size_t m, bool notRigorous)
{
    m_Root = root;
    m_M = m;
    m_NotRigorous = notRigorous;
}

void BaseSolver::IncrementForks()
{
    ++m_Forks;
}

void BaseSolver::Log(size_t depth, std::string &&str) const
{
    if (m_Verbosity >= 0)
        if (m_Verbosity >= depth)
            std::cout << std::string(depth, '\t') << str << std::endl;
}

void BaseSolver::Log(size_t depth, std::function<std::string()> strFunction) const
{
    if (m_Verbosity >= 0)
        if (m_Verbosity >= depth)
            std::cout << std::string(depth, '\t') << strFunction() << std::endl;
}

prob BaseSolver::PreFork(ExtendedMacro &macro, size_t depth)
{
    auto &&p = Fork(macro, depth);

    for (auto &micro : macro)
    {
        size_t i = 0;
        for (i = 0; i < micro.size(); i++)
            if (!macro.IsOpen(i) && !micro[i])
                break;
        if (i == micro.size())
        {
            Log(depth, [&]()
                {
                    return "Modify: p=" + to_string(p) + " -> p=" + to_string(p + micro.GetProb() / macro.GetTotalProb());
                });

            p += micro.GetProb() / macro.GetTotalProb();
            return p;
        }
    }

    Log(depth, [&]()
        {
            return "No modify: p=" + to_string(p);
        });
    return p;
}

prob BaseSolver::Solve(int verbosity)
{
    m_Verbosity = verbosity;
    return PreFork(*m_Root, 0);
}

prob BaseSolver::Fork(const ExtendedMacro &macro, size_t id, size_t depth)
{
    prob p0 = 0;

    block_t lst[] = {0, 1, 2};

    Log(depth, [&]()
        {
            return "Fork #" + std::to_string(id) + ":";
        });

    auto ptr = &macro;
    if (depth == 0 && m_NotRigorous)
        ptr = new ExtendedMacro(macro, id, NOMINE);

    for (auto m : lst)
    {
        ExtendedMacro newMacro(*ptr, id, m);
        if (newMacro.size() > 0)
        {
            auto &&p = newMacro.GetTotalProb();

            Log(depth, [&]()
                {
                    return "  case " + std::to_string(static_cast<int>(m)) +
                        " (p=" + to_string(p / ptr->GetTotalProb()) + "):";
                });
            auto &&pf = PreFork(newMacro, depth + 1);
            p0 += p * pf;
        }
    }

    p0 /= ptr->GetTotalProb();

    Log(depth, [&]()
        {
            return "Join #" + std::to_string(id) + ": p=" + to_string(p0);
        });

    return p0;
}
