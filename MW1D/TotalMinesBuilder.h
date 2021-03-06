#pragma once
#include "stdafx.h"
#include "MicroSetBuilder.h"

class TotalMinesBuilder : public MicroSetBuilder
{
public:
    TotalMinesBuilder(size_t n, size_t m);
    ~TotalMinesBuilder();

    NO_COPY(TotalMinesBuilder);
    NO_MOVE(TotalMinesBuilder);

    size_t GetM() const override;

protected:
    void First(Micro &set) override;
    bool Next(Micro &set) override;
    prob CalcProb(const Micro &set) override;

private:
    size_t m_M;
};
