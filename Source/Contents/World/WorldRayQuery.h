#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H

#include <cstddef>
#include <list>

struct WorldSpatialTree;

struct WorldRayQuery {
  /* 0000 */ std::list<WorldSpatialTree*> nodes;
  /* 0008 */ float origin[3];
  /* 0014 */ float direction[3];
  /* 0020 */ float limit;

  bool Intersects(const float* bounds) const;
  void Collect(const float* ray, WorldSpatialTree* root);
  void CollectNode(WorldSpatialTree* node, int depth);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldRayQuery) == 36);
static_assert(offsetof(WorldRayQuery, origin) == 0x0008);
static_assert(offsetof(WorldRayQuery, direction) == 0x0014);
static_assert(offsetof(WorldRayQuery, limit) == 0x0020);
#endif

#endif
