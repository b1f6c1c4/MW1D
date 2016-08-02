#pragma once
#include <vector>
#include "boost/rational.hpp"

#ifdef _DEBUG
#define ASSERT(val) do { if (!(val)) throw; } while (false)
#else
#define ASSERT(val)
#endif

#ifdef MW1D_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#define UNCERTAIN static_cast<size_t>(-1)

typedef std::vector<bool> bits;
typedef std::vector<bool>::reference bit_reference;
typedef std::vector<bool>::const_reference bit_const_reference;

typedef boost::rational<__int64> prob;

inline std::string to_string(prob value)
{
    return std::to_string(value.numerator()) + "/" + std::to_string(value.denominator());
}

inline double to_double(prob value)
{
    return boost::rational_cast<double>(value);
}
