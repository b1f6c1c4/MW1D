#pragma once
#include "stdafx.h"
#include "MicroSetBuilder.h"
#include "../../CppUtil/CppUtil/CopyMove.hpp"

class ProbabilityBuilder : public MicroSetBuilder
{
public:
    ProbabilityBuilder(size_t n, prob p);
    ~ProbabilityBuilder();

    NO_COPY(ProbabilityBuilder);
    NO_MOVE(ProbabilityBuilder);

    size_t GetM() const override;

protected:
    void First(Micro &set) override;
    bool Next(Micro &set) override;
    prob CalcProb(const Micro &set) override;

private:
    prob m_P;
};
