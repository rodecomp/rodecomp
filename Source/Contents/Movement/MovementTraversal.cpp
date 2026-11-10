#include "Contents/Movement/TerrainPathSearch.h"

// OFFSET: 0x00AD9550
// FUNCTION: fn_17879
bool CPathFinder::CanMove(int startX, int startY, int targetX, int targetY, char flag) const {
  C3dAttr* const attr = this->attr;
  if (targetX < 0 || targetY < 0 || targetX >= attr->width || targetY >= attr->height) {
    return false;
  }

  const TerrainHeightCell* cell = attr->CellAt(targetX, targetY);
  if (!cell || cell->type == 1 || (flag != 12 && cell->type == 5)) {
    return false;
  }

  if (startX == targetX || startY == targetY) {
    return true;
  }

  const TerrainHeightCell* side = attr->CellAt(startX, targetY);
  if (!side || side->type == 1 || side->type == 5) {
    return false;
  }

  side = attr->CellAt(targetX, startY);
  if (!side || side->type == 1 || side->type == 5) {
    return false;
  }

  return true;
}
