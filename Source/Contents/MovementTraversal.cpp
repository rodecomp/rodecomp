#include "Contents/TerrainPathSearch.h"

// OFFSET: 0x00AD9550
// FUNCTION: fn_17879
bool CPathFinder::CanMove(int startX, int startY, int targetX, int targetY, char flag) const {
  if (targetX < 0 || targetY < 0 || targetX >= this->attr->width || targetY >= this->attr->height) {
    return false;
  }

  const TerrainHeightCell* cell = this->attr->Grid().CellAt(targetX, targetY);
  if (!cell || cell->type == 1 || (flag != 12 && cell->type == 5)) {
    return false;
  }

  if (startX == targetX || startY == targetY) {
    return true;
  }

  const TerrainHeightCell* side = this->attr->Grid().CellAt(startX, targetY);
  if (!side || side->type == 1 || side->type == 5) {
    return false;
  }

  side = this->attr->Grid().CellAt(targetX, startY);
  if (!side || side->type == 1 || side->type == 5) {
    return false;
  }

  return true;
}
