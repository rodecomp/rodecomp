#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H

#include <cstddef>
#include <vector>
#include <windows.h>

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
extern float g_worldLightDirection[3];
extern float g_worldDiffuse[3];
extern float g_worldAmbient[3];
extern float g_worldLoadCenter[3];

#endif
