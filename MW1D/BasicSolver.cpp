#include "stdafx.h"
#include "BasicSolver.h"
#include <iostream>

ExtendedMacro::ExtendedMacro(size_t width) : Macro(width), Prob(-1), Info(width, UNKNOWN) { }

ExtendedMacro::ExtendedMacro(const ExtendedMacro &other, size_t id, block_t m) : Macro(other, id, m), Prob(-1), Info(other.Info)
{
    Info[id] = m;
}

ExtendedMacro::~ExtendedMacro() { }

BasicSolver::BasicSolver(std::shared_ptr<ExtendedMacro> root, size_t m) : m_Root(root), m_M(m), m_Forks(0), m_Verbose(false) { }

BasicSolver::~BasicSolver() { }

size_t BasicSolver::GetForks() const
{
    return m_Forks.load();
}

void BasicSolver::IncrementForks()
{
    ++m_Forks;
}

void BasicSolver::Log(size_t depth, std::string &&str) const
{
    if (m_Verbose)
        std::cout << std::string(depth, '\t') << str << std::endl;
}

void BasicSolver::Log(size_t depth, std::function<std::string()> strFunction) const
{
    if (m_Verbose)
        std::cout << std::string(depth, '\t') << strFunction() << std::endl;
}

prob BasicSolver::Solve(bool verbose)
{
    m_Verbose = verbose;
    return Fork(*m_Root, 0);
}

prob BasicSolver::Fork(const ExtendedMacro &macro, size_t id, size_t depth)
{
    prob p0 = 0;

    block_t lst[] = {0, 1, 2};

    Log(depth, [&]()
        {
            return "Fork #" + std::to_string(id) + ":";
        });

    for (auto m : lst)
    {
        ExtendedMacro newMacro(macro, id, m);
        if (newMacro.size() > 0)
        {
            auto p = newMacro.GetTotalProb();

            Log(depth, [&]()
                {
                    return "  case " + std::to_string(static_cast<int>(m)) +
                        " (p=" + to_string(p / macro.GetTotalProb()) + "):";
                });
            auto pf = Fork(newMacro, depth + 1);
            p0 += p * pf;
        }
    }

    p0 /= macro.GetTotalProb();

    Log(depth, [&]()
        {
            return "Join #" + std::to_string(id) + ": p=" + to_string(p0);
        });

    return p0;
}
