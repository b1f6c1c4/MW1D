#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class FullSolver : public BasicSolver
{
public:
    explicit FullSolver(size_t n, size_t m);
    ~FullSolver();

protected:
    double Fork(ExtendedMacro &macro) override;

    virtual double ForkProb(ExtendedMacro &macro, const std::vector<size_t> &cnts);
};
