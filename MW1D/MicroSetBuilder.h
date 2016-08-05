#pragma once
#include "stdafx.h"
#include "Micro.h"

class MicroSetBuilder
{
public:
    virtual ~MicroSetBuilder();

    NO_COPY(MicroSetBuilder);
    NO_MOVE(MicroSetBuilder);

    void Build(MicroSet &set);
    size_t GetN() const;
    virtual size_t GetM() const = 0;

protected:
    explicit MicroSetBuilder(size_t n);

    virtual void First(Micro &set) = 0;
    virtual bool Next(Micro &set) = 0;
    virtual prob CalcProb(const Micro &set) = 0;

private:
    size_t m_N;
};
