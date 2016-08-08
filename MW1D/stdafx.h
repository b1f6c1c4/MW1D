#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "../../CppUtil/CppUtil/CopyMove.hpp"

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

#ifdef USE_CAS

#include "Expression.h"
std::string to_string(prob value);
std::string to_alias_string(prob value);

#else

typedef double prob;
std::string to_string(prob value);

#endif
