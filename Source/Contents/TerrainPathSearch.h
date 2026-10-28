#ifndef RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H
#define RAGNAROK_SOURCE_CONTENTS_TERRAINPATHSEARCH_H

#include "Contents/MovementRouteInterpolation.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <queue>
#include <vector>

class C3dAttr;

struct MovementPathNodeCompare {
  bool operator()(const MovementPathNode* left, const MovementPathNode* right) const {
    return left->estimatedCost > right->estimatedCost;
  }
};

class MovementOpenList : public std::priority_queue<MovementPathNode*, std::vector<MovementPathNode*>, MovementPathNodeCompare> {
public:
  std::vector<MovementPathNode*>& Nodes() {
    return this->c;
  }
};

class CPathFinder {
public:
  CPathFinder();

  /* 0000 */ C3dAttr* attr;
  /* 0004 */ int destinationX;
  /* 0008 */ int destinationY;
  /* 000C */ int nodeCount;
  /* 0010 */ MovementPathNode nodes[150];
  /* 1078 */ std::map<int, MovementRouteInterpolation> unknown_1078;
  /* 1080 */ bool useNodePool;
  /* 1084 */ std::vector<MovementPathNode> nodePool;
  /* 1090 */ std::map<int, MovementPathNode*> nodeMap;
  /* 1098 */ MovementOpenList openList;

  void Reset();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CPathFinder, unknown_1078) == 0x1078);
static_assert(offsetof(CPathFinder, nodePool) == 0x1084);
static_assert(offsetof(CPathFinder, nodeMap) == 0x1090);
static_assert(offsetof(CPathFinder, openList) == 0x1098);
#endif

#endif
