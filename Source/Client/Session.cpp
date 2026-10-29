#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/World/World.h"
#include "Network/TextEncoding.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// STATIC: 0x01732638
// GLOBAL: gl_24474
CSession g_session;

// OFFSET: 0x00DDE860
// FUNCTION: fn_23401
int CSession::MercSkillLevel(int skillId) const {
  const int index = skillId - 8200;
  if (index > -1) {
    return this->m_mercSkillLevels[index];
  }

  return 0;
}

// OFFSET: 0x00E13B00
// FUNCTION: fn_23826
int CSession::SyncUnknown0E98() {
  if (this->unknown_0E98 != this->unknown_5558) {
    this->unknown_0E98 = this->unknown_5558;
  }

  return this->unknown_0E98;
}

// OFFSET: 0x00DF9B90
// FUNCTION: fn_23562
int CSession::IsExchangeItemListFull() {
  return this->GetFilledExchangeItemList() == 10;
}

// OFFSET: 0x00DFFCF0
// FUNCTION: fn_23633
void CSession::ClearItemSelection() {
  for (ITEM_INFO& item : this->m_inventoryItems) {
    if (item.unknown_0074[1] == 1) {
      item.unknown_0074[1] = 0;
    }
  }
}

// OFFSET: 0x00E3CFF0
// FUNCTION: fn_24179
bool CSession::IsDoramJob(int job) const {
  switch (job) {
  case JT_TRIBE_DORAM:
  case JT_DO_SUMMONER:
  case JT_DO_CART_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_DO_CART_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
  case JT_SPIRIT_HANDLER_RIDING:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00E3D090
// FUNCTION: fn_24180
bool CSession::IsEffectRenderEnabled() const {
  return true;
}

// OFFSET: 0x00E3D0A0

// OFFSET: 0x00E3D0F0

// OFFSET: 0x00E3D240

// OFFSET: 0x00E3D320

// OFFSET: 0x00E3DB20
// FUNCTION: fn_24194
bool CSession::ResetUnknownState() {
  this->unknown_59C0 = 1;
  this->unknown_59C4 = 1;
  this->unknown_59C8 = 1;
  this->unknown_59CC = 1;
  this->unknown_59D4 = 1;
  this->unknown_59BC = 1;
  this->unknown_0E94 = 1;
  this->unknown_103C = -1;
  this->unknown_5450 = 0;
  this->m_serverTimeOffset = 0;
  this->m_serverTimeAdjustCount = 0;
  return true;
}

// OFFSET: 0x00DFF1F0
// FUNCTION: fn_23614
char* CSession::ConvertTooltipText(const char* source, char* output) const {
  if (output != source) {
    std::strcpy(output, source);
  }

  for (char* current = output; *current != '\0'; current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0)) {
    if (*current == '_') {
      *current = ' ';
    }
  }

  return output;
}

// OFFSET: 0x00DDE890
// FUNCTION: fn_23402
ITEM_INFO CSession::FindItem1768(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1768) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDE920
// FUNCTION: fn_23403
ITEM_INFO CSession::GetItem1768At(int index) const {
  if (this->unknown_1768.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1768) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDE9C0
// FUNCTION: fn_23404
int CSession::GetItem1768Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1768) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDE9F0
// FUNCTION: fn_23405
ITEM_INFO CSession::FindItem1740(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1740) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEA80
// FUNCTION: fn_23406
ITEM_INFO CSession::GetItem1740At(int index) const {
  if (this->unknown_1740.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1740) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEB20
// FUNCTION: fn_23407
int CSession::GetItem1740Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1740) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDEB50
// FUNCTION: fn_23408
ITEM_INFO CSession::FindItem1750(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1750) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEBE0
// FUNCTION: fn_23409
ITEM_INFO CSession::GetItem1750At(int index) const {
  if (this->unknown_1750.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1750) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEC80
// FUNCTION: fn_23410
int CSession::GetItem1750Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1750) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDECB0
// FUNCTION: fn_23411
ITEM_INFO CSession::FindItem1758(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1758) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO();
}

// OFFSET: 0x00DDED40
// FUNCTION: fn_23412
ITEM_INFO CSession::FindItem1758ByItemId(int itemId) const {
  for (const ITEM_INFO& item : this->unknown_1758) {
    if (std::atoi(item.itemId.c_str()) == itemId) {
      return item;
    }
  }

  return ITEM_INFO();
}

// OFFSET: 0x00DDEDE0
// FUNCTION: fn_23413
ITEM_INFO CSession::GetItem1758At(int index) const {
  if (this->unknown_1758.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1758) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEEB0
// FUNCTION: fn_23414
ITEM_INFO CSession::FindItem1760(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1760) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF000
// FUNCTION: fn_23416
ITEM_INFO CSession::GetItem1760At(int index) const {
  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1760) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF0F0
// FUNCTION: fn_23418
int CSession::GetItem1760Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1760) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDF120
// FUNCTION: fn_23419
ITEM_INFO CSession::FindItem1770(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1770) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF1B0
// FUNCTION: fn_23420
long long CSession::GetItem1760Total() const {
  long long total = 0;
  for (const ITEM_INFO& item : this->unknown_1760) {
    total += static_cast<long long>(item.quantity) * static_cast<int>(item.unknown_0014[1]);
  }

  return total;
}

// OFFSET: 0x00DDF200
// FUNCTION: fn_23421
ITEM_INFO CSession::GetItem1748At(int index) const {
  if (this->unknown_1748.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1748) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DF9C50
// FUNCTION: fn_23563
bool CSession::HasItem1718(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1718) {
    if (item.unknown_0000[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DF9C80
// FUNCTION: fn_23564
bool CSession::HasExchangeItem(std::uint32_t id) const {
  for (int index = 0; index < 10; ++index) {
    if (this->m_exchangeItems[index].quantity != 0 && this->m_exchangeItems[index].unknown_0000[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DDB7E0
// FUNCTION: fn_23355
const char* CSession::GatName2RswName(const char* gatName) {
  // GLOBAL: gl_24763
  static std::string rswName;
  rswName = gatName;
  if (rswName.size() > 4) {
    rswName.resize(rswName.size() - 3);
    rswName += "rsw";
  }

  return rswName.c_str();
}

// OFFSET: 0x00DDEF40
// FUNCTION: fn_23415
ITEM_INFO CSession::FindItem1760ByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->unknown_1760) {
    if (item.itemId == itemId) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF090
// FUNCTION: fn_23417
int CSession::GetItem1760QuantityByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->unknown_1760) {
    if (item.itemId == itemId) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDF2D0
// FUNCTION: fn_23422
SessionRecord0734 CSession::FindRecord0734(int id) const {
  for (const SessionRecord0734& record : this->unknown_0734) {
    if (record.id == id) {
      return record;
    }
  }

  SessionRecord0734 empty;
  return empty;
}

// OFFSET: 0x00DDF320
// FUNCTION: fn_23423
const char* CSession::GetBgmFileName(const char* rswName) {
  std::string name = rswName;
  const std::size_t position = name.rfind("@");
  if (position != std::string::npos && position >= 4) {
    name.erase(position - 4, 3);
  }

  const char* key = name.c_str();
  auto found = std::lower_bound(this->m_bgmFileNames.begin(), this->m_bgmFileNames.end(), key,
                                [](const std::pair<const char*, const char*>& entry, const char* value) { return std::strcmp(entry.first, value) < 0; });
  if (found != this->m_bgmFileNames.end() && std::strcmp(found->first, key) == 0) {
    return found->second;
  }

  return "bgm\\01.mp3";
}

// OFFSET: 0x00DF9CD0
// FUNCTION: fn_23565
bool CSession::HasItem17D4(int itemId) const {
  if (!this->unknown_17D4.empty()) {
    for (const ITEM_INFO& item : this->unknown_17D4) {
      if (std::atoi(item.itemId.c_str()) == itemId) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00DF9D30
// FUNCTION: fn_23566
bool CSession::IsFriendName(const char* name) const {
  for (const MessengerMember& member : this->m_friends) {
    if (member.name == name) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00E1DFB0
// FUNCTION: fn_23889
int CSession::GetSceneUnknown004C() const {
  return static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->unknown_004C;
}
