#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class FullSolver : public BasicSolver
{
public:
    FullSolver(std::shared_ptr<ExtendedMacro> root, size_t m);
    ~FullSolver();

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;

    virtual prob ForkProb(ExtendedMacro &macro, const std::vector<size_t> &cnts, size_t depth);
};
