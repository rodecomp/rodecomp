#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/World/World.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/TextEncoding.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowOrdering.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/XorBytes.h"
#include "Script/LuaScript.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <mbstring.h>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>

// STATIC: 0x01732638
// GLOBAL: gl_24474
CSession g_session;

// GLOBAL: gl_24673
std::list<GuildInfo::Member> g_guildMembers;

// GLOBAL: gl_24864
std::string g_guildName;

// GLOBAL: gl_25140
static std::string s_messageEndTag = "</MSG>";

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

// OFFSET: 0x00E3D390
// FUNCTION: fn_24189
int CSession::HasItem61B8(std::uint32_t id) const {
  if (id == 0) {
    return 0;
  }

  for (int i = 0; i < 10; ++i) {
    if (id == this->unknown_61B8[i].unknown_0000[1] || id == this->unknown_6B68[i].unknown_0000[1]) {
      return 1;
    }
  }

  return 0;
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
// FUNCTION: fn_23352
bool CSession::IsNumberWithCommas(char* text) {
  const int length = static_cast<int>(std::strlen(text));
  for (int i = 0; i < length; ++i) {
    if (text[i] == ',') {
      for (int j = i; j < length; ++j) {
        text[j] = j == length - 1 ? ' ' : text[j + 1];
      }
    }

    if ((text[i] < '0' || text[i] > '9') && text[i] != ' ') {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00DDC050
// FUNCTION: fn_23361
int CSession::FindUnknown0708(int key) const {
  auto found = this->unknown_0708.find(key);
  if (found != this->unknown_0708.end()) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00DDC0A0
// FUNCTION: fn_23362
int CSession::FindUnknown0700(int key) const {
  auto found = this->unknown_0700.find(key);
  if (found != this->unknown_0700.end()) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00DDC0F0
// FUNCTION: fn_23363
const char* CSession::GetEnemyHitWaveName(int element) {
  switch (element) {
  case 3:
    switch (std::rand() % 2) {
    case 0:
      return "_enemy_hit_fire1.wav";
    case 1:
      return "_enemy_hit_fire2.wav";
    }
    break;
  case 4:
    switch (std::rand() % 2) {
    case 0:
      return "_enemy_hit_wind1.wav";
    case 1:
      return "_enemy_hit_wind2.wav";
    }
    break;
  }

  switch (std::rand() % 4) {
  case 0:
    return "_enemy_hit_normal1.wav";
  case 1:
    return "_enemy_hit_normal2.wav";
  case 2:
    return "_enemy_hit_normal3.wav";
  default:
    return "_enemy_hit_normal4.wav";
  }
}

// OFFSET: 0x00DDC1D0
// FUNCTION: fn_23364
std::string CSession::GetBodyPalName(int job, int sex, int palette, int costumeJob, int costume, int unused) {
  return g_equipmentResourceMgr->GetBodyPalName(job, sex, palette, costumeJob, costume, unused);
}

// OFFSET: 0x00DDC2C0
// FUNCTION: fn_23365
std::string CSession::GetRaceBodyResourceName(int type, int raceJob, int job, int sex, int costumeJob) {
  return g_equipmentResourceMgr->GetRaceBodyResourceName(type, raceJob, job, sex, costumeJob);
}

// OFFSET: 0x00DDC330
// FUNCTION: fn_23366
ITEM_INFO* CSession::GetEquippedItem(const ITEM_INFO* item) {
  if (item == nullptr) {
    return nullptr;
  }

  if (item->unknown_0000[3] != 0) {
    if (item->unknown_0000[3] != 0x8000 || this->unknown_16EC == item->unknown_0000[1]) {
      return nullptr;
    }

    for (ITEM_INFO& value : this->m_inventoryItems) {
      if (value.unknown_0000[1] == this->unknown_16EC) {
        return &value;
      }
    }

    return nullptr;
  }

  ITEM_INFO* equipped = nullptr;
  switch (item->unknown_0000[2]) {
  case 0x1:
    equipped = &this->unknown_17F0[0];
    break;
  case 0x2:
  case 0x22:
    equipped = &this->unknown_17F0[1];
    break;
  case 0x4:
    equipped = &this->unknown_17F0[2];
    break;
  case 0x88:
    equipped = &this->unknown_17F0[3];
    break;
  case 0x10:
    equipped = &this->unknown_17F0[4];
    break;
  case 0x20:
    equipped = &this->unknown_17F0[5];
    break;
  case 0x40:
    equipped = &this->unknown_17F0[6];
    break;
  case 0x100:
  case 0x101:
  case 0x300:
  case 0x301:
    equipped = &this->unknown_17F0[8];
    break;
  case 0x200:
  case 0x201:
    equipped = &this->unknown_17F0[9];
    break;
  case 0x1000:
    equipped = &this->unknown_2B50[0];
    break;
  case 0x20000:
    equipped = &this->unknown_2B50[1];
    break;
  case 0x2000:
    equipped = &this->unknown_2B50[2];
    break;
  case 0x100000:
    equipped = &this->unknown_2B50[3];
    break;
  case 0x10000:
    equipped = &this->unknown_2B50[4];
    break;
  case 0x40000:
    equipped = &this->unknown_2B50[5];
    break;
  case 0x80000:
    equipped = &this->unknown_2B50[6];
    break;
  case 0x200000:
    equipped = &this->unknown_2B50[7];
    break;
  case 0x400:
  case 0xC00:
  case 0x1400:
  case 0x1C00:
    equipped = &this->unknown_2B50[8];
    break;
  case 0x800:
  case 0x1800:
    equipped = &this->unknown_2B50[9];
    break;
  }

  if (equipped != nullptr && equipped->quantity != 0) {
    return equipped;
  }

  return nullptr;
}

// OFFSET: 0x00DDC580
// FUNCTION: fn_23367
ITEM_INFO CSession::FindItem2B50(std::uint32_t id, int* index) const {
  for (int i = 0; i < 10; ++i) {
    if (this->unknown_2B50[i].unknown_0000[1] == id) {
      *index = i;
      return this->unknown_2B50[i];
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDC7A0
// FUNCTION: fn_23369
ITEM_INFO CSession::FindItem17F0(std::uint32_t id, int* index) const {
  for (int i = 0; i < 10; ++i) {
    if (this->unknown_17F0[i].unknown_0000[1] == id) {
      *index = i;
      return this->unknown_17F0[i];
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDC830
// FUNCTION: fn_23370
ITEM_INFO CSession::GetExchangeItem(int index) const {
  return this->m_exchangeItems[index];
}

// OFFSET: 0x00DDC9D0

// OFFSET: 0x00DDD300
// FUNCTION: fn_23384
std::uint16_t CSession::FindInventoryIndexByItemId(int itemId) {
  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (std::atoi(item.itemId.c_str()) == itemId) {
      return static_cast<std::uint16_t>(item.unknown_0000[1]);
    }
  }

  return 0xFFFF;
}

// OFFSET: 0x00DDD400
// FUNCTION: fn_23385
std::vector<int> CSession::FindInventoryIndicesByItemId(int itemId, unsigned char identifiedOnly) {
  std::vector<int> indices;
  for (const ITEM_INFO& item : this->m_inventoryItems) {
    if (std::atoi(item.itemId.c_str()) == itemId && (identifiedOnly != 1 || item.identified == 1)) {
      indices.push_back(static_cast<int>(item.unknown_0000[1]));
    }
  }

  return indices;
}

// OFFSET: 0x00DDD710
// FUNCTION: fn_23388
ITEM_INFO CSession::GetInventoryItemAt(int index) const {
  if (this->m_inventoryItems.size() == 0) {
    return ITEM_INFO(0);
  }

  int position = 0;
  for (const ITEM_INFO& item : this->m_inventoryItems) {
    if (position == index) {
      return item;
    }
    ++position;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDD7B0
// FUNCTION: fn_23389
ITEM_INFO* CSession::FindInventoryItemByItemId(int itemId, bool identifiedOnly) {
  if (this->m_inventoryItems.size() == 0) {
    return nullptr;
  }

  for (ITEM_INFO& item : this->m_inventoryItems) {
    if (std::atoi(item.itemId.c_str()) == itemId && (!identifiedOnly || item.identified != 0)) {
      return &item;
    }
  }

  return nullptr;
}

// OFFSET: 0x00DDD9A0
// FUNCTION: fn_23390
int CSession::GetInventoryItemCount(int itemId) const {
  int count = 0;
  for (const ITEM_INFO& item : this->m_inventoryItems) {
    if (std::atoi(item.itemId.c_str()) == itemId) {
      count += item.quantity;
    }
  }

  return count;
}

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

// OFFSET: 0x00DFA150
// FUNCTION: fn_23570
bool CSession::IsGuildMemberCharacter(int characterId, char* name) const {
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

  return std::strcmp(g_guildName.c_str(), name) == 0;
}

// OFFSET: 0x00DFA160
// FUNCTION: fn_23571
bool CSession::IsUnknownName5248(const char* name) const {
  std::string value(name);
  const std::size_t position = value.rfind("@");
  if (position != std::string::npos && position >= 4) {
    value.erase(position - 4, 3);
  }

  return std::binary_search(this->unknown_5248.begin(), this->unknown_5248.end(), value.c_str(),
                            [](const char* left, const char* right) { return std::strcmp(left, right) < 0; });
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

namespace {
#pragma pack(push, 1)
  struct PACKET_CZ_USE_SKILL_END {
    /* 0000 */ std::int16_t packetType;
    /* 0002 */ std::uint16_t skillId;
  };
#pragma pack(pop)
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PACKET_CZ_USE_SKILL_END) == 4);
#endif

// OFFSET: 0x00E2ED90
// FUNCTION: fn_24151
void CSession::AddUnknown7F94(std::uint32_t id) {
  this->unknown_7F94.insert(id);
}

// OFFSET: 0x00E2F3B0
// FUNCTION: fn_24153
void CSession::EndSkillOption() {
  if (this->GetOption(2) != 0) {
    this->unknown_61A4 = 0;
    PACKET_CZ_USE_SKILL_END packet;
    packet.packetType = ID_CZ_USE_SKILL_END;
    packet.skillId = GC_ROLLINGCUTTER;
    GetRagConnection().Send(sizeof(packet), &packet);
    this->m_options.Set(2, 0);
  }
}

// OFFSET: 0x00E2F520
// FUNCTION: fn_24154
void CSession::ClearNormalItemSlots() {
  std::memset(this->m_normalItemSlots, 0, sizeof(this->m_normalItemSlots));
}

// OFFSET: 0x00E2F540
// FUNCTION: fn_24155
void CSession::ResetItems6B68() {
  for (int index = 0; index < 10; ++index) {
    this->unknown_6B68[index].Reset();
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2F570
// FUNCTION: fn_24156
void CSession::ResetItems61B8() {
  for (int index = 0; index < 10; ++index) {
    this->unknown_61B8[index].Reset();
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2F5A0
// FUNCTION: fn_24157
bool CSession::IsPartyRecommendTarget(std::uint32_t aid) {
  if (g_session.m_level < 70 || g_session.IsBabyJob(g_session.GetDisplayJob()) == true || g_session.unknown_5468 == 0) {
    return false;
  }

  CGameActor* actor = static_cast<CGameActor*>(g_session.FindGameActor(aid));

  if (!actor) {
    return false;
  }

  const std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(aid);
  if (ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyByName(name.c_str()).valid == 0) {
    return false;
  }

  return static_cast<std::int16_t>(actor->unknown_0210) <= 50 && g_session.IsBabyJob(aid) != true &&
         static_cast<unsigned>(aid - JT_NOVICE_H) > 20;
}

// OFFSET: 0x00E2F710
// FUNCTION: fn_24158
int CSession::GetBodyStyle(int job, int style, int renderJob) {
  if (style >= 2 || (renderJob >= JT_RUNE_KNIGHT_2ND && renderJob < JT_SECOND_JOB_END)) {
    return style;
  }

  if (static_cast<unsigned>(job - JT_NOVICE_H) <= 21) {
    return job <= JT_THIEF_H;
  }

  return static_cast<unsigned>(job - JT_NOVICE_H) <= 6;
}

// OFFSET: 0x00E2F920
// FUNCTION: fn_24160
std::vector<unsigned char> SerializeItems(const std::list<ITEM_INFO>& items) {
  std::vector<unsigned char> result;

  for (std::list<ITEM_INFO>::const_iterator it = items.begin(); it != items.end(); ++it) {
    const std::vector<unsigned char> bytes = it->Serialize();
    result.insert(result.end(), bytes.begin(), bytes.end());
  }

  return result;
}

// OFFSET: 0x00E2F9E0
// FUNCTION: fn_24161
int CSession::GetUnknown8148() const {
  return this->unknown_8148;
}

// OFFSET: 0x00DFF060
// FUNCTION: fn_23606
void CSession::RefreshWindow02C8() {
  if (UIWindow* window = g_windowMgr.window_02C8) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF080
// FUNCTION: fn_23607
void CSession::RefreshItem1740Window() {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F4)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF0A0
// FUNCTION: fn_23608
void CSession::RefreshItem1750Window(int type) {
  std::uint32_t window;
  if (type == 1) {
    window = g_windowMgr.unknown_0410;
  } else {
    window = type == 2 ? g_windowMgr.unknown_0420 : g_windowMgr.unknown_02F8;
  }

  if (window) {
    reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF0E0
// FUNCTION: fn_23609
void CSession::RefreshItem1768Window(unsigned char alternate, int argument) {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(alternate ? g_windowMgr.unknown_040C : g_windowMgr.unknown_02FC)) {
    window->SendMsg(nullptr, 23, argument, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF120
// FUNCTION: fn_23610
void CSession::UpdateUnknown1708(int enabled) {
  this->unknown_1708 = g_serviceOptions.Get(enabled ? 45 : 46);
}

// OFFSET: 0x00DFF150
// FUNCTION: fn_23611
void __fastcall sub_DFF150(CSession*) {
  if (UIWindow* window = g_windowMgr.window_02C8) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
  reinterpret_cast<void(__thiscall*)(UIWindowMgr*)>(0x00A96090)(&g_windowMgr);
}

// OFFSET: 0x00DFF180
// FUNCTION: fn_23612
void CSession::RefreshItem1738Window() {
  if (UIWindow* window = g_windowMgr.window_0288) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF1A0
// FUNCTION: fn_23613
void CSession::EraseRecord0734(int id) {
  for (auto it = this->unknown_0734.begin(); it != this->unknown_0734.end(); ++it) {
    if (id == it->id) {
      this->unknown_0734.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00DFF250
// FUNCTION: fn_23615
void CSession::IgnoreText(std::string text) {
}

// OFFSET: 0x00DFF2A0
// FUNCTION: fn_23616
void CSession::SaveMiniPartyInfo() {
  std::string text;

  for (auto it = this->unknown_0734.begin(); it != this->unknown_0734.end(); ++it) {
    char line[512];
    std::memset(line, 0, sizeof(line));
    sprintf_s(line, "{%s,%d,%d,%d}\n", g_session.DecodePlayerName(), it->unknown_0000, it->id, it->unknown_0008);
    text += line;
  }

  FILE* file = std::fopen("savedata\\MiniPartyInfo.lua", "w");
  if (file) {
    std::fprintf(file, text.c_str());
    std::fclose(file);
  }
}

// OFFSET: 0x00DFF480
// FUNCTION: fn_23617
void CSession::SetItem1768Quantity(int index, int quantity) {
  int position = 0;
  for (ITEM_INFO& item : this->unknown_1768) {
    if (position == index) {
      item.quantity = quantity;
      return;
    }
    ++position;
  }
}

// OFFSET: 0x00DFF4B0
// FUNCTION: fn_23618
void CSession::SetUnknown5C1C(int value) {
  this->unknown_5C1C = value;
  if (UIWindow* window = g_windowMgr.unknown_01E0) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF4E0
// FUNCTION: fn_23619
void CSession::UpdateFriendName(std::uint32_t characterId, const char* name) {
  for (auto it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    if (it->characterId == characterId) {
      if (std::strlen(name) == 0) {
        g_modeMgr.gameMode->SendMsg(176, it->accountId, it->characterId, 0, 0);
        this->m_friends.erase(it);
      } else {
        it->name = name;
      }
      break;
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(34)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00DFF870
// FUNCTION: fn_23622
void CSession::SetNavigationPlayerMap(const char* name) {
  if (name) {
    StringFormat(this->m_navigationPlayerMap, 20, _TRUNCATE, name);
  } else {
    this->m_navigationPlayerMap[0] = '\0';
  }
}

// OFFSET: 0x00DFF8A0
// FUNCTION: fn_23623
void SetMemberFlags(std::uint32_t* flags) {
  *flags |= 0x40;
}

// OFFSET: 0x00DFF8B0

// OFFSET: 0x00DFFA00
// FUNCTION: fn_23626
int CSession::SetFriendOffline(std::uint32_t accountId, std::uint32_t characterId, unsigned char offline) {
  for (MessengerMember& member : this->m_friends) {
    if (member.accountId == accountId && member.characterId == characterId) {
      member.offline = offline;
      if (g_serviceOptions.Get(165)) {
        if (member.name.empty() && g_modeMgr.GetGameMode()) {
          member.name = LookupPlayerName(member.characterId);
        }

        char message[256];
        if (member.offline == 0) {
          std::sprintf(message, LookupMessage(1041), member.name.c_str());
        } else {
          std::sprintf(message, LookupMessage(1042), member.name.c_str());
        }
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 0, 0, 0);
      }
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00DFFB70
// FUNCTION: fn_23627
void CSession::SetHomunSkillLevel(int skillId, int level) {
  const int index = skillId - 8000;
  if (index > -1) {
    this->m_homunSkillLevels[index] = level;
  }
}

// OFFSET: 0x00DFFBA0
// FUNCTION: fn_23628
void CSession::UpdateInventoryItem(const ITEM_INFO& item, unsigned char refresh) {
  auto it = this->m_inventoryItems.begin();
  while (it != this->m_inventoryItems.end() && it->unknown_0000[1] != item.unknown_0000[1]) {
    ++it;
  }

  if (it == this->m_inventoryItems.end()) {
    this->m_inventoryItems.push_back(item);
  } else if (item.quantity == 0) {
    this->m_inventoryItems.erase(it);
  } else {
    *it = item;
  }

  if (refresh) {
    RefreshItemWindows(&g_windowMgr);
  }
}

// OFFSET: 0x00DFFC10
// FUNCTION: fn_23629
void CSession::SetMercSkillLevel(int skillId, int level) {
  const int index = skillId - 8200;
  if (index > -1) {
    this->m_mercSkillLevels[index] = level;
  }
}

// OFFSET: 0x00DFFC40
// FUNCTION: fn_23630
void CSession::SetItem1768Unknown0014(int index, std::uint32_t value) {
  int position = 0;
  for (ITEM_INFO& item : this->unknown_1768) {
    if (position == index) {
      item.unknown_0014[0] = value;
      item.unknown_0014[1] = value;
      return;
    }
    ++position;
  }
}

// OFFSET: 0x00DFFC80
// FUNCTION: fn_23631
void CSession::AddRecord0734(int id, std::uint32_t value) {
  SessionRecord0734 record;
  record.unknown_0000 = 1;
  record.id = id;
  record.unknown_0008 = value;
  this->unknown_0734.push_back(record);
}

// OFFSET: 0x00DFFCB0
// FUNCTION: fn_23632
void CSession::SetUnknown5210(int value) {
  this->unknown_5210 = value;
  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFFD20
// FUNCTION: fn_23634
void CSession::SetUnknown5214(int value) {
  this->unknown_5214 = value;
  if (UIWindow* window = g_windowMgr.LookupWindow(10011)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E000D0
// FUNCTION: fn_23635
void CSession::SetPlayerPosition(int x, int y, int direction) {
  this->m_playerDir = direction;
  this->m_playerX = x;
  this->m_playerY = y;
}

// OFFSET: 0x00E00100
// FUNCTION: fn_23636
void CSession::SetRecord0734Value(int id, std::uint32_t value) {
  for (SessionRecord0734& record : this->unknown_0734) {
    if (record.id == id) {
      record.unknown_0008 = value;
      return;
    }
  }
}

// OFFSET: 0x00E31B20
// FUNCTION: fn_24174
ITEM_INFO CSession::GetItem6B68(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (static_cast<unsigned int>(index) <= 9) {
    return this->unknown_6B68[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E31BB0
// FUNCTION: fn_24175
ITEM_INFO CSession::GetItem61B8(int index) const {
  if (index == 15) {
    return LookupClientItemByIdentity(this->m_inventoryItems, this->unknown_7EC8);
  }

  if (static_cast<unsigned int>(index) <= 9) {
    return this->unknown_61B8[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E31DE0
// FUNCTION: fn_24177
bool CSession::IsRequestReady(int requestId) {
  std::list<SessionRequestTimer>::iterator it = this->m_requestTimers.begin();
  for (; it != this->m_requestTimers.end(); ++it) {
    if (requestId == it->requestId || it->requestId == 0x241) {
      break;
    }
  }
  if (it != this->m_requestTimers.end() && it->readyTime - 100 > timeGetTime()) {
    return false;
  }
  return true;
}

// OFFSET: 0x00DF9400
// FUNCTION: fn_23514
void CSession::RefreshHpSpWindows() {
  if (!g_modeMgr.GetGameMode()) {
    return;
  }

  if (g_windowMgr.unknown_01DC) {
    if (this->unknown_556C < this->unknown_5568 ||
        *reinterpret_cast<const int*>(&this->unknown_5570[4]) < *reinterpret_cast<const int*>(&this->unknown_5570[0])) {
      return;
    }

    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->Invalidate();
    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01DC)->InvalidateChildren();
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->NotifyItemBounds();
}

// OFFSET: 0x00DF9460
// FUNCTION: fn_23515
void CSession::RefreshHomunculusInfo() {
  if (g_modeMgr.GetGameMode() && g_windowMgr.window_03C0) {
    g_windowMgr.window_03C0->Invalidate();
    g_windowMgr.window_03C0->InvalidateChildren();
  }
}

// OFFSET: 0x00DF94D0
// FUNCTION: fn_23517
void CSession::RefreshMercenaryInfo() {
  if (g_modeMgr.GetGameMode() && g_windowMgr.window_03C8) {
    g_windowMgr.window_03C8->Invalidate();
    g_windowMgr.window_03C8->InvalidateChildren();
  }
}

// OFFSET: 0x00DF9520
// FUNCTION: fn_23519
bool CSession::IsBlankString(const char* text) const {
  const std::size_t length = std::strlen(text);
  if (length == 0) {
    return true;
  }

  if (text[0] == ' ') {
    return true;
  }

  return text[length - 1] == ' ';
}

// OFFSET: 0x00DF9AA0
// FUNCTION: fn_23561
bool CSession::HasNoUnknown1784Items() const {
  for (int i = 0; i < 11; ++i) {
    if (!this->unknown_1784[i].empty()) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00DFA3C0
// FUNCTION: fn_23573
bool CSession::HasItem1740Number(int value) const {
  for (const ITEM_INFO& item : this->unknown_1740) {
    if (std::atoi(item.itemId.c_str()) == value) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFA420
// FUNCTION: fn_23574
bool CSession::HasUnknown809C(std::string name) {
  return std::find(this->unknown_809C.begin(), this->unknown_809C.end(), name) != this->unknown_809C.end();
}

// OFFSET: 0x00DFA4C0
// FUNCTION: fn_23575
bool CSession::IsKnownName(std::string name) const {
  if (name == this->DecodePlayerName()) {
    return true;
  }

  for (const MessengerMember& member : this->m_friends) {
    if (member.name == name.c_str()) {
      return true;
    }
  }

  GuildInfo* guildInfo = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
  if (guildInfo->GetMemberAccountId(name.c_str()) != -1) {
    return true;
  }

  return name.find(s_messageEndTag) != std::string::npos;
}

// OFFSET: 0x00DFC0B0
// FUNCTION: fn_23593
void CSession::FlagInventoryItem(std::uint32_t id) {
  for (ITEM_INFO& item : this->m_inventoryItems) {
    if (item.unknown_0000[1] == id) {
      item.unknown_0074[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC0F0
// FUNCTION: fn_23594
void CSession::FlagItem1740(std::uint32_t id) {
  for (ITEM_INFO& item : this->unknown_1740) {
    if (item.unknown_0000[1] == id) {
      item.unknown_0074[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC130
// FUNCTION: fn_23595
void CSession::FlagItem1738(std::uint32_t id) {
  for (ITEM_INFO& item : this->unknown_1738) {
    if (item.unknown_0000[1] == id) {
      item.unknown_0074[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC170
// FUNCTION: fn_23596
void CSession::CollectItems1750() {
  this->unknown_1750.erase(this->unknown_1750.begin(), this->unknown_1750.end());

  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (item.unknown_0000[1] > 0) {
      const int itemId = std::atoi(item.itemId.c_str());
      if (std::find(this->unknown_1778.begin(), this->unknown_1778.end(), itemId) != this->unknown_1778.end()) {
        this->unknown_1750.push_back(item);
      }
    }
  }
}

// OFFSET: 0x00DFC330
// FUNCTION: fn_23597
void CSession::RefreshItems1750() {
  this->unknown_1750.erase(this->unknown_1750.begin(), this->unknown_1750.end());

  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (item.unknown_0000[1] > 0) {
      const int itemId = std::atoi(item.itemId.c_str());
      if (std::find(this->unknown_1778.begin(), this->unknown_1778.end(), itemId) != this->unknown_1778.end()) {
        this->unknown_1750.push_back(item);
      }
    }
  }

  for (auto it = this->unknown_1750.begin(); it != this->unknown_1750.end();) {
    const int itemId = std::atoi(it->itemId.c_str());
    auto match = this->unknown_1748.begin();
    for (; match != this->unknown_1748.end(); ++match) {
      if (std::atoi(match->itemId.c_str()) == itemId) {
        break;
      }
    }

    if (match == this->unknown_1748.end()) {
      it = this->unknown_1750.erase(it);
      continue;
    }

    const std::uint32_t value = match->unknown_0014[0];
    if (value == 0xFFFFFFFF || g_session.HasItem61B8(it->unknown_0000[1]) == 1) {
      it = this->unknown_1750.erase(it);
      continue;
    }

    it->unknown_0014[0] = value;
    it->unknown_0014[1] = value;
    ++it;
  }
}

std::uint32_t __fastcall ItemCardColor(const ITEM_INFO* item);

// OFFSET: 0x00DDD2C0
// FUNCTION: fn_23383
std::uint32_t CSession::GetItemNameColor(const ITEM_INFO& item) const {
  if (!item.identified) {
    return 0xFFAAAAAA;
  }
  const std::uint32_t color = ItemCardColor(&item);
  if (color == 0) {
    return item.unknown_0064 == 2 ? 0xFFFFFF00 : 0xFFFFFFFF;
  }
  return color;
}

// OFFSET: 0x00E30720
// FUNCTION: fn_24166
int CSession::CountItems2B50() const {
  std::vector<std::uint32_t> itemIds;
  int count = 0;
  for (int i = 0; i < 10; ++i) {
    const std::uint32_t itemId = this->unknown_2B50[i].unknown_0000[1];
    if (itemId != 0 && std::find(itemIds.begin(), itemIds.end(), itemId) == itemIds.end()) {
      itemIds.push_back(itemId);
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00E30830
// FUNCTION: fn_24167
int CSession::CountItems17F0() const {
  std::vector<std::uint32_t> itemIds;
  int count = 0;
  for (int i = 0; i < 10; ++i) {
    const std::uint32_t itemId = this->unknown_17F0[i].unknown_0000[1];
    if (itemId != 0 && std::find(itemIds.begin(), itemIds.end(), itemId) == itemIds.end()) {
      itemIds.push_back(itemId);
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00DF9500
// FUNCTION: fn_23518
void CSession::InvalidateWindowFrames() {
  reinterpret_cast<WindowOrdering&>(g_windowMgr).InvalidateFrames(g_windowMgr.unknown_04FC);
}

// OFFSET: 0x00DE3670
// FUNCTION: fn_23480
const char* CSession::GetMonsterW3dName(int job) const {
  return "캐릭터\\mdragon.w3d";
}

// OFFSET: 0x00DE38E0
// FUNCTION: fn_23483
unsigned char CSession::GetShopType(int id) {
  auto it = this->m_shops.find(id);
  if (it == this->m_shops.end()) {
    return 0;
  }

  return it->second.unknown_0000;
}

// OFFSET: 0x00DE3930
// FUNCTION: fn_23484
std::pair<int, int> CSession::GetShopEntry(unsigned int index, int key) {
  auto name = this->m_nameToId.find(std::string(this->m_navigationPlayerMap));
  if (name != this->m_nameToId.end()) {
    auto shop = this->m_shops.find(name->second);
    if (shop != this->m_shops.end() && index < shop->second.unknown_0004.size()) {
      std::map<int, std::pair<int, int>>& entries = shop->second.unknown_0004[index];
      auto entry = entries.find(key);
      if (entry != entries.end()) {
        return entry->second;
      }
    }
  }

  return std::pair<int, int>(0, 0);
}

// OFFSET: 0x00DE3CE0
// FUNCTION: fn_23487
bool CSession::HasControlCharacter(std::string text) {
  for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
    const char c = *it;
    if (c >= 0) {
      const unsigned char value = static_cast<unsigned char>(c);
      if (value <= 0x1F || value == 0x7F || value == 0x81 || value == 0x8D || value == 0x8F || value == 0x90 ||
          value == 0x9D || value == 0xAD) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00DE3D90

// OFFSET: 0x00DE9430
// FUNCTION: fn_23501
void CSession::LoadMiniPartyInfo() {
  CFile file;
  if (!file.Open("savedata\\MiniPartyInfo.lua", 0, false, false)) {
    return;
  }

  const int size = file.GetSize();
  char* data = new char[size + 1];
  file.Read(data, size);
  data[size] = '\0';
  file.Close();

  int state = 1;
  char name[24] = {0};
  char token[24];
  SessionRecord0734 record;
  for (int i = 0; i < size; ++i) {
    record.unknown_0000 = 1;
    switch (data[i]) {
    case ',': {
      if (_mbsstr(reinterpret_cast<const unsigned char*>(name),
            reinterpret_cast<const unsigned char*>(g_session.DecodePlayerName())) != nullptr) {
        if (state == 2) {
          ++i;
          record.id = std::atoi(&data[i]);
          state = 3;
        } else if (state == 3) {
          ++i;
          record.unknown_0008 = std::atoi(&data[i]);
          state = 1;
        }
      }
      break;
    }
    case '{': {
      ++i;
      std::memset(name, 0, sizeof(name));
      std::memset(token, 0, sizeof(token));
      int position = 0;
      while (data[i] != ',') {
        token[position] += data[i];
        ++position;
        ++i;
      }
      strcpy_s(name, sizeof(name), token);
      if (state == 1) {
        ++i;
        record.unknown_0000 = std::atoi(&data[i]);
        state = 2;
      }
      break;
    }
    case '}':
      this->unknown_0734.push_back(record);
      break;
    case '\n':
    case '\r':
      break;
    }
  }

  std::free(data);
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// OFFSET: 0x00DFD060
// FUNCTION: fn_23603
void CSession::LoadExternalSettings() {
  if (g_serviceMode == 1) {
    this->m_hotkeyScript->Load(std::strstr(g_launchServerName, "QmSakray") != nullptr ? "Lua Files\\service_korea\\ExternalSettings_kr_sak_qm" : "Lua Files\\service_korea\\ExternalSettings_kr_sak", true, false);
  } else if (g_serviceMode == 2) {
    this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr_indoor", true, false);
  } else if (g_serviceMode == 0 && std::strstr(g_launchServerName, "Dev") != nullptr) {
    this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr_qm", true, false);
  } else {
    this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr", true, false);
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel", &*reinterpret_cast<int*>(this->unknown_7EE8));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel3rd", &this->m_maxLevel);
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelExtend2", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x00]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelUpperJob", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x04]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelHomun", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x08]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelNovice", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x0C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelSuperNovice", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x10]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelBase", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x14]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel2nd", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x18]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel3rd", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x1C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelExtend2", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x20]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelUpperJob", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x24]));

  if (*reinterpret_cast<int*>(this->unknown_7EE8) == -1 ||
      this->m_maxLevel == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x00]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x04]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x08]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x0C]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x10]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x14]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x18]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x1C]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x20]) == -1 ||
      *reinterpret_cast<int*>(&this->unknown_7EF0[0x24]) == -1) {
    const std::uint8_t source[] = {
      0x1E, 0x9B, 0xE7, 0x02, 0xF7, 0x9D, 0x18,
      0x1D, 0x2F, 0x8C, 0xE7, 0x03, 0xD6, 0xD8,
      0x29, 0x11, 0x32, 0x87, 0xF2, 0x1E, 0xA5,
      0xEB, 0x3C, 0x09, 0x2E, 0x8C, 0x95
    };
    thread_local EncryptedName<sizeof(source), 0x655DBD856D95E95BULL> name(source);
    ShowErrorDialog(nullptr, name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel4th", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x74]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel4th", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x78]));

  if (*reinterpret_cast<int*>(&this->unknown_7EF0[0x74]) == -1 || *reinterpret_cast<int*>(&this->unknown_7EF0[0x78]) == -1) {
    const std::uint8_t source[] = {
      0xC2, 0x6B, 0x53, 0x54, 0x97, 0x4F, 0xEC,
      0xE5, 0xF3, 0x7C, 0x53, 0x55, 0xB6, 0x0A,
      0xDD, 0xE9, 0xEE, 0x77, 0x46, 0x48, 0xC5,
      0x39, 0xC8, 0xF1, 0xF2, 0x7C, 0x21
    };
    thread_local EncryptedName<sizeof(source), 0x9DA96FE53B211987ULL> name(source);
    ShowErrorDialog(nullptr, name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelDoram", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x28]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelDoram", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x2C]));

  if (*reinterpret_cast<int*>(&this->unknown_7EF0[0x2C]) == -1) {
    const std::uint8_t source[] = {
      0x84, 0x29, 0xA1, 0x34, 0x8D, 0xA7, 0xC6,
      0xDF, 0xB5, 0x3E, 0xA1, 0x35, 0xAC, 0xE2,
      0xF7, 0xD3, 0xA8, 0x35, 0xB4, 0x28, 0xDF,
      0xD1, 0xE2, 0xCB, 0xB4, 0x3E, 0xD3
    };
    thread_local EncryptedName<sizeof(source), 0xA78387FF5BD35BC1ULL> name(source);
    ShowErrorDialog(nullptr, name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableBoolValueForC", 21), "ss>b", "MakeableRace", "Doram", &*reinterpret_cast<int*>(this->unknown_7F74));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Default99LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x30]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Default99LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x34]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Baby99LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x38]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Baby99LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x3C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "SecondHigh99LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x40]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "SecondHigh99LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x44]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Homun99LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x48]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Homun99LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x4C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Boss99LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x50]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default150LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x54]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default150LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x58]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default160LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x5C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default160LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x60]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "HomunMaxLvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x64]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "HomunMaxLvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x68]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default185LvAura", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x6C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default185LvAura_sub", &*reinterpret_cast<int*>(&this->unknown_7EF0[0x70]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "MaxLevelEffect4th", &this->m_maxLevelEffect4th);
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "MaxLevelEffect4th_sub", &this->m_maxLevelEffect4thSub);

  if (this->m_maxLevelEffect4th == -1 || this->m_maxLevelEffect4thSub == -1) {
    const std::uint8_t source[] = {
      0x66, 0x23, 0xB5, 0x54, 0xA1, 0x55, 0x94,
      0xF9, 0x57, 0x34, 0xB5, 0x55, 0x80, 0x10,
      0xA5, 0xF5, 0x4A, 0x3F, 0xA0, 0x48, 0xF3,
      0x23, 0xB0, 0xED, 0x56, 0x34, 0xC7
    };
    thread_local EncryptedName<sizeof(source), 0x81D175D33BC75123ULL> name(source);
    ShowErrorDialog(nullptr, name.Text(), 0);
  }
}

// OFFSET: 0x00DF7C40
// FUNCTION: fn_23508
void CSession::ResetCharacterState() {
  *reinterpret_cast<float*>(&this->m_navigationPlayerMap[0x18]) = sqrtf(50.0f);

  const int randomValue = std::rand() % 1024 + 1;

  this->m_zeny = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x04]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x08]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x20]) = 0;
  this->m_maxWeight = 0;
  this->m_weight = 0;

  if (this->unknown_556C != randomValue) {
    this->unknown_556C = randomValue;
    this->RefreshHpSpWindows();
  }

  if (this->unknown_5568 != randomValue) {
    this->unknown_5568 = randomValue;
    this->RefreshHpSpWindows();
  }

  this->m_zeny = randomValue;
  this->m_level = randomValue;
  this->m_jobLevel = randomValue;

  std::memcpy(this->m_playerNameKey, "잠이들었던거야", sizeof("잠이들었던거야"));

  this->unknown_1678 = 0;
  this->unknown_167C = 0;
  this->unknown_1680 = 0;
  this->unknown_166C = 0;
  this->unknown_1670 = 0;
  this->unknown_1674 = 0;

  *reinterpret_cast<int*>(&this->unknown_169C[0x00]) = 2;
  *reinterpret_cast<int*>(&this->unknown_169C[0x04]) = 2;
  *reinterpret_cast<int*>(&this->unknown_169C[0x14]) = 2;
  *reinterpret_cast<int*>(&this->unknown_169C[0x08]) = 2;
  *reinterpret_cast<int*>(&this->unknown_169C[0x0C]) = 2;
  *reinterpret_cast<int*>(&this->unknown_169C[0x10]) = 2;

  *reinterpret_cast<int*>(&this->unknown_162C[0x28]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x30]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x34]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x38]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x28]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x2C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x20]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x3C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x40]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x44]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x4C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x48]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x18]) = 0;
  *reinterpret_cast<int*>(&this->unknown_169C[0x24]) = 0;

  *reinterpret_cast<int*>(&this->unknown_162C[0x04]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x08]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x20]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x0C]) = 999999;
  *reinterpret_cast<int*>(&this->unknown_162C[0x10]) = 0;
  *reinterpret_cast<int*>(&this->unknown_162C[0x14]) = 999999;
  *reinterpret_cast<int*>(&this->unknown_162C[0x18]) = 0;

  this->cartCount = 0;
  this->cartMaxCount = 0;
  this->cartWeight = 0;
  this->cartMaxWeight = 0;
  *reinterpret_cast<int*>(&this->unknown_0F44[0x00]) = 13;
  *reinterpret_cast<int*>(&this->unknown_0F44[0x04]) = 5;
  this->unknown_0F24[0] = 0;
  this->m_score1 = -1;
  this->m_score2 = -1;
  this->unknown_0F30[0] = 0;
  this->unknown_0F4C = 20;
  this->unknown_0F50 = 0;

  this->unknown_16EC = 0;
  this->unknown_54A0 = 0;
  this->unknown_54A4 = 0;
  this->unknown_54A8 = 0;
  this->unknown_1708 = 1;
  this->unknown_5480 = 0;
  this->unknown_5484 = 0;
  this->unknown_5488 = 0;
  this->unknown_548C = 0;
  *reinterpret_cast<int*>(&g_session.unknown_544C[0x00]) = 0;
  *reinterpret_cast<std::uint16_t*>(&this->unknown_555C[0x08]) = 0;
  this->unknown_5468 = 0;
  this->unknown_5478[0] = 1;
  this->unknown_5450 = 0;
  this->unknown_5448 = 0;
  this->unknown_59DC[0] = 1;

  MEMORYSTATUS memoryStatus = {};
  memoryStatus.dwLength = sizeof(memoryStatus);
  GlobalMemoryStatus(&memoryStatus);
  this->unknown_5470 = memoryStatus.dwTotalPhys < 0x9B72EC0 ? 1 : 0;

  std::memset(&this->unknown_07F4[0x40], 0, 0x660);
  this->unknown_5C1C = 0;

  std::map<int, int>& traitStats = reinterpret_cast<std::map<int, int>&>(this->unknown_7FC0);
  traitStats[0xE7] = 0;
  traitStats[0xDB] = 0;
  traitStats[0xDC] = 0;
  traitStats[0xDD] = 0;
  traitStats[0xDE] = 0;
  traitStats[0xDF] = 0;
  traitStats[0xE0] = 0;
  traitStats[0xE1] = 0;
  traitStats[0xE2] = 0;
  traitStats[0xE3] = 0;
  traitStats[0xE4] = 0;
  traitStats[0xE5] = 0;
  traitStats[0xE6] = 0;
}

// OFFSET: 0x00DF88B0
// FUNCTION: fn_23511
void CSession::ResetInventoryItems() {
  this->m_inventoryItems.erase(this->m_inventoryItems.begin(), this->m_inventoryItems.end());
  this->m_pingSamples.erase(this->m_pingSamples.begin(), this->m_pingSamples.end());

  for (int index = 0; index < 10; ++index) {
    this->unknown_17F0[index].Reset();
  }
}

// OFFSET: 0x00DF8940
// FUNCTION: fn_23512
void CSession::ClearItemContainers() {
  this->unknown_1720.erase(this->unknown_1720.begin(), this->unknown_1720.end());
  this->unknown_1730.erase(this->unknown_1730.begin(), this->unknown_1730.end());
  this->unknown_1718.erase(this->unknown_1718.begin(), this->unknown_1718.end());

  this->unknown_5210 = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_exchangeItems[index].quantity = 0;
  }

  this->unknown_5214 = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_itemSlots[index].quantity = 0;
  }

  this->unknown_1738.erase(this->unknown_1738.begin(), this->unknown_1738.end());

  g_session.m_skillBundle.skills.clear();
  this->m_friends.clear();

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode != nullptr) {
    for (std::list<MessengerMember>::iterator it = this->m_partyMembers.begin(); it != this->m_partyMembers.end(); ++it) {
      CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(it->accountId);
      if (actor != nullptr && static_cast<EffectActor*>(actor)->state.Attachments().HasId(160)) {
        actor->SendMsg(nullptr, 109, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
  }

  this->m_partyMembers.clear();
  this->unknown_54C4 = "";
  this->unknown_5464 = 0;
  this->ClearItemQuantities();
}

// OFFSET: 0x00DF3160
// FUNCTION: fn_23503
void CSession::ClearEntryList5BD4() {
  this->unknown_5BD4.clear();
  this->unknown_5BE0 = 0;
  this->unknown_5BE4 = 0;
  this->unknown_5BE8 = -1;
  this->unknown_5BEC = -1;
  this->unknown_5BF0 = 0xFFFFFFFF;
}

// OFFSET: 0x00D8D9E0
