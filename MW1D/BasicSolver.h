#pragma once
#include "stdafx.h"
#include "Macro.h"
#include <atomic>

#define UNKNOWN static_cast<block_t>(-2)

class ExtendedMacro : public Macro
{
public:
    explicit ExtendedMacro(size_t width);
    ExtendedMacro(const ExtendedMacro &other, size_t id, block_t m);
    ~ExtendedMacro();

    double Prob;
    std::vector<block_t> Info;
};

class BasicSolver
{
public:
    explicit BasicSolver(size_t n, size_t m);
    virtual ~BasicSolver();

    size_t GetForks() const;

    double Solve();

protected:
    double Fork(const ExtendedMacro &macro, size_t id);
    virtual double Fork(ExtendedMacro &macro) = 0;

    void IncrementForks();

    std::unique_ptr<ExtendedMacro> m_Root;
    size_t m_M;

private:
    std::atomic<size_t> m_Forks;
};
