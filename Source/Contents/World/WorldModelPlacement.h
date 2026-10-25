#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELPLACEMENT_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDMODELPLACEMENT_H

class C3dActor;
class C3dModelRes;
class BinkMovies;
struct WorldModelRecord;
struct ModelLightingContext;

C3dActor* CreatePlacedWorldModel(const WorldModelRecord& placement, C3dModelRes& resource, BinkMovies& movies, ModelLightingContext& lighting);

#endif
