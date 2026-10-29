#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/GuildInfo.h"
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

// GLOBAL: gl_24673
std::list<GuildInfo::Member> g_guildMembers;

// GLOBAL: gl_24864
std::string g_guildName;

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
// FUNCTION: fn_24181
bool CSession::CheckInterval(long long interval) {
  const int now = static_cast<int>(timeGetTime());
  if (now < this->m_lastIntervalTime + interval) {
    return false;
  }

  this->m_lastIntervalTime = now;
  return true;
}

// OFFSET: 0x00E3D0F0
// FUNCTION: fn_24182
bool CSession::IsPlayerPerformerJob(int) const {
  switch (g_session.m_job) {
  case JT_BARD:
  case JT_DANCER:
  case JT_BARD_H:
  case JT_DANCER_H:
  case JT_BARD_B:
  case JT_DANCER_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00E3D240
// FUNCTION: fn_24183
bool CSession::IsSpecialMonsterJob(int job) const {
  switch (job) {
  case 331:
  case JT_WISH_MAIDEN:
  case JT_I_G_RAKE_SCARABA:
  case JT_GM_HINALLE:
  case JT_G_OBSIDIAN:
  case JT_C4_ZOMBIE:
  case JT_C5_ZAKUDAM:
  case JT_C3_HARPY:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00E3D320
// FUNCTION: fn_24187
bool CSession::IsWithinUnknown59C0(int value) const {
  if (value == -1) {
    return false;
  }

  return this->unknown_59C0 - this->unknown_8130 <= value;
}

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
  if (!this->unknown_1784[10].empty()) {
    for (const ITEM_INFO& item : this->unknown_1784[10]) {
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

// OFFSET: 0x00DDB4D0

// OFFSET: 0x00DDC050

// OFFSET: 0x00DDC0A0

// OFFSET: 0x00DDC0F0

// OFFSET: 0x00DDC330

// OFFSET: 0x00DDC580

// OFFSET: 0x00DDC7A0

// OFFSET: 0x00DDC830

// OFFSET: 0x00DDC9D0

// OFFSET: 0x00DDD300

// OFFSET: 0x00DDD400

// OFFSET: 0x00DDD710

// OFFSET: 0x00DDD7B0

// OFFSET: 0x00DDD9A0

// OFFSET: 0x00DDF5B0
// FUNCTION: fn_23424
ITEM_INFO CSession::GetItem1784At(int list, int index) const {
  if (list < 0) {
    return ITEM_INFO(0);
  }

  if (static_cast<unsigned int>(list) < 11 && !this->unknown_1784[list].empty()) {
    int position = 0;
    for (const ITEM_INFO& item : this->unknown_1784[list]) {
      if (position == index) {
        return item;
      }
      ++position;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF7B0
// FUNCTION: fn_23425
int CSession::GetCommandId(const char* name) const {
  auto found = std::lower_bound(this->m_commandIds.begin(), this->m_commandIds.end(), name,
                                [](const std::pair<const char*, int>& entry, const char* value) { return std::strcmp(entry.first, value) < 0; });
  if (found != this->m_commandIds.end() && std::strcmp(found->first, name) == 0) {
    return found->second;
  }

  return 3;
}

// OFFSET: 0x00DDF890
// FUNCTION: fn_23426
int CSession::GetItem1728Count() const {
  return static_cast<int>(this->unknown_1728.size());
}

// OFFSET: 0x00DDFA50
// FUNCTION: fn_23442
ITEM_INFO CSession::GetItemSlot(int index) const {
  return this->m_itemSlots[index];
}

// OFFSET: 0x00DDFAC0
// FUNCTION: fn_23443
ITEM_INFO CSession::GetItem21A0(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (index >= 0 && index < 10) {
    return this->unknown_21A0[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDFD30
// FUNCTION: fn_23445
int CSession::GetItem1718QuantityByItemId(const ITEM_INFO& item) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->unknown_1718.begin(); it != this->unknown_1718.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      return it->quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFDB0
// FUNCTION: fn_23446
int CSession::GetItem1718QuantityOf(const ITEM_INFO& item) const {
  for (const ITEM_INFO& value : this->unknown_1718) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      return value.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFDE0
// FUNCTION: fn_23447
std::uint16_t CSession::GetItem1718Unknown006CByItemId(const ITEM_INFO& item) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->unknown_1718.begin(); it != this->unknown_1718.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == std::atoi(item.itemId.c_str())) {
      return it->unknown_006C;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFE60
// FUNCTION: fn_23448
std::uint16_t CSession::GetItem1718Unknown006COf(const ITEM_INFO& item) const {
  for (const ITEM_INFO& value : this->unknown_1718) {
    if (value.unknown_0000[1] == item.unknown_0000[1]) {
      return value.unknown_006C;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFE90
// FUNCTION: fn_23449
std::uint16_t CSession::SumItem1720Unknown006CByItemId() const {
  std::uint16_t total = 0;
  for (std::list<ITEM_INFO>::const_iterator it = this->unknown_1720.begin(); it != this->unknown_1720.end(); ++it) {
    total += it->quantity * this->GetItem1718Unknown006CByItemId(*it);
  }

  return total;
}

// OFFSET: 0x00DDFF60
// FUNCTION: fn_23450
std::uint16_t CSession::SumItem1720Unknown006C() const {
  std::uint16_t total = 0;
  for (const ITEM_INFO& item : this->unknown_1720) {
    total += item.quantity * this->GetItem1718Unknown006COf(item);
  }

  return total;
}

// OFFSET: 0x00DE0620
// FUNCTION: fn_23456
ITEM_INFO CSession::FindItem1720ByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->unknown_1720) {
    if (item.itemId == itemId) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE06E0
// FUNCTION: fn_23457
ITEM_INFO CSession::GetItem1720At(int index) const {
  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1720) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0770
// FUNCTION: fn_23458
int CSession::GetItem1720QuantityByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->unknown_1720) {
    if (item.itemId == itemId) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE07D0
// FUNCTION: fn_23459
int CSession::GetItem1720Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1720) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0800
// FUNCTION: fn_23460
int CSession::GetItem1720Total() const {
  int total = 0;
  for (const ITEM_INFO& item : this->unknown_1720) {
    total += item.unknown_0014[1] * item.quantity;
  }

  return total;
}

// OFFSET: 0x00DE08E0
// FUNCTION: fn_23462
const char* CSession::GetMonsterRmaName(int job) const {
  return "캐릭터\\칼질몬스터.rma";
}

// OFFSET: 0x00DE08F0
// FUNCTION: fn_23463
ITEM_INFO CSession::FindItem1730(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1730) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0980
// FUNCTION: fn_23464
ITEM_INFO CSession::GetItem1730At(int index) const {
  if (this->unknown_1730.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1730) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0A20
// FUNCTION: fn_23465
int CSession::GetItem1730Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1730) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0D30
// FUNCTION: fn_23470
ITEM_INFO CSession::GetItem1718At(int index) const {
  if (this->unknown_1718.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1718) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0DD0
// FUNCTION: fn_23471
int CSession::GetItem1718Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1718) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0E00
// FUNCTION: fn_23472
const char* CSession::GetHotkeyDescription(int index) const {
  return this->m_hotkeyDescriptions[index].c_str();
}

// OFFSET: 0x00DE0EB0
// FUNCTION: fn_23475
ITEM_INFO CSession::FindItem1738(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1738) {
    if (item.unknown_0000[1] == id) {
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0F40
// FUNCTION: fn_23476
ITEM_INFO CSession::GetItem1738At(int index) const {
  if (this->unknown_1738.empty()) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->unknown_1738) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0FE0
// FUNCTION: fn_23477
int CSession::GetItem1738Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->unknown_1738) {
    if (item.unknown_0000[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDB540
// FUNCTION: fn_23353
bool CSession::RemoveFriendByCharacterId(std::uint32_t characterId) {
  for (std::list<MessengerMember>::iterator it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    if (it->characterId == characterId) {
      this->m_friends.erase(it);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DF9F10
// FUNCTION: fn_23568
bool CSession::IsGuildMemberAccount(std::uint32_t accountId) const {
  std::list<GuildInfo::Member> members(g_guildMembers.begin(), g_guildMembers.end());
  for (const GuildInfo::Member& member : members) {
    if (static_cast<std::uint32_t>(member.accountId) == accountId) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFA030
// FUNCTION: fn_23569
bool CSession::IsGuildMemberCharacter(std::uint32_t characterId) const {
  std::list<GuildInfo::Member> members(g_guildMembers.begin(), g_guildMembers.end());
  for (const GuildInfo::Member& member : members) {
    if (static_cast<std::uint32_t>(member.characterId) == characterId) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DF9D90
// FUNCTION: fn_23567
bool CSession::IsGuildMemberName(const char* name) const {
  std::list<GuildInfo::Member> members(g_guildMembers.begin(), g_guildMembers.end());
  for (const GuildInfo::Member& member : members) {
    if (member.name == name) {
      return true;
    }
  }

  return g_guildName == name;
}

// OFFSET: 0x00DDB580
// FUNCTION: fn_23354
void CSession::RemovePartyMemberByName(const char* name) {
  for (std::list<MessengerMember>::iterator it = this->m_partyMembers.begin(); it != this->m_partyMembers.end(); ++it) {
    if (it->name != name) {
      continue;
    }

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode == nullptr) {
      return;
    }

    CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(it->accountId);
    if (actor != nullptr && static_cast<EffectActor*>(actor)->state.Attachments().HasId(160)) {
      actor->SendMsg(nullptr, 109, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    this->m_partyMembers.erase(it);
    return;
  }
}
