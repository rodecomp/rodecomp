#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALTREE_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDSPATIALTREE_H

#include "Contents/Ground/GroundGeometry.h"
#include "Resource/World/TerrainResource.h"

#include <cstdint>
#include <vector>

class CFile;
class C3dAttr;
class GroundGeometry;
class C3dActor;

struct WorldSpatialTree {
  __declspec(noinline) WorldSpatialTree();
  __declspec(noinline) ~WorldSpatialTree();
  WorldSpatialTree(const WorldSpatialTree&) = delete;
  WorldSpatialTree& operator=(const WorldSpatialTree&) = delete;
  void Subdivide(int depth);
  void ReadBounds(CFile& input, int depth);
  void BindTerrain(C3dAttr& resource, int depth);
  void BindGround(GroundGeometry& resource, int depth);
  void CopyBounds(int depth, const WorldSpatialTree& source);
  void RefreshBounds(int depth);
  void RefreshModelBounds(int depth);
  void InsertModel(C3dActor* model, int depth);
  void InsertModelAfter(C3dActor* model, int depth);
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
  /* 0054 */ GroundGeometry* ground;
  /* 0058 */ int groundCells[4];
  /* 0068 */ C3dAttr* terrain;
  /* 006C */ int terrainCells[4];
};

// OFFSET: 0x00AC8590
// FUNCTION: fn_17674
inline void WorldSpatialTree::BindTerrain(C3dAttr& resource, int depth) {
  const float minimumZ = this->minimum[2];
  this->dirty = 1;
  this->terrain = &resource;
  resource.SpatialCell(this->minimum[0], minimumZ, this->terrainCells[0], this->terrainCells[1]);
  this->terrain->SpatialCell(this->maximum[0], this->maximum[2], this->terrainCells[2], this->terrainCells[3]);

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->BindTerrain(resource, depth + 1);
    }
  }
}

// OFFSET: 0x00AC8620
// FUNCTION: fn_17675
inline void WorldSpatialTree::BindGround(GroundGeometry& resource, int depth) {
  const float minimumZ = this->minimum[2];
  this->dirty = 1;
  this->ground = &resource;
  resource.SpatialCell(this->minimum[0], minimumZ, this->groundCells[0], this->groundCells[1]);
  this->ground->SpatialCell(this->maximum[0], this->maximum[2], this->groundCells[2], this->groundCells[3]);

  if (depth != 5) {
    for (unsigned index = 0; index < 4; ++index) {
      this->children[index]->BindGround(resource, depth + 1);
    }
  }
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldSpatialTree) == 124);
#endif

#endif
