#include "stdafx.h"
#include "Micro.h"
#include <algorithm>
#include <iterator>

Micro::Micro() : m_Bits(std::make_shared<bits>()), m_PriorProb(0) { }

Micro::Micro(const Micro &other) : m_Bits(other.m_Bits), m_PriorProb(other.m_PriorProb) { }

Micro::Micro(Micro &&other) : m_Bits(nullptr), m_PriorProb(other.m_PriorProb)
{
    m_Bits.swap(other.m_Bits);
}

Micro &Micro::DeepClone(const Micro &other)
{
    if (this == &other)
        return *this;
    m_Bits = std::make_shared<bits>(*other.m_Bits);
    return *this;
}

bit_reference Micro::operator[](size_t id)
{
    return m_Bits->operator[](id);
}

bit_const_reference Micro::operator[](size_t id) const
{
    return m_Bits->operator[](id);
}

size_t Micro::size() const
{
    return m_Bits->size();
}

void Micro::resize(size_t sz, bool val)
{
    m_Bits->resize(sz, val);
}

prob Micro::GetProb() const
{
    return m_PriorProb;
}

void Micro::SetProb(prob value)
{
    m_PriorProb = value;
}

MicroSet::MicroSet() : m_TotalProb(0) { }

MicroSet::MicroSet(const MicroSet &other) : m_Set(other.m_Set), m_TotalProb(other.m_TotalProb) { }

MicroSet::MicroSet(const MicroSet &other, filter_t filter)
{
    m_Set.reserve(other.m_Set.size());
    std::copy_if(other.m_Set.begin(), other.m_Set.end(), std::back_inserter(m_Set), filter);

    m_TotalProb = 0;
    for (auto &micro : m_Set)
        m_TotalProb += micro.GetProb();
}

MicroSet::~MicroSet() { }

size_t MicroSet::size() const
{
    return m_Set.size();
}

std::vector<Micro>::const_iterator MicroSet::begin() const
{
    return m_Set.begin();
}

std::vector<Micro>::const_iterator MicroSet::end() const
{
    return m_Set.end();
}

void MicroSet::Emplace(const Micro &m)
{
    m_Set.emplace_back(m);
    m_TotalProb += m.GetProb();
}

void MicroSet::Emplace(Micro &&m)
{
    m_Set.emplace_back(std::move(m));
    m_TotalProb += m.GetProb();
}

prob MicroSet::GetTotalProb() const
{
    return m_TotalProb;
}
