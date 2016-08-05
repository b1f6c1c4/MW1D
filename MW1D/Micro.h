#pragma once
#include "stdafx.h"
#include <functional>
#include <memory>

class Micro
{
public:
    Micro();
    Micro(const Micro &other);
    Micro(Micro &&other);

    Micro &operator=(const Micro &) = delete;
    Micro &operator=(Micro &&) = delete;

    Micro &DeepClone(const Micro &other);

    bit_reference operator[](size_t id);
    bit_const_reference operator[](size_t id) const;

    size_t size() const;
    void resize(size_t sz, bool val);

    prob GetProb() const;
    void SetProb(prob value);

private:
    std::shared_ptr<bits> m_Bits;
    prob m_PriorProb;
};

typedef std::function<bool(const Micro &)> filter_t;

class MicroSet
{
public:
    MicroSet();
    MicroSet(const MicroSet &other);
    MicroSet(const MicroSet &other, filter_t filter);
    virtual ~MicroSet();

    Micro &operator=(const Micro &) = delete;
    NO_MOVE(MicroSet);

    size_t size() const;
    std::vector<Micro>::const_iterator begin() const;
    std::vector<Micro>::const_iterator end() const;

    void Emplace(const Micro &m);
    void Emplace(Micro &&m);

    prob GetTotalProb() const;

private:
    std::vector<Micro> m_Set;
    prob m_TotalProb;
};
