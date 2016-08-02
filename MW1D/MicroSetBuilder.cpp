#include "stdafx.h"
#include "MicroSetBuilder.h"

MicroSetBuilder::MicroSetBuilder(size_t n) : m_N(n) { }

MicroSetBuilder::~MicroSetBuilder() { }

void MicroSetBuilder::Build(MicroSet &set)
{
    Micro micro;
    First(micro);
    while (true)
    {
        Micro microT;
        microT.DeepClone(micro);
        microT.SetProb(CalcProb(micro));
        set.Emplace(microT);

        if (!Next(micro))
            break;
    }
}

size_t MicroSetBuilder::GetN() const
{
    return m_N;
}
