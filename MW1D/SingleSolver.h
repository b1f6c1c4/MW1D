#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class SingleSolver : public BasicSolver
{
public:
    explicit SingleSolver(size_t n, size_t m);
    ~SingleSolver();

protected:
    double Fork(ExtendedMacro &macro, size_t depth) override;
};
