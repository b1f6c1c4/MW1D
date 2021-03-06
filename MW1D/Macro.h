#pragma once
#include "stdafx.h"
#include "Micro.h"

typedef char block_t;
#define MINE static_cast<block_t>(-1)
#define UNKNOWN static_cast<block_t>(-2)
#define NOMINE static_cast<block_t>(-3)

class Macro : public MicroSet
{
public:
    explicit Macro(size_t width);
    Macro(const Macro &other, size_t id, block_t m, bool open = true);
    ~Macro();

    NO_COPY(Macro);
    DEFAULT_MOVE(Macro);

    size_t GetN() const;
    bool IsOpen(size_t id) const;

    bit_reference operator[](size_t id);
    bit_const_reference operator[](size_t id) const;

private:
    size_t m_Width;

    bits m_Mask;

    static bool BlockOpenFilter(const Micro &micro, size_t id, block_t m);
};
