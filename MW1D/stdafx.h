#pragma once

#ifdef _DEBUG
#define ASSERT(val) do { if (!(val)) throw; } while (false)
#else
#define ASSERT(val)
#endif

#include <vector>
#include "boost/rational.hpp"

typedef std::vector<bool> bits;
typedef std::vector<bool>::reference bit_reference;
typedef std::vector<bool>::const_reference bit_const_reference;

typedef boost::rational<__int64> prob;

inline std::string to_string(prob value)
{
    return std::to_string(value.numerator()) + "/" + std::to_string(value.denominator());
}
