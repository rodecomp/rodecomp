#ifndef RAGNAROK_SOURCE_UI_SHOP_PRICECOLOR_H
#define RAGNAROK_SOURCE_UI_SHOP_PRICECOLOR_H

#include <cstdint>

struct PriceColors {
  std::uint32_t primary;
  std::uint32_t secondary;
};

PriceColors PriceColor(std::int64_t amount);

#endif
