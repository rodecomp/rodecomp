#ifndef RAGNAROK_SOURCE_CONTENTS_MOVEMENTROUTEINTERPOLATION_H
#define RAGNAROK_SOURCE_CONTENTS_MOVEMENTROUTEINTERPOLATION_H

#include <cstdint>
#include <vector>

struct MovementRoutePoint {
  /* 0000 */ std::int32_t x;
  /* 0004 */ std::int32_t y;
  /* 0008 */ int metadata;
  /* 000C */ std::uint32_t tick;
};

struct MovementRouteInterpolation {
  /* 0000 */ std::vector<MovementRoutePoint> points;
  /* 000C */ std::uint32_t segment;
  /* 0010 */ float startX;
  /* 0014 */ float startY;

  int Sample(std::uint32_t tick, float* x, float* y, int* metadata);
  std::uint32_t Segment() const;
};

static_assert(sizeof(MovementRoutePoint) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MovementRouteInterpolation) == 24);
#endif

#endif
