#pragma once
#include "stdafx.h"
#include "BasicSolver.h"

class SingleSolver : public BasicSolver
{
public:
    SingleSolver(std::shared_ptr<ExtendedMacro> root, size_t m);
    ~SingleSolver();

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;
};
