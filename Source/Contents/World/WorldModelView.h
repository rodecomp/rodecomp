#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELVIEW_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELVIEW_H

#include <list>

struct OrientedBounds;
struct WorldSpatialTree;
struct ModelRenderContext;
struct GroundLightingContext;
class GroundGeometry;

struct ModelViewPlanes {
  float planes[6][4];
  float normals[6][3];

  int ClassifyAxisAligned(const WorldSpatialTree& bounds) const;
  int ClassifyOriented(const OrientedBounds& bounds) const;
};

struct WorldViewSpatial {
  WorldViewSpatial();
  ~WorldViewSpatial();

  std::list<WorldSpatialTree*> inside;
  std::list<WorldSpatialTree*> clipped;
  ModelViewPlanes planes;

  void Collect(float horizontal, float vertical, const float* matrix, WorldSpatialTree& root);
  void CollectNode(WorldSpatialTree& node, int depth, bool fullyInside);
};

int ClassifyModelView(const OrientedBounds& bounds, const ModelViewPlanes& planes);

void DrawWorldModelLeaves(
  std::list<WorldSpatialTree*>& clipped, std::list<WorldSpatialTree*>& inside, const ModelViewPlanes& planes, const float* matrix, unsigned frame, int updates,
  GroundGeometry& ground, ModelRenderContext& models, GroundLightingContext& lighting, int* visibleRectangle
);

#endif
