#pragma once
#include "stdafx.h"
#include "Macro.h"
#include <string>
#include <atomic>
#include "../../CppUtil/CppUtil/CopyMove.hpp"

#define UNKNOWN static_cast<block_t>(-2)
#define UNCERTAIN static_cast<size_t>(-1)

class ExtendedMacro : public Macro
{
public:
    explicit ExtendedMacro(size_t width);
    ExtendedMacro(const ExtendedMacro &other, size_t id, block_t m);
    ~ExtendedMacro();

    NO_COPY(ExtendedMacro);
    NO_MOVE(ExtendedMacro);

    prob Prob;
    std::vector<block_t> Info;
};

class BasicSolver
{
public:
    BasicSolver(std::shared_ptr<ExtendedMacro> root, size_t m);
    virtual ~BasicSolver();

    NO_COPY(BasicSolver);
    NO_MOVE(BasicSolver);

    size_t GetForks() const;

    prob Solve(bool verbose = false);

protected:
    prob Fork(const ExtendedMacro &macro, size_t id, size_t depth);
    virtual prob Fork(ExtendedMacro &macro, size_t depth) = 0;

    void IncrementForks();
    void Log(size_t depth, std::string &&str) const;
    void Log(size_t depth, std::function<std::string()> strFunction) const;

    std::shared_ptr<ExtendedMacro> m_Root;
    size_t m_M;

private:
    std::atomic<size_t> m_Forks;
    bool m_Verbose;
};
