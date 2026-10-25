#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODECURSOR_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODECURSOR_H

#include <cstdint>

struct ModeCursor {
  std::int32_t type;
  std::uint32_t frame;

  ModeCursor();
  void Set(std::int32_t value, std::uint32_t& changeTick);
};

static_assert(sizeof(ModeCursor) == 8);

#endif
