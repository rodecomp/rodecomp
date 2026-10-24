#ifndef RAGNAROK_SOURCE_UI_COOLDOWNSTATE_H
#define RAGNAROK_SOURCE_UI_COOLDOWNSTATE_H

#include "Render/RenderPrimitive.h"

#include <map>

struct CooldownEdge {
  float origin;
  float current;
  int radius;
  unsigned char increasing;
};

// Original clock chooses the replay server tick only in restricted mode.
struct CooldownClock {
  const bool& restricted;
  const std::uint32_t& serverTick;
  std::uint32_t Now() const;
};

// Original824-byte radial cooldown value, copied into the shortcut lists.
// Geometry/renderer submission is owned by the separate drawing operation.
struct CooldownState {
  /* 0000 */ std::map<unsigned, CooldownEdge> edges;
  /* 0008 */ unsigned segment;
  /* 000C */ unsigned vertexCount;
  /* 0010 */ float angle;
  /* 0014 */ std::uint32_t duration;
  /* 0018 */ std::uint32_t start;
  /* 001C */ std::uint32_t elapsed;
  /* 0020 */ unsigned finalMinute;
  /* 0024 */ int reverse;
  /* 0028 */ RenderVertex vertices[24];
  /* 0328 */ int x;
  /* 032C */ int y;
  /* 0330 */ int slot;
  /* 0334 */ std::uint32_t color;

  CooldownState(
    int slot,
    int x,
    int y,
    std::uint32_t duration,
    std::uint32_t start,
    std::uint32_t elapsed,
    int reverse
  );
  void ClearEdges();
  void AddEdge(unsigned segment, float origin, int radius, unsigned char increasing);
  float Update(const CooldownClock& clock);
};

static_assert(sizeof(CooldownEdge) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CooldownState) == 824);
#endif

#endif
