#include "UI/Item/UIMerchantItemLogWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0099EF50
// FUNCTION: fn_13578
void UIMerchantItemLogWnd::Slot19() {
  bool changed = false;
  for (auto it = this->logs.begin(); it != this->logs.end(); ++it) {
    if (it->name.compare(LookupMessage(581)) == 0) {
      it->name = LookupPlayerName(it->characterId);
      changed = true;
    }
  }

  if (changed) {
    this->Invalidate();
  }
}

// OFFSET: 0x0099F090
// FUNCTION: fn_13579
void UIMerchantItemLogWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}
