#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDRAYQUERY_H

#include <list>

struct WorldSpatialTree;

struct WorldRayQuery {
  std::list<WorldSpatialTree*> nodes;
  float origin[3];
  float direction[3];
  float limit;

  bool Intersects(const float* bounds) const;
  void Collect(const float* ray, WorldSpatialTree* root);
  void CollectNode(WorldSpatialTree* node, int depth);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldRayQuery) == 36);
#endif

#endif
