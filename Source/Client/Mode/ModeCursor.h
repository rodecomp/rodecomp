#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODECURSOR_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODECURSOR_H

#include <cstddef>
#include <cstdint>

struct ModeCursor {
  /* 0050 */ std::int32_t type;
  /* 0054 */ std::uint32_t frame;

  ModeCursor();
  void Set(std::int32_t value, std::uint32_t& changeTick);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModeCursor) == 8);
static_assert(offsetof(ModeCursor, type) + 0x50 == 0x50);
static_assert(offsetof(ModeCursor, frame) + 0x50 == 0x54);
#endif

#endif
