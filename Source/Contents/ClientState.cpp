#include "Contents/ClientState.h"

#include "Client/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00E48B00
// FUNCTION: fn_24225
void CSession::CollectCardItems() {
  this->m_cardItems.clear();
  for (const ItemInfo& item : this->m_inventoryItems) {
    if (item.unknown_0000[0] == 6) {
      this->m_cardItems.push_back(item);
    }
  }
}

// OFFSET: 0x00E48B70
