#ifndef RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNSTATE_H
#define RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNSTATE_H

#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <map>

struct CooldownEdge {
  /* 0000 */ float origin;
  /* 0004 */ float current;
  /* 0008 */ int radius;
  /* 000C */ unsigned char increasing;
};

struct CooldownClock {
  const bool& restricted;
  const std::uint32_t& serverTick;
  std::uint32_t Now() const;
};

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

  CooldownState(int slot, int x, int y, std::uint32_t duration, std::uint32_t start, std::uint32_t elapsed, int reverse);
  CooldownState(const CooldownState& other);
  ~CooldownState();
  void ClearEdges();
  void AddEdge(unsigned segment, float origin, int radius, unsigned char increasing);
  float Update(const CooldownClock& clock);
};

static_assert(sizeof(CooldownEdge) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CooldownEdge, current) == 0x04);
static_assert(offsetof(CooldownEdge, radius) == 0x08);
static_assert(offsetof(CooldownEdge, increasing) == 0x0C);
static_assert(sizeof(CooldownState) == 824);
#endif

#endif
