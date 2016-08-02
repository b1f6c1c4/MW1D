#include "stdafx.h"
#include "MicroSetBuilder.h"

MicroSetBuilder::MicroSetBuilder() { }

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
