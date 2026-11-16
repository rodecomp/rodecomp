#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H

#include <cstddef>
#include <vector>
#include <windows.h>
#include "Utility/Math/Vector3d.h"

struct WorldModelRecord;

struct DeferredWorldModel {
  /* 0000 */ float distance;
  /* 0004 */ const WorldModelRecord* placement;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DeferredWorldModel) == 8);
static_assert(offsetof(DeferredWorldModel, placement) == 4);
#endif

extern HANDLE hObject;
extern std::vector<DeferredWorldModel> g_deferredWorldModels;
extern vector3d g_worldLightDirection;
extern vector3d g_worldDiffuse;
extern vector3d g_worldAmbient;
extern float g_worldLoadCenter[3];

#endif
