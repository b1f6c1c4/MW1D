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

    size_t GetWidth() const;
    bool IsOpen(size_t id) const;

private:
    size_t m_Width;

    bits m_Mask;

    static bool BlockOpenFilter(const Micro &micro, size_t id, block_t m);
};
