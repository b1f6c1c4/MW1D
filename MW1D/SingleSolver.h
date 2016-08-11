#pragma once
#include "stdafx.h"
#include "BaseSolver.h"

class SingleSolver : public BaseSolver
{
public:
    explicit SingleSolver(bool extended = false);
    ~SingleSolver();

    std::string GetDescription() const override;

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;

private:
    bool m_Extended;
};
