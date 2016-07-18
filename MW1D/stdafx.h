#pragma once

#ifdef _DEBUG
#define ASSERT(val) do { if (!(val)) throw; } while (false)
#else
#define ASSERT(val)
#endif

#include <vector>

typedef std::vector<bool> bits;
typedef std::vector<bool>::reference bit_reference;
typedef std::vector<bool>::const_reference bit_const_reference;
