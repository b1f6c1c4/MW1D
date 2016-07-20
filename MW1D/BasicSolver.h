#pragma once
#include "stdafx.h"
#include "Macro.h"
#include <string>
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

    double Solve(bool verbose = false);

protected:
    double Fork(const ExtendedMacro &macro, size_t id, size_t depth);
    virtual double Fork(ExtendedMacro &macro, size_t depth) = 0;

    void IncrementForks();
    void Log(size_t depth, std::string &&str) const;
    void Log(size_t depth, std::function<std::string()> strFunction) const;

    std::unique_ptr<ExtendedMacro> m_Root;
    size_t m_M;

private:
    std::atomic<size_t> m_Forks;
    bool m_Verbose;
};
