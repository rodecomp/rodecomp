#ifndef RAGNAROK_SOURCE_CLIENT_MODECURSOR_H
#define RAGNAROK_SOURCE_CLIENT_MODECURSOR_H

#include <cstdint>

// CMode fields+0x50..+0x57; change tick remains at its actual owner field+0x24.
struct ModeCursor {
  std::int32_t type;
  std::uint32_t frame;

  ModeCursor();
  void Set(std::int32_t value, std::uint32_t& changeTick);
};

static_assert(sizeof(ModeCursor) == 8);

#endif
