#include "stdafx.h"

#ifdef USE_RATIONAL

std::string to_string(prob value)
{
    return value.str();
}

std::string to_alias_string(prob value)
{
    std::ostringstream out;
    out.precision(std::numeric_limits<double>::max_digits10);
    out << value.convert_to<double>();
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
