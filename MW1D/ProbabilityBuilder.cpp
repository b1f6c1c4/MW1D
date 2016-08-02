#include "stdafx.h"
#include "ProbabilityBuilder.h"

ProbabilityBuilder::ProbabilityBuilder(size_t n, prob p) : m_N(n), m_P(p) { }

ProbabilityBuilder::~ProbabilityBuilder() { }

void ProbabilityBuilder::First(Micro &set)
{
    set.resize(m_N, false);
}

bool ProbabilityBuilder::Next(Micro &set)
{
    for (auto i = 0; i < m_N; i++)
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
    prob p = 1;
    for (auto i = 0; i < m_N; i++)
        if (set[i])
            p *= m_P;
        else
            p *= 1 - m_P;
    return p;
}
