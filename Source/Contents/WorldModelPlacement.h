#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDMODELPLACEMENT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDMODELPLACEMENT_H

class C3dActor;
class C3dModelRes;
class BinkMovies;
struct WorldModelRecord;
struct ModelLightingContext;

// Model creation block shared by immediate and worker world import. Returned
// actor ownership transfers to the caller; resource/mesh lifetime remains with
// the resource manager. Distance scheduling and world startup are separate.
C3dActor* CreatePlacedWorldModel(
  const WorldModelRecord& placement,
  C3dModelRes& resource,
  BinkMovies& movies,
  ModelLightingContext& lighting
);

#endif
