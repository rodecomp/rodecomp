#include "UI/Items/UISwapEquipmentWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

// OFFSET: 0x0082F990
// FUNCTION: fn_10390
void UISwapEquipmentWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  ITEM_INFO item(0);
  unsigned char equipped = 0;
  switch (drag->source) {
  case 1:
    item = LookupClientItemByIdentity(g_session.m_inventoryItems, drag->id);
    break;
  case 2:
    equipped = 1;
    item = g_session.GetItem17F0(drag->slot);
    break;
  case 24:
    equipped = 1;
    item = g_session.GetItem2B50(drag->slot);
    break;
  }

  if (item.unknown_0000[1] > 0) {
    switch (item.unknown_0000[0]) {
    case 10:
    case 16:
    case 17:
    case 19:
      mode->SendMsg(302, drag->id, item.unknown_0000[2], 0, 0);
      break;
    default:
      if (equipped == 1) {
        mode->SendMsg(305, reinterpret_cast<int>(&item), 0, 0, 0);
      } else if (item.unknown_0000[2] != 0 && item.identified != 0 && item.unknown_0000[3] == 0) {
        mode->SendMsg(302, drag->id, item.unknown_0000[2], 0, 0);
      }
      break;
    }
  }

  g_modeMgr.gameMode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00831860
// FUNCTION: fn_10402
void UISwapEquipmentWnd::SetSlotValue(int index, int value) {
  switch (index) {
  case 8:
    this->unknown_00C8 = value;
    break;
  case 9:
    this->unknown_00CC = value;
    break;
  case 0:
    this->unknown_00C4 = value;
    break;
  case 2:
    this->unknown_00D0 = value;
    break;
  }
}
