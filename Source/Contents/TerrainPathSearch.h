#ifndef RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H
#define RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H

#include "Contents/MovementRouteInterpolation.h"
#include "Resource/World/TerrainResource.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct NodeTotalGreater {
  bool operator()(const PathNode* left, const PathNode* right) const {
    return left->estimatedCost > right->estimatedCost;
  }
};

class CPriorityQueue {
public:
  std::vector<PathNode*>& Nodes() {
    return this->nodes;
  }
  void Push(PathNode* node);

  /* 0000 */ std::vector<PathNode*> nodes;
};

class CPathFinder {
public:
  CPathFinder();
  ~CPathFinder();

  /* 0000 */ C3dAttr* attr;
  /* 0004 */ int destinationX;
  /* 0008 */ int destinationY;
  /* 000C */ int nodeCount;
  /* 0010 */ PathNode nodes[150];
  /* 1078 */ std::map<std::string, C3dAttr*> attrCache;
  /* 1080 */ bool useNodePool;
  /* 1084 */ std::vector<PathNode> nodePool;
  /* 1090 */ std::map<int, PathNode*> nodeMap;
  /* 1098 */ CPriorityQueue openList;

  void Clear();
  void Reset();
  bool CanMove(int startX, int startY, int targetX, int targetY, char flag) const;
  PathNode* FindOrCreateNode(int x, int y);
  bool AddNeighbor(PathNode* current, int cost, int nextX, int nextY, int metadata);
  bool FindPath(
    int startTick, int startX, int startY, int currentFractionX, int currentFractionY, int destX, int destY, int speed, MovementRouteInterpolation& route,
    char flag
  );
  int Height() const;
  int Width() const;
};

extern CPathFinder g_pathFinder;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPathFinder, attrCache) == 0x1078);
static_assert(offsetof(CPathFinder, nodePool) == 0x1084);
static_assert(offsetof(CPathFinder, nodeMap) == 0x1090);
static_assert(offsetof(CPathFinder, openList) == 0x1098);
static_assert(sizeof(CPriorityQueue) == 0x0C);
static_assert(sizeof(CPathFinder) == 0x10A4);
#endif

#endif
