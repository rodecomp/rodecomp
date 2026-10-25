#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELLOADING_H

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
  float distance;
  const WorldModelRecord* placement;
};

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

private:
  static DWORD WINAPI ThreadEntry(void* context);
};

#endif
