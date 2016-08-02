#pragma once
#include "stdafx.h"
#include "MicroSetBuilder.h"
#include "../../CppUtil/CppUtil/CopyMove.hpp"

class TotalMinesBuilder : public MicroSetBuilder
{
public:
    TotalMinesBuilder(size_t n, size_t m);
    ~TotalMinesBuilder();

    NO_COPY(TotalMinesBuilder);
    NO_MOVE(TotalMinesBuilder);

protected:
    void First(Micro &set) override;
    bool Next(Micro &set) override;
    prob CalcProb(const Micro &set) override;

private:
    size_t m_N;
    size_t m_M;
};
