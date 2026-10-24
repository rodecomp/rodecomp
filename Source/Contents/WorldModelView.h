#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDMODELVIEW_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDMODELVIEW_H

#include <list>

struct OrientedBounds;
struct WorldSpatialTree;
struct ModelRenderContext;
struct GroundLightingContext;
class GroundGeometry;

// Original CView plane coefficients and its separate normal cache are read
// independently. Do not assume the two arrays are identical or normalize them.
struct ModelViewPlanes {
  float planes[6][4];
  float normals[6][3];

  int ClassifyAxisAligned(const WorldSpatialTree& bounds) const;
  int ClassifyOriented(const OrientedBounds& bounds) const;
};

// Appended leaf lists at original CView +0xF8/+0x100. Collection does not clear
// previous results; the completed draw phase owns that release point.
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

// Model/ground portion of CView Draw. Leaf lists have already
// been classified by the view's spatial traversal; other world objects and
// overlay passes remain separate from this concrete slice.
void DrawWorldModelLeaves(
  std::list<WorldSpatialTree*>& clipped,
  std::list<WorldSpatialTree*>& inside,
  const ModelViewPlanes& planes,
  const float* matrix,
  unsigned frame,
  int updates,
  GroundGeometry& ground,
  ModelRenderContext& models,
  GroundLightingContext& lighting,
  int* visibleRectangle
);

#endif
