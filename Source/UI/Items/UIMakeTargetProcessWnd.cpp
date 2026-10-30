#include "UI/Items/UIMakeTargetProcessWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>

struct ClientItemLists {
  bool AddItem1710(const ITEM_INFO& item, unsigned char refresh);
};

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

// OFFSET: 0x009A3D10
// FUNCTION: fn_13614
void UIMakeTargetProcessWnd::Slot25(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.unknown_0000[1] != 0) {
    g_modeMgr.gameMode->SendMsg(134, reinterpret_cast<int>(item.itemId.c_str()), 0, 0, 0);
  } else {
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x009A48C0
