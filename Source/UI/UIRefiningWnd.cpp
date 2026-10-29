#include "UI/UIRefiningWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007F5270
// FUNCTION: fn_09902
void UIRefiningWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46 && drag->source == 1) {
    this->SendMsg(this, 23, drag->id, 0, 0, 0);
  }
  g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x007F52D0

// OFFSET: 0x007F54B0

// OFFSET: 0x007F5500
// FUNCTION: fn_09905
int UIRefiningWnd::SlotAt(int x, int y) {
  int offsetX = x - 11;
  if (static_cast<unsigned>(offsetX) > 242 || static_cast<unsigned>(y - 55) > 52) {
    return -1;
  }
  int index = offsetX / 49;
  if (x > index * 49 + 57 || index >= 5) {
    return -1;
  }
  return index;
}

// OFFSET: 0x007F7030

// OFFSET: 0x007F72A0

// OFFSET: 0x007F72D0

// OFFSET: 0x007F7D80

// OFFSET: 0x007F7E10
// FUNCTION: fn_09919
void UIRefiningWnd::Slot32(int x, int y) {
}

// OFFSET: 0x007F8C40

// OFFSET: 0x007F8C80

// OFFSET: 0x007F8EA0
