#include "UI/PriceColor.h"

// OFFSET: 0x00A7D8F0
// FUNCTION: fn_16957
PriceColors PriceColor(std::int64_t amount) {
  if (amount < 0) {
    return {0xFFFF00, 0};
  }

  if (amount >= 1000000000) {
    return {0x7F00FF, 0xFF};
  }

  if (amount >= 100000000) {
    return {0x64C8C8, 0};
  }

  if (amount >= 10000000) {
    return {0xFF, 0xFFFFFFFF};
  }

  if (amount >= 1000000) {
    return {0xFF00, 0};
  }

  if (amount >= 100000) {
    return {0xFF0000, 0xFFFFFFFF};
  }

  if (amount >= 10000) {
    return {0xFF1EFF, 0xFFFFFFFF};
  }

  if (amount >= 1000) {
    return {0xFFFF, 0xFF};
  }

  if (amount >= 100) {
    return {0xFFFF00, 0xFF0000};
  }

  if (amount >= 10) {
    return {0xC800C8, 0xFF0000};
  }

  return {0xFFFF00, 0};
}
