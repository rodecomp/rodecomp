#include "Contents/TerrainPathSearch.h"

// OFFSET: 0x00AD9840
// FUNCTION: fn_17885
void CPathFinder::Reset() {
  this->nodePool.erase(this->nodePool.begin(), this->nodePool.end());
  this->nodeMap.erase(this->nodeMap.begin(), this->nodeMap.end());
  std::vector<MovementPathNode*>& open = this->openList.Nodes();
  open.erase(open.begin(), open.end());
}
