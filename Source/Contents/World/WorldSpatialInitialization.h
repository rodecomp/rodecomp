#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALINITIALIZATION_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALINITIALIZATION_H

#include <span>

struct WorldSpatialTree;
class GroundGeometry;
class C3dAttr;
class C3dActor;

void PublishWorldModels(WorldSpatialTree& root, C3dActor* const* models, int& published, int& pending);

#endif
