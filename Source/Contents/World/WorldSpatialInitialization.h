#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALINITIALIZATION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALINITIALIZATION_H

#include <span>

struct WorldSpatialTree;
class GroundGeometry;
class C3dAttr;
class C3dActor;

void InitializeWorldSpatial(
  WorldSpatialTree& root, GroundGeometry& ground, C3dAttr& terrain, std::span<C3dActor* const> models, const WorldSpatialTree* serialized
);

void PublishWorldModels(WorldSpatialTree& root, C3dActor* const* models, int& published, int& pending);

#endif
