#pragma once
#include "stdafx.h"
#include "Macro.h"
#include <atomic>

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

    size_t GetForks() const;

    double Solve();

private:
    std::atomic<size_t> m_Forks;

    std::unique_ptr<ProbMacro> m_Root;

    double Fork(const ProbMacro &macro, size_t id);
    double Fork(ProbMacro &macro);

};
