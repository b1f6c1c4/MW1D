#include "Micro.h"
#include <algorithm>
#include <iterator>

Micro::Micro() : m_Bits(std::make_shared<bits>()) { }

Micro::Micro(const Micro &other) : m_Bits(other.m_Bits) { }

Micro::Micro(Micro &&other) : m_Bits(nullptr)
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

MicroSet::MicroSet() { }

MicroSet::MicroSet(const MicroSet &other) : m_Set(other.m_Set) { }

MicroSet::MicroSet(const MicroSet &other, filter_t filter)
{
    m_Set.reserve(other.m_Set.size());
    std::copy_if(other.m_Set.begin(), other.m_Set.end(), std::back_inserter(m_Set), filter);
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
}

void MicroSet::Emplace(Micro &&m)
{
    m_Set.emplace_back(std::move(m));
}
