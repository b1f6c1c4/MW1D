#pragma once
#include "stdafx.h"
#include "Micro.h"

#define MINE -1
typedef char block_t;

class Macro : public MicroSet
{
public:
    explicit Macro(size_t width);
    Macro(const Macro &other, size_t id, block_t m);
    ~Macro();

private:

    bits m_Mask;

    static bool BlockOpenFilter(const Micro &micro, size_t id, block_t m);
};

inline Macro::Macro(size_t width) : m_Mask(width, false) { }

inline Macro::Macro(const Macro &other, size_t id, block_t m) : MicroSet(other, std::bind(&Macro::BlockOpenFilter, std::placeholders::_1, id, m)), m_Mask(other.m_Mask)
{
    m_Mask[id] = true;
}

inline Macro::~Macro() { }

inline bool Macro::BlockOpenFilter(const Micro &micro, size_t id, block_t m)
{
    if (m == MINE)
        return micro[id];
    auto cnt = 0;
    if (id > 0)
        cnt += micro[id - 1] ? 1 : 0;
    if (id < micro.size() - 1)
        cnt += micro[id + 1] ? 1 : 0;
    return cnt == m;
}
