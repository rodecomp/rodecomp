#ifndef RAGNAROK_SOURCE_UTILITY_MATH_SCALARCONVERSION_H
#define RAGNAROK_SOURCE_UTILITY_MATH_SCALARCONVERSION_H

#include <limits>

// INLINED
inline int TruncateToInt(double value) {
  return value > -2147483649.0 && value < 2147483648.0 ? static_cast<int>(value) : std::numeric_limits<int>::min();
}

#endif
