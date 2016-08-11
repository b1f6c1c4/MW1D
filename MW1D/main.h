#pragma once
#include "stdafx.h"

#ifdef MW1D_DLL
extern "C"
{
    DLL_API void CoreInterfaceT(__int32 n, __int32 m, bool nR, const char *solver, const char *filePath);

    DLL_API void CoreInterfaceP(__int32 n, __int32 pN, __int32 pD, bool nR, const char *solver, const char *filePath);
}
#endif
