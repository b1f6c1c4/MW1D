#pragma once
#include "stdafx.h"
#include "BaseSolver.h"

class FullSolver : public BaseSolver
{
public:
    FullSolver();
    ~FullSolver();

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;

    virtual prob ForkProb(ExtendedMacro &macro, const std::vector<size_t> &cnts, size_t depth);
};
