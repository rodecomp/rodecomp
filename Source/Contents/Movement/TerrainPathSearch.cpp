#include "Contents/Movement/TerrainPathSearch.h"
#include "Resource/World/TerrainResource.h"
#include <algorithm>
#include <cstdlib>

// INLINED
inline PathNode* CPriorityQueue::Pop() {
  PathNode* node = this->nodes.front();
  std::pop_heap(this->nodes.begin(), this->nodes.end(), NodeTotalGreater());
  this->nodes.pop_back();
  return node;
}

// OFFSET: 0x00AD7E70
// FUNCTION: fn_17860
CPathFinder::CPathFinder() {
  this->useNodePool = false;
}

// OFFSET: 0x00AD88F0
// FUNCTION: fn_17868
bool CPathFinder::FindPath(
  int startTick, int startX, int startY, int currentFractionX, int currentFractionY, int destX, int destY, int speed, MovementRouteInterpolation& route,
  char flag
) {
  this->destinationX = destX;
  this->destinationY = destY;

  if (startX == destX && startY == destY) {
    return false;
  }

  route.segment = 0;
  this->Reset();
  this->nodeCount = 0;

  PathNode* start = this->FindOrCreateNode(startX, startY);
  start->state = 1;
  start->parent = nullptr;
  start->cost = 0;
  start->estimatedCost = 10 * (std::abs(startX - this->destinationX) + std::abs(startY - this->destinationY));

  this->openList.Push(start);

  while (!this->openList.nodes.empty()) {
    PathNode* current = this->openList.Pop();

    if (current->x == destX && current->y == destY) {
      const auto startCoordinate = [](int cell, int fraction) {
        const float offset = static_cast<float>(fraction) * 0.0625f;
        return cell > 0 ? static_cast<float>(static_cast<double>(cell) - 0.5 + offset) : offset;
      };
      route.startX = startCoordinate(startX, currentFractionX);
      route.startY = startCoordinate(startY, currentFractionY);

      this->BuildMovementRoute(startTick, current, speed, route);
      return true;
    }

    const auto tryNeighbor = [this, current, flag](int dx, int dy, int direction) {
      const int nextY = current->y + dy;
      const int nextX = current->x + dx;
      const int cost = dx != 0 && dy != 0 ? 14 : 10;
      return !this->CanMove(current->x, current->y, nextX, nextY, flag) || this->AddNeighbor(current, cost, nextX, nextY, direction);
    };

    if (!tryNeighbor(1, -1, 5) || !tryNeighbor(1, 0, 6) || !tryNeighbor(1, 1, 7) || !tryNeighbor(0, 1, 0) ||
        !tryNeighbor(-1, 1, 1) || !tryNeighbor(-1, 0, 2) || !tryNeighbor(-1, -1, 3) || !tryNeighbor(0, -1, 4)) {
      return false;
    }

    current->state = 2;
  }

  return false;
}

// OFFSET: 0x00AD9030
// FUNCTION: fn_17875
PathNode* CPathFinder::FindOrCreateNode(int x, int y) {
  const int width = this->attr->width;
  const int key = y * width + x;
  auto found = this->nodeMap.find(key);
  if (found != this->nodeMap.end() && found->second) {
    return found->second;
  }

  PathNode* node;
  if (this->useNodePool) {
    const int count = static_cast<int>(this->nodePool.size());
    if (this->nodeCount >= count - 1) {
      this->nodePool.resize(this->attr->height * width + 1);
    }
    node = &this->nodePool[this->nodeCount++];
  } else {
    if (this->nodeCount >= 149) {
      return nullptr;
    }
    node = &this->nodes[this->nodeCount++];
  }

  if (!node) {
    return nullptr;
  }

  node->x = x;
  node->y = y;
  node->state = 0;
  this->nodeMap[y * this->attr->width + x] = node;
  return node;
}

// INLINED
static void UpdateOpenNode(std::vector<PathNode*>& open, PathNode* node) {
  for (auto it = open.begin(); it != open.end(); ++it) {
    if ((*it)->x == node->x && (*it)->y == node->y) {
      std::push_heap(open.begin(), it + 1, NodeTotalGreater());
      return;
    }
  }
}

// OFFSET: 0x00AD9610
// FUNCTION: fn_17882
bool CPathFinder::AddNeighbor(PathNode* current, int cost, int nextX, int nextY, int metadata) {
  const int totalCost = current->cost + cost;
  PathNode* next = this->FindOrCreateNode(nextX, nextY);
  if (!next) {
    return false;
  }

  if (next->state != 0) {
    if (next->cost > totalCost) {
      next->parent = current;
      next->cost = totalCost;
      next->estimatedCost = totalCost + 10 * (std::abs(nextX - this->destinationX) + std::abs(nextY - this->destinationY));
      next->metadata = metadata;

      if (next->state == 1) {
        UpdateOpenNode(this->openList.Nodes(), next);
      } else if (next->state == 2) {
        next->state = 1;
      }
    }

    return true;
  }

  next->parent = current;
  next->state = 1;
  next->cost = totalCost;
  next->estimatedCost = totalCost + 10 * (std::abs(nextX - this->destinationX) + std::abs(nextY - this->destinationY));
  next->metadata = metadata;
  this->openList.Push(next);
  return true;
}

// OFFSET: 0x00AD9760
// FUNCTION: fn_17883
void CPriorityQueue::Push(PathNode* node) {
  this->nodes.push_back(node);
  std::push_heap(this->nodes.begin(), this->nodes.end(), NodeTotalGreater());
}

// OFFSET: 0x00AD97C0
// FUNCTION: fn_17884
void CPathFinder::Clear() {
  for (auto it = this->attrCache.begin(); it != this->attrCache.end(); ++it) {
    delete it->second;
  }

  this->attrCache.clear();
}

// OFFSET: 0x00AD9840
// FUNCTION: fn_17885
void CPathFinder::Reset() {
  this->nodePool.erase(this->nodePool.begin(), this->nodePool.end());
  this->nodeMap.erase(this->nodeMap.begin(), this->nodeMap.end());
  std::vector<PathNode*>& open = this->openList.Nodes();
  open.erase(open.begin(), open.end());
}

// OFFSET: 0x00AD9920
// FUNCTION: fn_17888
void CPathFinder::SetAttr(C3dAttr* attr) {
  this->attr = attr;
}
