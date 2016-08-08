#pragma once
#include "stdafx.h"
#include "BaseSolver.h"
#include "FLHeuristic.h"

class FullSolver : public BaseSolver
{
public:
    FullSolver();
    explicit FullSolver(std::shared_ptr<FLHeuristic> heu);
    ~FullSolver();

    std::string GetDescription() const override;

protected:
    prob Fork(ExtendedMacro &macro, size_t depth) override;

    virtual prob Heuristic(ExtendedMacro &macro, size_t depth);

private:
    std::shared_ptr<FLHeuristic> m_Heuristic;
};
