#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDSPATIALINITIALIZATION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDSPATIALINITIALIZATION_H

#include <span>

struct WorldSpatialTree;
struct GroundSpatial;
class C3dAttr;
class C3dActor;

// CWorld spatial assembly. Ground references must outlive
// the tree; rebuilding deletes children but retains the root's object vector.
void InitializeWorldSpatial(
  WorldSpatialTree& root,
  GroundSpatial& ground,
  C3dAttr& terrain,
  std::span<C3dActor* const> models,
  const WorldSpatialTree* serialized
);

// Publication block in CWorld update. The worker publishes actor
// pointers before incrementing pending; this pass consumes one count snapshot.
// Model storage must remain stable while the worker and this pass access it.
void PublishWorldModels(WorldSpatialTree& root, C3dActor* const* models, int& published, int& pending);

#endif
