#pragma once
#include "stdafx.h"
#include "BaseSolver.h"

class SingleSolver : public BaseSolver
{
public:
    SingleSolver();
    ~SingleSolver();

    std::string GetDescription() const override;

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;
};
