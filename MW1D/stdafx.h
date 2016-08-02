#pragma once
#include <vector>
#include "boost/multiprecision/cpp_int.hpp"

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

typedef boost::multiprecision::cpp_rational prob;

inline std::string to_string(prob value)
{
    return value.str();
}

inline double to_double(prob value)
{
    return value.convert_to<double>();
}
