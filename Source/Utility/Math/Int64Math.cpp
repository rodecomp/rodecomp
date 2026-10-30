#include "Utility/Math/Int64Math.h"

// OFFSET: 0x008C6AE0
// FUNCTION: fn_11919
std::int64_t MaxInt64(const std::int64_t& first, const int& second) {
  const std::int64_t value = second;
  if (first < value) {
    return value;
  }
  return first;
}
