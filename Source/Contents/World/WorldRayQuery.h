#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H

#include <cstddef>
#include <list>
#include "Utility/Math/Vector3d.h"

struct WorldSpatialTree;

struct WorldRayQuery {
  /* 0000 */ std::list<WorldSpatialTree*> nodes;
  /* 0008 */ vector3d origin;
  /* 0014 */ vector3d direction;
  /* 0020 */ float limit;

  WorldRayQuery();
  __declspec(noinline) int Intersects(const float* bounds) const;
  void Collect(const float* ray, WorldSpatialTree* root);
  __declspec(noinline) void CollectNode(WorldSpatialTree* node, int depth);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldRayQuery) == 36);
static_assert(offsetof(WorldRayQuery, origin) == 0x0008);
static_assert(offsetof(WorldRayQuery, direction) == 0x0014);
static_assert(offsetof(WorldRayQuery, limit) == 0x0020);
#endif

#endif
