#pragma once
#include "stdafx.h"
#include "BaseSolver.h"

class ExtendedMacro;

class FLHeuristic
{
public:
    virtual ~FLHeuristic();

    void SetBase(std::shared_ptr<FLHeuristic> base);

    std::string GetDescription() const;

    void Process(const ExtendedMacro &macro, std::vector<size_t> &target);

protected:
    FLHeuristic();

    virtual std::string GetOneDescription() const = 0;

    virtual void ProcessOne(const ExtendedMacro &macro, std::vector<size_t> &target) = 0;

    std::shared_ptr<FLHeuristic> m_Base;
};

template <typename T>
class FLMinimal : public FLHeuristic
{
protected:
    void ProcessOne(const ExtendedMacro &macro, std::vector<size_t> &target) override;

    virtual T GetValue(const ExtendedMacro &macro, std::vector<size_t> &target, size_t id) = 0;
};

template <typename T>
void FLMinimal<T>::ProcessOne(const ExtendedMacro &macro, std::vector<size_t> &target)
{
    auto nt = target.size();

    std::vector<T> vals;
    vals.reserve(nt);

    for (size_t i = 0; i < nt; i++)
        vals.push_back(GetValue(macro, target, target[i]));

    auto val = vals.front();

    for (size_t i = 1; i < nt; i++)
        if (vals[i] < val && !macro.IsOpen(target[i]))
            val = vals[i];

    std::vector<size_t> targ;
    for (size_t i = 0; i < nt; i++)
        if (!(val < vals[i]))
            targ.push_back(target[i]);

    target.swap(targ);
}

class FLP : public FLMinimal<prob>
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const ExtendedMacro &macro, std::vector<size_t> &target, size_t id) override;
};

class FLDist : public FLMinimal<prob>
{
protected:
    prob GetValue(const ExtendedMacro &macro, std::vector<size_t> &target, size_t id) override;

    virtual prob GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id) = 0;
};

class FLU : public FLDist
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id) override;
};

class FLSE : public FLDist
{
protected:
    prob GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id) override;

    virtual prob GetValue(const std::vector<prob> &safeDist) = 0;
};

class FLS : public FLSE
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const std::vector<prob> &safeDist) override;
};

class FLE : public FLSE
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const std::vector<prob> &safeDist) override;
};

class FLQ : public FLDist
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id) override;

private:
    static prob Log(prob value);
};

class FLZ : public FLDist
{
protected:
    std::string GetOneDescription() const override;

    prob GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id) override;
};

std::shared_ptr<FLHeuristic> CreateHeuristic(const std::string &str);
