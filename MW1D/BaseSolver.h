#pragma once
#include "stdafx.h"
#include "Macro.h"
#include <string>
#include <atomic>

class ExtendedMacro : public Macro
{
public:
    explicit ExtendedMacro(size_t width);
    ExtendedMacro(const ExtendedMacro &other, size_t id, block_t m);
    ~ExtendedMacro();

    NO_COPY(ExtendedMacro);
    DEFAULT_MOVE(ExtendedMacro);

    prob Prob;
    std::vector<block_t> Info;
};

class BaseSolver
{
public:
    BaseSolver();
    virtual ~BaseSolver();

    NO_COPY(BaseSolver);
    NO_MOVE(BaseSolver);

    virtual std::string GetDescription() const = 0;

    size_t GetForks() const;

    void LoadData(std::shared_ptr<ExtendedMacro> root, size_t m, bool notRigorous);
    prob Solve(int verbosity);

protected:
    prob Fork(const ExtendedMacro &macro, size_t id, size_t depth);
    virtual prob Fork(ExtendedMacro &macro, size_t depth) = 0;

    void IncrementForks();
    void Log(size_t depth, std::string &&str) const;
    void Log(size_t depth, std::function<std::string()> strFunction) const;

    std::shared_ptr<ExtendedMacro> m_Root;
    size_t m_M;

    bool m_NotRigorous;

private:
    std::atomic<size_t> m_Forks;
    int m_Verbosity;

    prob PreFork(ExtendedMacro &macro, size_t depth);
};
