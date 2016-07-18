#pragma once
#include "stdafx.h"
#include "Macro.h"

class ProbMacro : public Macro
{
public:
    explicit ProbMacro(size_t width);
    ProbMacro(const ProbMacro &other, size_t id, block_t m);
    ~ProbMacro();

    double OptimalProb;
};

class Solver
{
public:
    explicit Solver(size_t n, size_t m);
    ~Solver();

    double Solve();

private:
    std::unique_ptr<ProbMacro> m_Root;

    static double Fork(const ProbMacro &macro, size_t id);
    static double Fork(ProbMacro &macro);

};
