#pragma once
#include "stdafx.h"
#include "Micro.h"
#include "../../CppUtil/CppUtil/CopyMove.hpp"

class MicroSetBuilder
{
public:
    virtual ~MicroSetBuilder();

    NO_COPY(MicroSetBuilder);
    NO_MOVE(MicroSetBuilder);

    void Build(MicroSet &set);

protected:
    MicroSetBuilder();

    virtual void First(Micro &set) = 0;
    virtual bool Next(Micro &set) = 0;
    virtual prob CalcProb(const Micro &set) = 0;
};
