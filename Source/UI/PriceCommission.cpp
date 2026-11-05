#include <cstdint>

// OFFSET: 0x004D2D50
// FUNCTION: fn_00161
int PriceCommission(std::int64_t price) {
  return price < 10000001 ? 0 : 5;
}
