#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELVIEW_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELVIEW_H

#include <cstddef>
#include <list>

struct OrientedBounds;
struct WorldSpatialTree;
struct ModelRenderContext;
struct GroundLightingContext;
class GroundGeometry;

struct ModelViewPlanes {
  /* 0010 */ float planes[6][4];
  /* 0070 */ float normals[6][3];

  int ClassifyAxisAligned(const WorldSpatialTree& bounds) const;
  int ClassifyOriented(const OrientedBounds& bounds) const;
};

struct WorldViewSpatial {
  WorldViewSpatial();
  ~WorldViewSpatial();

  /* 0000 */ std::list<WorldSpatialTree*> inside;
  /* 0008 */ std::list<WorldSpatialTree*> clipped;
  /* 0010 */ ModelViewPlanes planes;

  void Collect(float horizontal, float vertical, const float* matrix, WorldSpatialTree& root);
  void CollectNode(WorldSpatialTree& node, int depth, bool fullyInside);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModelViewPlanes) == 0x00A8);
static_assert(offsetof(ModelViewPlanes, normals) == 0x0060);
static_assert(offsetof(WorldViewSpatial, clipped) == 0x0008);
static_assert(offsetof(WorldViewSpatial, planes) == 0x0010);
#endif

int ClassifyModelView(const OrientedBounds& bounds, const ModelViewPlanes& planes);

void DrawWorldModelLeaves(
  std::list<WorldSpatialTree*>& clipped, std::list<WorldSpatialTree*>& inside, const ModelViewPlanes& planes, const float* matrix, unsigned frame, int updates,
  GroundGeometry& ground, ModelRenderContext& models, GroundLightingContext& lighting, int* visibleRectangle
);

#endif
