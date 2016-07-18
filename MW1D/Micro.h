#pragma once
#include "stdafx.h"
#include <functional>
#include <memory>

#ifdef SHARE_MICRO
typedef std::shared_ptr<bits> Micro;
#else
typedef bits Micro;
#endif

typedef std::function<bool(const Micro &)> filter_t;

class MicroSet
{
public:
    MicroSet();
    MicroSet(const MicroSet &other);
    MicroSet(const MicroSet &other, filter_t filter);
    virtual ~MicroSet();

    size_t Size() const;

    void Emplace(const Micro &m);
    void Emplace(Micro &&m);

private:
    std::vector<Micro> m_Set;
};

inline MicroSet::MicroSet() { }

inline MicroSet::MicroSet(const MicroSet &other) : m_Set(other.m_Set) { }

inline MicroSet::MicroSet(const MicroSet &other, filter_t filter)
{
    for (auto &m : other.m_Set)
        if (filter(m))
            m_Set.push_back(m);
}

inline MicroSet::~MicroSet() { }

inline size_t MicroSet::Size() const
{
    return m_Set.size();
}

inline void MicroSet::Emplace(const Micro &m)
{
    m_Set.emplace_back(m);
}

inline void MicroSet::Emplace(Micro &&m)
{
    m_Set.emplace_back(std::move(m));
}
