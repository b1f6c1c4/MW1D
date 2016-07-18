#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#ifdef MW1D_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#define ASSERT(val) do { if (!(val)) throw; } while (false)
#else
#define ASSERT(val)
#endif
