#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDMODELLOADING_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDMODELLOADING_H

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

// Borrowed CWorld model vector/published/pending fields and the original shared
// deferred list/thread handle. Import reserves space for all placements before
// launching; placements, dependencies and vector storage must survive Finish.
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
