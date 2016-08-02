#include "stdafx.h"
#include "ProbabilityBuilder.h"

ProbabilityBuilder::ProbabilityBuilder(size_t n, prob p) : MicroSetBuilder(n), m_P(p) { }

ProbabilityBuilder::~ProbabilityBuilder() { }

size_t ProbabilityBuilder::GetM() const
{
    return UNCERTAIN;
}

void ProbabilityBuilder::First(Micro &set)
{
    auto n = GetN();
    if (m_P == 1)
        set.resize(n, true);
    else
        set.resize(n, false);
}

bool ProbabilityBuilder::Next(Micro &set)
{
    if (m_P == 0 || m_P == 1)
        return false;

    auto n = GetN();
    for (auto i = 0; i < n; i++)
        if (set[i])
            set[i] = 0;
        else
        {
            set[i] = 1;
            return true;
        }

    return false;
}

prob ProbabilityBuilder::CalcProb(const Micro &set)
{
    auto n = GetN();
    prob p = 1;
    for (auto i = 0; i < n; i++)
        if (set[i])
            p *= m_P;
        else
            p *= 1 - m_P;
    return p;
}
