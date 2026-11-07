#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDTERRAINPICKING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDTERRAINPICKING_H

struct WorldSpatialTree;

bool PickTerrainRay(const float* ray, WorldSpatialTree& root, int& column, int& row);

#endif
