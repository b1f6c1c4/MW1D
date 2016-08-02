#pragma once
#include "stdafx.h"
#include "BaseSolver.h"

class OptimalSolver : public BaseSolver
{
public:
    OptimalSolver();
    ~OptimalSolver();

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;
};
