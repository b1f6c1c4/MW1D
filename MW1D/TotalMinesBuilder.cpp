#include "stdafx.h"
#include "TotalMinesBuilder.h"

TotalMinesBuilder::TotalMinesBuilder(size_t n, size_t m) : MicroSetBuilder(n), m_M(m) { }

TotalMinesBuilder::~TotalMinesBuilder() { }

size_t TotalMinesBuilder::GetM() const
{
    return m_M;
}

void TotalMinesBuilder::First(Micro &set)
{
    auto n = GetN();
    set.resize(n, false);
    for (auto i = n - m_M; i < n; i++)
        set[i] = true;
}

bool TotalMinesBuilder::Next(Micro &set)
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

prob TotalMinesBuilder::CalcProb(const Micro &set)
{
    return 1;
}
