#include "Contents/TerrainPathSearch.h"
#include "Resource/World/TerrainResource.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00AD7E70
// FUNCTION: fn_17860
CPathFinder::CPathFinder() : useNodePool(false) {
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

  std::vector<PathNode*>& open = this->openList.Nodes();
  open.push_back(start);
  std::push_heap(open.begin(), open.end(), NodeTotalGreater());

  while (!open.empty()) {
    PathNode* current = open.front();
    std::pop_heap(open.begin(), open.end(), NodeTotalGreater());
    open.pop_back();

    if (current->x == destX && current->y == destY) {
      float x = static_cast<float>(currentFractionX) * 0.0625f;
      if (startX > 0) {
        x = static_cast<float>(static_cast<double>(startX) - 0.5 + x);
      }
      route.startX = x;

      float y = static_cast<float>(currentFractionY) * 0.0625f;
      if (startY > 0) {
        y = static_cast<float>(static_cast<double>(startY) - 0.5 + y);
      }
      route.startY = y;

      BuildMovementRoute(startTick, current, speed, route);
      return true;
    }

    if (this->CanMove(current->x, current->y, current->x + 1, current->y - 1, flag) && !this->AddNeighbor(current, 14, current->x + 1, current->y - 1, 5)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x + 1, current->y, flag) && !this->AddNeighbor(current, 10, current->x + 1, current->y, 6)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x + 1, current->y + 1, flag) && !this->AddNeighbor(current, 14, current->x + 1, current->y + 1, 7)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x, current->y + 1, flag) && !this->AddNeighbor(current, 10, current->x, current->y + 1, 0)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x - 1, current->y + 1, flag) && !this->AddNeighbor(current, 14, current->x - 1, current->y + 1, 1)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x - 1, current->y, flag) && !this->AddNeighbor(current, 10, current->x - 1, current->y, 2)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x - 1, current->y - 1, flag) && !this->AddNeighbor(current, 14, current->x - 1, current->y - 1, 3)) {
      return false;
    }
    if (this->CanMove(current->x, current->y, current->x, current->y - 1, flag) && !this->AddNeighbor(current, 10, current->x, current->y - 1, 4)) {
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
  this->nodeMap[key] = node;
  return node;
}

// OFFSET: 0x00AD9610
// FUNCTION: fn_17882
bool CPathFinder::AddNeighbor(PathNode* current, int cost, int nextX, int nextY, int metadata) {
  const int totalCost = current->cost + cost;
  PathNode* next = this->FindOrCreateNode(nextX, nextY);
  if (!next) {
    return false;
  }

  if (next->state == 0) {
    next->parent = current;
    next->state = 1;
    next->cost = totalCost;
    next->estimatedCost = totalCost + 10 * (std::abs(nextX - this->destinationX) + std::abs(nextY - this->destinationY));
    next->metadata = metadata;
    this->openList.Push(next);
    return true;
  }

  if (next->cost > totalCost) {
    next->parent = current;
    next->cost = totalCost;
    next->estimatedCost = totalCost + 10 * (std::abs(nextX - this->destinationX) + std::abs(nextY - this->destinationY));
    next->metadata = metadata;

    if (next->state == 1) {
      std::vector<PathNode*>& open = this->openList.Nodes();
      for (auto it = open.begin(); it != open.end(); ++it) {
        if ((*it)->x == next->x && (*it)->y == next->y) {
          std::push_heap(open.begin(), it + 1, NodeTotalGreater());
          return true;
        }
      }
    } else if (next->state == 2) {
      next->state = 1;
    }
  }

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
  for (auto& [name, cached] : this->attrCache) {
    delete cached;
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
