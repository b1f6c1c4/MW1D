#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class OptimalSolver : public BasicSolver
{
public:
    explicit OptimalSolver(size_t n, size_t m);
    ~OptimalSolver();

protected:
    double Fork(ExtendedMacro &macro, size_t depth) override;
};
