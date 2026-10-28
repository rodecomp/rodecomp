#ifndef RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H
#define RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H

#include "Contents/MovementRouteInterpolation.h"
#include "Resource/World/TerrainResource.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct MovementPathNodeCompare {
  bool operator()(const MovementPathNode* left, const MovementPathNode* right) const {
    return left->estimatedCost > right->estimatedCost;
  }
};

class MovementOpenList {
public:
  std::vector<MovementPathNode*>& Nodes() {
    return this->nodes;
  }

  /* 0000 */ std::vector<MovementPathNode*> nodes;
};

class CPathFinder {
public:
  CPathFinder();
  ~CPathFinder();

  /* 0000 */ C3dAttr* attr;
  /* 0004 */ int destinationX;
  /* 0008 */ int destinationY;
  /* 000C */ int nodeCount;
  /* 0010 */ MovementPathNode nodes[150];
  /* 1078 */ std::map<std::string, C3dAttr*> attrCache;
  /* 1080 */ bool useNodePool;
  /* 1084 */ std::vector<MovementPathNode> nodePool;
  /* 1090 */ std::map<int, MovementPathNode*> nodeMap;
  /* 1098 */ MovementOpenList openList;

  void Clear();
  void Reset();
  bool CanMove(int startX, int startY, int targetX, int targetY, char flag) const;
  MovementPathNode* FindOrCreateNode(int x, int y);
  bool AddNeighbor(MovementPathNode* current, int cost, int nextX, int nextY, int metadata);
  bool FindPath(
    int startTick, int startX, int startY, int currentFractionX, int currentFractionY, int destX, int destY, int speed, MovementRouteInterpolation& route,
    char flag
  );
  int Height() const;
  int Width() const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPathFinder, attrCache) == 0x1078);
static_assert(offsetof(CPathFinder, nodePool) == 0x1084);
static_assert(offsetof(CPathFinder, nodeMap) == 0x1090);
static_assert(offsetof(CPathFinder, openList) == 0x1098);
static_assert(sizeof(MovementOpenList) == 0x0C);
static_assert(sizeof(CPathFinder) == 0x10A4);
#endif

#endif
