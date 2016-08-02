#pragma once
#include "stdafx.h"

#ifdef MW1D_DLL
extern "C"
{
    DLL_API void CoreInterfaceT(__int32 n, __int32 m, const char *solver, __int64 *num, __int64 *den);

    DLL_API void CoreInterfaceP(__int32 n, __int32 pN, __int32 pD, const char *solver, __int64 *num, __int64 *den);
}
#endif