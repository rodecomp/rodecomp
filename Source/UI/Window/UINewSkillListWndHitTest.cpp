#include "UI/Window/UINewSkillListWnd.h"
#include "UI/Window/WindowMouseHit.h"

// OFFSET: 0x009B0240
// FUNCTION: fn_13804
int UINewSkillListWnd::SkillIndexAt(int x, int y) {
  if (!this->IsSkillArea(x, y)) {
    return -1;
  }

  int column = 0;
  int row;
  if (g_mouseRefreshGate != 0) {
    row = (y - 34) / 32;
  } else {
    column = (x - 76) / 68;
    row = (y - 44) / 56;
    if (x > column * 68 + 100) {
      return -1;
    }
    if (y > row * 56 + 58) {
      return -1;
    }
  }

  return (this->m_firstRow + row) * this->m_columns + column;
}
