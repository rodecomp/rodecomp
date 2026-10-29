#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSCENE_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSCENE_H

#include "Contents/World/WorldSpatialTree.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <vector>

class BinkMovies;
class CGameMode;
class CPlayer;
class CRenderObject;
class GroundGeometry;

class WorldScene {
public:
  WorldScene();
  virtual ~WorldScene();
  WorldScene(const WorldScene&) = delete;
  WorldScene& operator=(const WorldScene&) = delete;
  void Clear();
  bool FindActorInCell(const float* position, std::function<bool(std::uint32_t)> predicate) const;
  CRenderObject* GetGameActorByAID(std::uint32_t aid) const;
  CRenderObject* GetGameActorByAidIncludeMe(std::uint32_t aid) const;
  __declspec(noinline) CRenderObject* GetSkillByAID(std::uint32_t aid) const;
  void ClearActorCells();

  /* 0004 */ CGameMode* gameMode;
  /* 0008 */ std::list<CRenderObject*> objects;
  /* 0010 */ std::list<CRenderObject*> actors;
  /* 0018 */ std::list<CRenderObject*> unknown_0018;
  /* 0020 */ std::list<CRenderObject*> unknown_0020;
  /* 0028 */ GroundGeometry* ground;
  /* 002C */ CPlayer* player;
  /* 0030 */ C3dAttr* terrain;
  /* 0034 */ std::vector<C3dActor*> models;
  /* 0040 */ int published;
  /* 0044 */ int pending;
  /* 0048 */ int unknown_0048;
  /* 004C */ int unknown_004C;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;
  /* 0058 */ WorldSpatialTree spatial;
  /* 00D4 */ const WorldSpatialTree* serializedSpatial;
  /* 00D8 */ std::list<std::uint32_t> pendingActorIds;
  /* 00E0 */ BinkMovies* movies;
  /* 00E4 */ std::map<float, std::list<std::uint32_t>> actorCells;
  /* 00EC */ std::list<std::uint32_t> unindexedActorIds;
  /* 00F4 */ bool unknown_00F4;
  /* 00F8 */ int nearbyRange;
  /* 00FC */ bool unknown_00FC;
  /* 00FD */ bool unknown_00FD;
};

CRenderObject* FindRenderObject(std::uint32_t aid);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(WorldScene, gameMode) == 0x04);
static_assert(offsetof(WorldScene, objects) == 0x08);
static_assert(offsetof(WorldScene, actors) == 0x10);
static_assert(offsetof(WorldScene, unknown_0018) == 0x18);
static_assert(offsetof(WorldScene, unknown_0020) == 0x20);
static_assert(offsetof(WorldScene, ground) == 0x28);
static_assert(offsetof(WorldScene, player) == 0x2C);
static_assert(offsetof(WorldScene, terrain) == 0x30);
static_assert(offsetof(WorldScene, models) == 0x34);
static_assert(offsetof(WorldScene, published) == 0x40);
static_assert(offsetof(WorldScene, pending) == 0x44);
static_assert(offsetof(WorldScene, spatial) == 0x58);
static_assert(offsetof(WorldScene, serializedSpatial) == 0xD4);
static_assert(offsetof(WorldScene, pendingActorIds) == 0xD8);
static_assert(offsetof(WorldScene, movies) == 0xE0);
static_assert(offsetof(WorldScene, actorCells) == 0xE4);
static_assert(offsetof(WorldScene, unindexedActorIds) == 0xEC);
static_assert(offsetof(WorldScene, unknown_00F4) == 0xF4);
static_assert(offsetof(WorldScene, nearbyRange) == 0xF8);
static_assert(offsetof(WorldScene, unknown_00FC) == 0xFC);
static_assert(offsetof(WorldScene, unknown_00FD) == 0xFD);
static_assert(sizeof(WorldScene) == 0x100);
#endif

#endif
