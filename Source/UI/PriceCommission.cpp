#include <cstdint>

// FUNCTION: fn_00161
int PriceCommission(std::int64_t price) {
  return price > 10000000 ? 5 : 0;
}
