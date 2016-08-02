#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class OptimalSolver : public BasicSolver
{
public:
    OptimalSolver(std::shared_ptr<ExtendedMacro> root, size_t m);
    ~OptimalSolver();

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;
};
