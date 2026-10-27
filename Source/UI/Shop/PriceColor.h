#ifndef RAGNAROK_SOURCE_UI_SHOP_PRICECOLOR_H
#define RAGNAROK_SOURCE_UI_SHOP_PRICECOLOR_H

#include <cstddef>
#include <cstdint>

struct PriceColors {
  /* 0000 */ std::uint32_t primary;
  /* 0004 */ std::uint32_t secondary;
};

PriceColors PriceColor(std::int64_t amount);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PriceColors) == 0x08);
static_assert(offsetof(PriceColors, secondary) == 0x04);
#endif

#endif
