#include "stdafx.h"

#ifdef USE_CAS

std::string to_string(prob value)
{
    return to_alias_string(value);
}

std::string to_alias_string(prob value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

#else

std::string to_string(prob value)
{
    std::ostringstream out;
    out.precision(std::numeric_limits<double>::max_digits10);
    out << std::fixed << value;
    return out.str();
}

#endif
