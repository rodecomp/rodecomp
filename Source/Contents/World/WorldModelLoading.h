#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H

#include <cstddef>
#include <vector>
#include <windows.h>

class C3dActor;
class ResourceMgr;
class BinkMovies;
struct WorldModelRecord;
struct ModelLightingContext;
struct WorldSpatialTree;
struct TerrainHeight;

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
extern int g_partyEffectNoticePending;

struct WorldModelLoading {
  ResourceMgr& resources;
  BinkMovies& movies;
  ModelLightingContext& lighting;
  std::vector<C3dActor*>& models;
  int& published;
  int& pending;
  std::vector<DeferredWorldModel>& deferred;
  HANDLE& thread;

  void Schedule(const WorldModelRecord& placement, const TerrainHeight& terrain, int column, int row, float* center);
  void Start();
  void Run();
  void Publish(WorldSpatialTree& root);
  void Finish();

  static DWORD WINAPI ThreadEntry(void* context);
};

#endif
