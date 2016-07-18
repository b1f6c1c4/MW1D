#include "Macro.h"

Macro::Macro(size_t width) : m_Width(width), m_Mask(width, false) { }

Macro::Macro(const Macro &other, size_t id, block_t m) : MicroSet(other, std::bind(&Macro::BlockOpenFilter, std::placeholders::_1, id, m)), m_Width(other.m_Width), m_Mask(other.m_Mask)
{
    m_Mask[id] = true;
}

Macro::~Macro() { }

size_t Macro::GetWidth() const
{
    return m_Width;
}

bool Macro::IsOpen(size_t id) const
{
    return m_Mask[id];
}

bool Macro::BlockOpenFilter(const Micro &micro, size_t id, block_t m)
{
    if (m == MINE)
        return micro[id];

    if (micro[id])
        return false;

    if (id == 0)
    {
        switch (m)
        {
        case 0:
            return !micro[1];
        case 1:
            return micro[1];
        default:
            return false;
        }
    }

    if (id == micro.size() - 1)
    {
        switch (m)
        {
        case 0:
            return !micro[micro.size() - 2];
        case 1:
            return micro[micro.size() - 2];
        default:
            return false;
        }
    }

    switch (m)
    {
    case 0:
        return !micro[id - 1] && !micro[id + 1];
    case 1:
        return micro[id - 1] ^ micro[id + 1];
    case 2:
        return micro[id - 1] && micro[id + 1];
    default:
        return false;
    }
}
