#ifndef RAGNAROK_SOURCE_CONTENTS_MOVEMENTROUTEINTERPOLATION_H
#define RAGNAROK_SOURCE_CONTENTS_MOVEMENTROUTEINTERPOLATION_H

#include <cstddef>
#include <cstdint>
#include <vector>

struct MovementPathNode {
  /* 0000 */ std::int32_t x;
  /* 0004 */ std::int32_t y;
  /* 0008 */ MovementPathNode* parent;
  /* 000C */ int cost;
  /* 0010 */ int estimatedCost;
  /* 0014 */ int state;
  /* 0018 */ int metadata;
};

struct MovementRoutePoint {
  /* 0000 */ std::int32_t x;
  /* 0004 */ std::int32_t y;
  /* 0008 */ int metadata;
  /* 000C */ std::uint32_t tick;
};

struct MovementRouteInterpolation {
  MovementRouteInterpolation();

  /* 0000 */ std::vector<MovementRoutePoint> points;
  /* 000C */ std::uint32_t segment;
  /* 0010 */ float startX;
  /* 0014 */ float startY;

  bool Point(int index, std::int32_t* x, std::int32_t* y) const;
  std::uint32_t* EndTick(std::uint32_t* tick) const;
  int EndMetadata() const;
  int NextPoint(std::uint32_t tick, std::uint32_t* nextTick, float* x, float* y) const;
  int Sample(std::uint32_t tick, float* x, float* y, int* metadata);
  std::uint32_t Segment() const;
};

void BuildMovementRoute(std::uint32_t tick, const MovementPathNode* last, int stepDuration, MovementRouteInterpolation& route);

static_assert(sizeof(MovementRoutePoint) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MovementPathNode) == 28);
static_assert(offsetof(MovementPathNode, metadata) == 0x18);
static_assert(sizeof(MovementRouteInterpolation) == 24);
static_assert(offsetof(MovementRouteInterpolation, segment) == 0xC);
static_assert(offsetof(MovementRouteInterpolation, startX) == 0x10);
static_assert(offsetof(MovementRouteInterpolation, startY) == 0x14);
#endif

#endif
