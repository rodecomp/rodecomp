#include "Utility/Math/Int64Math.h"

#include <algorithm>

// OFFSET: 0x008C6AE0
// FUNCTION: fn_11919
std::int64_t MaxInt64(const std::int64_t& first, const int& second) {
  return std::max(first, static_cast<std::int64_t>(second));
}

// OFFSET: 0x0086BF00
// FUNCTION: fn_11017
std::int64_t sub_86BF00(const int& value, const std::int64_t& other) {
  return std::max(static_cast<std::int64_t>(value), other);
}
