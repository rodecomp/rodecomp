#include "UI/Items/UIMakeTargetProcessWnd.h"

// OFFSET: 0x009A09E0
// FUNCTION: fn_13603
ITEM_INFO UIMakeTargetProcessWnd::GetItemAt(int x, int y) {
  switch (this->makeType) {
  case 994:
  case 995:
  case 996:
  case 997:
  case 998:
  case 999:
  case 1000:
    return ITEM_INFO(0);
  }

  if (y > this->geometry.height - 56 && y < this->geometry.height - 32) {
    if (x >= 4 && x <= 34) {
      return this->materials[0];
    }
    if (x >= 36 && x <= 66) {
      return this->materials[1];
    }
    if (x >= 68 && x <= 98) {
      return this->materials[2];
    }
  }

  return ITEM_INFO(0);
}
