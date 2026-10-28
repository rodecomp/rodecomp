#include "Client/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <timeapi.h>
#include <windows.h>

// OFFSET: 0x00DE0A50
// FUNCTION: fn_23466
std::uint32_t CSession::GetServerTime() {
  return timeGetTime() - this->m_serverTimeOffset - 72;
}

// OFFSET: 0x00DFEF40
// FUNCTION: fn_23605
std::uint32_t CSession::AddPingSample(std::uint32_t sample) {
  this->m_pingSamples.push_back(sample);

  while (this->m_pingSamples.size() > 3) {
    this->m_pingSamples.pop_front();
  }

  std::uint32_t sum = 0;
  for (std::uint32_t value : this->m_pingSamples) {
    sum += value;
  }

  this->m_averagePing = sum / this->m_pingSamples.size();
  return this->m_averagePing;
}

// OFFSET: 0x00E00130
// FUNCTION: fn_23637
std::uint32_t CSession::SetServerTime(std::uint32_t serverTime) {
  const std::uint32_t offset = timeGetTime() - serverTime;
  this->m_serverTimeOffset = offset;
  this->m_serverTimeAdjustCount = 0;
  return offset;
}

// OFFSET: 0x00E00160
// FUNCTION: fn_23638
void CSession::SetHotkeyDescription(const char* description, int index) {
  for (int slot = 9 * this->m_hotkeyPage; slot < 9 * (this->m_hotkeyPage + 1); ++slot) {
    if (this->m_hotkeyDescriptions[slot] == description) {
      this->m_hotkeyDescriptions[slot] = "nothing";
    }
  }

  if (description[0] == '\0') {
    this->m_hotkeyDescriptions[index] = "nothing";
  } else {
    this->m_hotkeyDescriptions[index] = description;
  }
}

// OFFSET: 0x00E00320
// FUNCTION: fn_23639
void CSession::AddItemSlot(ItemInfo item) {
  for (int slot = 0; slot < 10; ++slot) {
    if (this->m_itemSlots[slot].quantity == 0) {
      this->m_itemSlots[slot] = item;
      break;
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E003D0
// FUNCTION: fn_23640
SessionShop* CSession::GetShop(int shopId) {
  const auto found = this->m_shops.find(shopId);
  if (found != this->m_shops.end()) {
    return &found->second;
  }

  this->m_shops[shopId] = SessionShop{};
  return &this->m_shops[shopId];
}

// OFFSET: 0x00E004D0
// FUNCTION: fn_23641
void CSession::SetShopFlag(int shopId, char flag) {
  if (SessionShop* shop = this->GetShop(shopId)) {
    shop->unknown_0000 = static_cast<unsigned char>(flag);
  }
}

// OFFSET: 0x00E004F0
// FUNCTION: fn_23642
void CSession::SetShopItem(int shopId, unsigned int index, int itemId, int first, int second) {
  SessionShop* shop = this->GetShop(shopId);
  if (shop == nullptr) {
    return;
  }

  if (shop->unknown_0004.size() <= index) {
    shop->unknown_0004.resize(index + 1);
  }

  std::map<int, std::pair<int, int>>& items = shop->unknown_0004[index];
  if (items.find(itemId) == items.end()) {
    items[itemId] = index == 0 ? std::make_pair(1, second) : std::make_pair(first, second);
  }
}

// OFFSET: 0x00E006E0
// FUNCTION: fn_23643
bool CSession::AddShopName(std::string name, int shopId) {
  std::string key = name;
  if (key.empty()) {
    key = this->m_navigationPlayerMap;
  }

  const auto found = this->m_nameToId.find(key);
  if (found != this->m_nameToId.end() && found->second != -1) {
    return true;
  }

  this->m_nameToId[name] = shopId;
  return false;
}

// OFFSET: 0x00E010B0
// FUNCTION: fn_23646
int CSession::SetUnknown8134(int value) {
  this->unknown_8134 = value;
  return value;
}

// OFFSET: 0x00E01670
// FUNCTION: fn_23647
void CSession::ClearUnknown1710(int id) {
  for (ItemInfo& entry : this->m_inventoryItems) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016A0
// FUNCTION: fn_23648
void CSession::ClearUnknown1740(int id) {
  for (ItemInfo& entry : this->unknown_1740) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E016D0
// FUNCTION: fn_23649
void CSession::ClearUnknown1738(int id) {
  for (ItemInfo& entry : this->unknown_1738) {
    if (entry.unknown_0000[1] == static_cast<std::uint32_t>(id)) {
      entry.unknown_0074[2] = 0;
      break;
    }
  }
}

// OFFSET: 0x00E01700
// FUNCTION: fn_23650
std::uint32_t CSession::SyncServerTime(std::uint32_t serverTime) {
  const std::uint32_t current = timeGetTime() - this->m_serverTimeOffset - 72;
  if (current + 72 < serverTime) {
    this->m_serverTimeOffset = timeGetTime() - serverTime;
    this->m_serverTimeAdjustCount = 0;
    return this->m_serverTimeOffset;
  }

  if (current + 72 > serverTime + 144) {
    if (++this->m_serverTimeAdjustCount < 4) {
      return serverTime + 144;
    }

    this->m_serverTimeOffset = timeGetTime() + 72 - current;
    this->m_serverTimeAdjustCount = 0;
    return this->m_serverTimeOffset;
  }

  this->m_serverTimeAdjustCount = 0;
  return serverTime + 144;
}
