#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALTREE_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALTREE_H

#include <cstdint>
#include <vector>

struct ResourceByteReader;
class C3dAttr;
struct GroundSpatial;
class C3dActor;

struct WorldSpatialTree {
  WorldSpatialTree();
  ~WorldSpatialTree();
  WorldSpatialTree(const WorldSpatialTree&) = delete;
  WorldSpatialTree& operator=(const WorldSpatialTree&) = delete;
  void Subdivide(int depth);
  void ReadBounds(ResourceByteReader& input, int depth);
  void ClearChildren();
  void BindTerrain(C3dAttr& resource, int depth);
  void BindGround(GroundSpatial& resource, int depth);
  void CopyBounds(int depth, const WorldSpatialTree& source);
  void RefreshBounds(int depth);
  void InsertModel(C3dActor& model, int depth);
  void RemoveModel(C3dActor* model, int depth);
  std::vector<C3dActor*>& ModelsAt(float x, float z, int depth);

  /* 0000 */ WorldSpatialTree* parent;
  /* 0004 */ WorldSpatialTree* children[4];
  /* 0014 */ float minimum[3];
  /* 0020 */ float maximum[3];
  /* 002C */ float center[3];
  /* 0038 */ float halfSize[3];
  /* 0044 */ int dirty;
  /* 0048 */ std::vector<C3dActor*> objects;
  /* 0054 */ GroundSpatial* ground;
  /* 0058 */ int groundCells[4];
  /* 0068 */ C3dAttr* terrain;
  /* 006C */ int terrainCells[4];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldSpatialTree) == 124);
#endif

#endif
