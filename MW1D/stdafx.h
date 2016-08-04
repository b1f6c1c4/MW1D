#pragma once
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <iomanip>

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

#ifdef USE_RATIONAL

#include "boost/multiprecision/cpp_int.hpp"

typedef boost::multiprecision::cpp_rational prob;
std::string to_string(prob value);
std::string to_alias_string(prob value);

#else

typedef double prob;
std::string to_string(prob value);

#endif
