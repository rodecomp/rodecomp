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
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Window/UIBasicInfoWnd.h"
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
// FUNCTION: fn_23624
void CSession::SetUnknown16EC(std::uint32_t value) {
  this->unknown_16EC = value;
  this->InvalidateWindowFrames();

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  auto it = g_activeSkillEffects.begin();
  for (; it != g_activeSkillEffects.end(); ++it) {
    if (it->id == 0x2B7) {
      break;
    }
  }

  if (it == g_activeSkillEffects.end()) {
    if (value) {
      ActiveSkillEffect effect;
      effect.id = 0x2B7;
      g_activeSkillEffects.push_back(effect);
    }
  } else if (!value) {
    g_activeSkillEffects.erase(it);
  }

  if (mode->scene && mode->scene->player && mode->scene->player->unknown_0544) {
    mode->scene->player->unknown_0544->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
  }
}

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

// OFFSET: 0x00DF9490
// FUNCTION: fn_23516
void CSession::RefreshSkillTree() {
  JobInheritance inheritance{*this->m_hotkeyScript};
  inheritance.InitializeSkillTree(this->m_skillBundle, this->m_job);
  if (g_windowMgr.window_02C4) {
    g_windowMgr.window_02C4->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }

  if (g_windowMgr.unknown_01DC) {
    UIBasicInfoWnd* info = reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.unknown_01DC);
    info->SetHeight(info->geometry.height);
  }
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
// FUNCTION: fn_23488
void CSession::sub_DE3D90(char* text) {
  std::string greeting = "(Hi) ";
  greeting += text ? text : "*^_^*";

  for (const MessengerMember& member : this->m_friends) {
    g_windowMgr.unknown_04DC = greeting.c_str();
    static_cast<CMode*>(g_charViewMode)->SendMsg(0xB, reinterpret_cast<int>(member.name.c_str()), 0, 0, 0);
    if (++g_pendingWhisperCount > 40) {
      g_pendingWhisperCount = 40;
    }
  }
}

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
// FUNCTION: fn_22269
void CSession::ClearAchievements() {
  this->unknown_6144.clear();
  this->unknown_6150 = 0;
  this->unknown_6154 = 0;
  this->unknown_6158 = 0;
  this->unknown_6160.clear();
  this->unknown_614C = 0;
  this->unknown_616C.clear();
  this->unknown_615C = 0;
  this->unknown_6178.clear();
}

// OFFSET: 0x00DDC9D0
// FUNCTION: fn_23371
const std::vector<std::string>* CSession::GetFogParameters(const char* rswName, int* found) {
  *found = 0;
  // GLOBAL: gl_24841
  static std::vector<std::string> s_defaultParameters(4);

  std::string name = rswName;
  const std::size_t position = name.rfind("@");
  if (position != std::string::npos && position >= 4) {
    name.erase(position - 4, 3);
  }

  const char* key = name.c_str();
  auto it = std::lower_bound(this->m_fogParameters.begin(), this->m_fogParameters.end(), key,
                             [](const std::pair<std::string, std::vector<std::string>>& entry, const char* value) {
                               return std::strcmp(entry.first.c_str(), value) < 0;
                             });
  if (it != this->m_fogParameters.end() && std::strcmp(it->first.c_str(), name.c_str()) == 0) {
    *found = 1;
    return &it->second;
  }

  return &s_defaultParameters;
}

// OFFSET: 0x00DE3410
// FUNCTION: fn_23479
const std::vector<std::string>* CSession::GetMapViewParameters(const char* rswName, int* found) {
  std::vector<std::pair<std::string, std::vector<std::string>>>& parameters =
    reinterpret_cast<std::vector<std::pair<std::string, std::vector<std::string>>>&>(this->unknown_523C);
  *found = 0;
  // GLOBAL: gl_24993
  static std::vector<std::string> s_defaultParameters(9);

  std::string name = rswName;
  const std::size_t position = name.rfind("@");
  if (position != std::string::npos && position >= 4) {
    name.erase(position - 4, 3);
  }

  const char* key = name.c_str();
  auto it = std::lower_bound(parameters.begin(), parameters.end(), key,
                             [](const std::pair<std::string, std::vector<std::string>>& entry, const char* value) {
                               return std::strcmp(entry.first.c_str(), value) < 0;
                             });
  if (it != parameters.end() && std::strcmp(it->first.c_str(), name.c_str()) == 0) {
    *found = 1;
    return &it->second;
  }

  return &s_defaultParameters;
}

// OFFSET: 0x00DF8670
// FUNCTION: fn_23510
void CSession::InitWeaponHitWaveNames() {
  std::vector<std::string>& weaponWaves = reinterpret_cast<std::vector<std::string>&>(this->unknown_525C[0x18]);
  weaponWaves.clear();
  weaponWaves.resize(105, "_hit_mace.wav");
  for (int weapon = 0; weapon < 105; ++weapon) {
    const char* name = nullptr;
    if (this->m_hotkeyScript->Call("ReqWeaponHitWaveName", "d>s", weapon, &name) && std::strcmp(name, "") != 0) {
      weaponWaves[weapon] = name;
    }
  }
}

// OFFSET: 0x00DF8C40
// FUNCTION: fn_23513
void CSession::InitJobKeyNames() {
  std::vector<const char*>& names = reinterpret_cast<std::vector<const char*>&>(this->unknown_0FC4[0x14]);
  names.resize(JT_2004_JOB_LAST, "NOVICE");
  names[JT_NOVICE] = "NOVICE";
  names[JT_SUPERNOVICE] = "SUPERNOVICE";
  names[JT_PORING_NOVICE] = "PORING_NOVICE";
  names[JT_PORING_SNOVICE] = "PORING_SNOVICE";
  names[JT_GUNSLINGER] = "GUNSLINGER";
  names[JT_REBELLION] = "REBELLION";
  names[JT_NINJA] = "NINJA";
  names[JT_KAGEROU] = "KAGEROU";
  names[JT_OBORO] = "OBORO";
  names[JT_TAEKWON] = "TAEKWON";
  names[JT_STAR] = "STAR";
  names[JT_STAR_EMPEROR] = "STAR_EMPEROR";
  names[JT_LINKER] = "LINKER";
  names[JT_SOUL_REAPER] = "SOUL_REAPER";
  names[JT_STAR2] = "STAR2";
  names[JT_STAR2_EMPEROR] = "STAR2_EMPEROR";
  names[JT_PECO_GUNNER] = "PECO_GUNNER";
  names[JT_PECO_REBELLION] = "PECO_REBELLION";
  names[JT_FROG_NINJA] = "FROG_NINJA";
  names[JT_FROG_KAGEROU] = "FROG_KAGEROU";
  names[JT_FROG_OBORO] = "FROG_OBORO";
  names[JT_PORING_TAEKWON] = "PORING_TAEKWON";
  names[JT_PORING_STAR] = "PORING_STAR";
  names[JT_HAETAE_STAR_EMPEROR] = "HAETAE_STAR_EMPEROR";
  names[JT_FROG_LINKER] = "FROG_LINKER";
  names[JT_HAETAE_SOUL_REAPER] = "HAETAE_SOUL_REAPER";
  names[JT_SWORDMAN] = "SWORDMAN";
  names[JT_KNIGHT] = "KNIGHT";
  names[JT_KNIGHT_H] = "KNIGHT_H";
  names[JT_RUNE_KNIGHT] = "RUNE_KNIGHT";
  names[JT_CRUSADER] = "CRUSADER";
  names[JT_CRUSADER_H] = "CRUSADER_H";
  names[JT_ROYAL_GUARD] = "ROYAL_GUARD";
  names[JT_CHICKEN] = "CHICKEN";
  names[JT_CHICKEN_H] = "CHICKEN_H";
  names[JT_RUNE_CHICKEN] = "RUNE_CHICKEN";
  names[JT_CHICKEN2] = "CHICKEN2";
  names[JT_CHICKEN2_H] = "CHICKEN2_H";
  names[JT_ROYAL_CHICKEN] = "ROYAL_CHICKEN";
  names[JT_PECO_SWORD] = "PECO_SWORD";
  names[JT_LION_KNIGHT] = "LION_KNIGHT";
  names[JT_LION_KNIGHT_H] = "LION_KNIGHT_H";
  names[JT_LION_RUNE_KNIGHT] = "LION_RUNE_KNIGHT";
  names[JT_LION_CRUSADER] = "LION_CRUSADER";
  names[JT_LION_CRUSADER_H] = "LION_CRUSADER_H";
  names[JT_LION_ROYAL_GUARD] = "LION_ROYAL_GUARD";
  names[JT_RUNE_CHICKEN2] = "RUNE_CHICKEN2";
  names[JT_RUNE_CHICKEN3] = "RUNE_CHICKEN3";
  names[JT_RUNE_CHICKEN4] = "RUNE_CHICKEN4";
  names[JT_RUNE_CHICKEN5] = "RUNE_CHICKEN5";
  names[JT_MAGICIAN] = "MAGICIAN";
  names[JT_WIZARD] = "WIZARD";
  names[JT_WIZARD_H] = "WIZARD_H";
  names[JT_WARLOCK] = "WARLOCK";
  names[JT_SAGE] = "SAGE";
  names[JT_SAGE_H] = "SAGE_H";
  names[JT_SORCERER] = "SORCERER";
  names[JT_FOX_MAGICIAN] = "FOX_MAGICIAN";
  names[JT_FOX_WIZ] = "FOX_WIZ";
  names[JT_FOX_HWIZ] = "FOX_HWIZ";
  names[JT_FOX_WARLOCK] = "FOX_WARLOCK";
  names[JT_FOX_SAGE] = "FOX_SAGE";
  names[JT_FOX_PROF] = "FOX_PROF";
  names[JT_FOX_SORCERER] = "FOX_SORCERER";
  names[JT_ACOLYTE] = "ACOLYTE";
  names[JT_PRIEST] = "PRIEST";
  names[JT_PRIEST_H] = "PRIEST_H";
  names[JT_ARCHBISHOP] = "ARCHBISHOP";
  names[JT_MONK] = "MONK";
  names[JT_MONK_H] = "MONK_H";
  names[JT_SURA] = "SURA";
  names[JT_SHEEP_ACO] = "SHEEP_ACO";
  names[JT_SHEEP_PRIEST] = "SHEEP_PRIEST";
  names[JT_SHEEP_HPRIEST] = "SHEEP_HPRIEST";
  names[JT_SHEEP_ARCB] = "SHEEP_ARCB";
  names[JT_SHEEP_MONK] = "SHEEP_MONK";
  names[JT_SHEEP_CHAMP] = "SHEEP_CHAMP";
  names[JT_SHEEP_SURA] = "SHEEP_SURA";
  names[JT_MERCHANT] = "MERCHANT";
  names[JT_BLACKSMITH] = "BLACKSMITH";
  names[JT_BLACKSMITH_H] = "BLACKSMITH_H";
  names[JT_MECHANIC] = "MECHANIC";
  names[JT_ALCHEMIST] = "ALCHEMIST";
  names[JT_ALCHEMIST_H] = "ALCHEMIST_H";
  names[JT_GENETIC] = "GENETIC";
  names[JT_MADOGEAR] = "MADOGEAR";
  names[JT_PIG_MERCHANT] = "PIG_MERCHANT";
  names[JT_PIG_BLACKSMITH] = "PIG_BLACKSMITH";
  names[JT_PIG_WHITESMITH] = "PIG_WHITESMITH";
  names[JT_PIG_MECHANIC] = "PIG_MECHANIC";
  names[JT_PIG_ALCHE] = "PIG_ALCHE";
  names[JT_PIG_CREATOR] = "PIG_CREATOR";
  names[JT_PIG_GENETIC] = "PIG_GENETIC";
  names[JT_THIEF] = "THIEF";
  names[JT_ASSASSIN] = "ASSASSIN";
  names[JT_ASSASSIN_H] = "ASSASSIN_H";
  names[JT_GUILLOTINE_CROSS] = "GUILLOTINE_CROSS";
  names[JT_ROGUE] = "ROGUE";
  names[JT_ROGUE_H] = "ROGUE_H";
  names[JT_SHADOW_CHASER] = "SHADOW_CHASER";
  names[JT_DOG_THIEF] = "DOG_THIEF";
  names[JT_DOG_ASSASSIN] = "DOG_ASSASSIN";
  names[JT_DOG_ASSA_X] = "DOG_ASSA_X";
  names[JT_DOG_G_CROSS] = "DOG_G_CROSS";
  names[JT_DOG_ROGUE] = "DOG_ROGUE";
  names[JT_DOG_STALKER] = "DOG_STALKER";
  names[JT_DOG_CHASER] = "DOG_CHASER";
  names[JT_ARCHER] = "ARCHER";
  names[JT_HUNTER] = "HUNTER";
  names[JT_HUNTER_H] = "HUNTER_H";
  names[JT_RANGER] = "RANGER";
  names[JT_BARD] = "BARD";
  names[JT_BARD_H] = "BARD_H";
  names[JT_MINSTREL] = "MINSTREL";
  names[JT_DANCER] = "DANCER";
  names[JT_DANCER_H] = "DANCER_H";
  names[JT_WANDERER] = "WANDERER";
  names[JT_WOLF_RANGER] = "WOLF_RANGER";
  names[JT_OSTRICH_ARCHER] = "OSTRICH_ARCHER";
  names[JT_OSTRICH_HUNTER] = "OSTRICH_HUNTER";
  names[JT_OSTRICH_SNIPER] = "OSTRICH_SNIPER";
  names[JT_OSTRICH_RANGER] = "OSTRICH_RANGER";
  names[JT_OSTRICH_BARD] = "OSTRICH_BARD";
  names[JT_OSTRICH_CROWN] = "OSTRICH_CROWN";
  names[JT_OSTRICH_MINSTREL] = "OSTRICH_MINSTREL";
  names[JT_OSTRICH_DANCER] = "OSTRICH_DANCER";
  names[JT_OSTRICH_ZIPSI] = "OSTRICH_ZIPSI";
  names[JT_OSTRICH_WANDER] = "OSTRICH_WANDER";
  names[JT_DO_SUMMONER] = "DO_SUMMONER";
  names[JT_DO_CART_SUMMONER] = "DO_CART_SUMMONER";
  names[JT_MARRIED] = "MARRIED";
  names[JT_SANTA] = "SANTA";
  names[JT_SUMMER] = "SUMMER";
  names[JT_HANBOK] = "HANBOK";
  names[JT_OKTOBERFEST] = "OKTOBERFEST";
  names[JT_SUMMER2] = "SUMMER2";
  names[JT_CHICKEN2_B] = "CHICKEN2_B";
  names[309] = "BIOLO";
  names[322] = "BIOLO_RIDING";
  names[303] = "MEISTER";
  names[316] = "MEISTER_RIDING";
  names[329] = "MEISTER_MADOGEAR1";
  names[302] = "DRAGON_KNIGHT";
  names[315] = "DRAGON_KNIGHT_RIDING";
  names[330] = "DRAGON_KNIGHT_CHICKEN";
  names[308] = "IMPERIAL_GUARD";
  names[321] = "IMPERIAL_GUARD_RIDING";
  names[331] = "IMPERIAL_GUARD_CHICKEN";
  names[352] = "SKY_EMPEROR";
  names[366] = "SKY_EMPEROR2";
  names[359] = "SKY_EMPEROR_RIDING";
  names[353] = "SOUL_ASCETIC";
  names[360] = "SOUL_ASCETIC_RIDING";
  names[354] = "SHINKIRO";
  names[361] = "SHINKIRO_RIDING";
  names[355] = "SHIRANUI";
  names[362] = "SHIRANUI_RIDING";
  names[356] = "NIGHT_WATCH";
  names[363] = "NIGHT_WATCH_RIDING";
  names[357] = "HYPER_NOVICE";
  names[364] = "HYPER_NOVICE_RIDING";
  names[358] = "SPIRIT_HANDLER";
  names[365] = "SPIRIT_HANDLER_RIDING";
}

// OFFSET: 0x00E2F860
// FUNCTION: fn_24159
std::vector<unsigned char> SerializeMessengerMembers(const std::list<MessengerMember>& members) {
  std::vector<unsigned char> records;
  for (std::list<MessengerMember>::const_iterator it = members.begin(); it != members.end(); ++it) {
    const std::vector<unsigned char> record = it->Serialize();
    records.insert(records.end(), record.begin(), record.end());
  }
  return records;
}

// OFFSET: 0x00DF3850
// FUNCTION: fn_23507
void CSession::InitCommandTables() {
  this->m_commandIds.clear();
  this->m_commandIds.push_back(std::make_pair("/help", 14));
  this->m_commandIds.push_back(std::make_pair("/h", 14));
  this->m_commandIds.push_back(std::make_pair("/killall", 5));
  this->m_commandIds.push_back(std::make_pair("/wjqthrwk", 6));
  this->m_commandIds.push_back(std::make_pair("/snrn", 6));
  this->m_commandIds.push_back(std::make_pair("/who", 6));
  this->m_commandIds.push_back(std::make_pair("/w", 6));
  this->m_commandIds.push_back(std::make_pair("/dmadkr", 7));
  this->m_commandIds.push_back(std::make_pair("/bgm", 7));
  this->m_commandIds.push_back(std::make_pair("/effect", 8));
  this->m_commandIds.push_back(std::make_pair("/dlvprxm", 8));
  this->m_commandIds.push_back(std::make_pair("/sound", 11));
  this->m_commandIds.push_back(std::make_pair("/tingonly", 195));
  this->m_commandIds.push_back(std::make_pair("/gyrhkdma", 11));
  this->m_commandIds.push_back(std::make_pair("/skip", 10));
  this->m_commandIds.push_back(std::make_pair("/ex", 15));
  this->m_commandIds.push_back(std::make_pair("/inall", 17));
  this->m_commandIds.push_back(std::make_pair("/exall", 16));
  this->m_commandIds.push_back(std::make_pair("/sit", 22));
  this->m_commandIds.push_back(std::make_pair("/dkswrl", 22));
  this->m_commandIds.push_back(std::make_pair("/stand", 22));
  this->m_commandIds.push_back(std::make_pair("/tjrl", 22));
  this->m_commandIds.push_back(std::make_pair("/coxldqkd", 23));
  this->m_commandIds.push_back(std::make_pair("/chat", 23));
  this->m_commandIds.push_back(std::make_pair("/xkfxhl", 27));
  this->m_commandIds.push_back(std::make_pair("/leave", 27));
  this->m_commandIds.push_back(std::make_pair("/stat", 2));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(544), 29));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(545), 30));
  this->m_commandIds.push_back(std::make_pair("/delight", 31));
  this->m_commandIds.push_back(std::make_pair("/ghk", 36));
  this->m_commandIds.push_back(std::make_pair("/fret", 35));
  this->m_commandIds.push_back(std::make_pair("/Wkwmd", 35));
  this->m_commandIds.push_back(std::make_pair("/q", 24));
  this->m_commandIds.push_back(std::make_pair("/rlQma", 31));
  this->m_commandIds.push_back(std::make_pair("/heart", 32));
  this->m_commandIds.push_back(std::make_pair("/gkxm", 32));
  this->m_commandIds.push_back(std::make_pair("/sweat", 33));
  this->m_commandIds.push_back(std::make_pair("/Eka", 33));
  this->m_commandIds.push_back(std::make_pair("/wnajr", 40));
  this->m_commandIds.push_back(std::make_pair("/qh", 41));
  this->m_commandIds.push_back(std::make_pair("/bo", 41));
  this->m_commandIds.push_back(std::make_pair("/rkdnl", 39));
  this->m_commandIds.push_back(std::make_pair("/gawi", 39));
  this->m_commandIds.push_back(std::make_pair("/bawi", 40));
  this->m_commandIds.push_back(std::make_pair("/qkdnl", 40));
  this->m_commandIds.push_back(std::make_pair("/whkvy", 9));
  this->m_commandIds.push_back(std::make_pair("/wkdth", 9));
  this->m_commandIds.push_back(std::make_pair("/where", 9));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(554), 38));
  this->m_commandIds.push_back(std::make_pair("/money", 37));
  this->m_commandIds.push_back(std::make_pair("/ehs", 37));
  this->m_commandIds.push_back(std::make_pair("/anger", 36));
  this->m_commandIds.push_back(std::make_pair("/aha", 34));
  this->m_commandIds.push_back(std::make_pair("/dkgk", 34));
  this->m_commandIds.push_back(std::make_pair("/tip", 42));
  this->m_commandIds.push_back(std::make_pair("/rldjr", 43));
  this->m_commandIds.push_back(std::make_pair("/memo", 43));
  this->m_commandIds.push_back(std::make_pair("/snap", 46));
  this->m_commandIds.push_back(std::make_pair("/tmsoq", 46));
  this->m_commandIds.push_back(std::make_pair("/skillsnap", 45));
  this->m_commandIds.push_back(std::make_pair("/tmzlftmsoq", 45));
  this->m_commandIds.push_back(std::make_pair("/itemsnap", 47));
  this->m_commandIds.push_back(std::make_pair("/dkdlxpatmsoq", 47));
  this->m_commandIds.push_back(std::make_pair("/rkfanfl", 51));
  this->m_commandIds.push_back(std::make_pair("/savechat", 51));
  this->m_commandIds.push_back(std::make_pair("/miss", 53));
  if (g_serviceMode != 3) {
    this->m_commandIds.push_back(std::make_pair("/noshift", 105));
    this->m_commandIds.push_back(std::make_pair("/noctrl", 109));
    this->m_commandIds.push_back(std::make_pair("/ns", 105));
    this->m_commandIds.push_back(std::make_pair("/nc", 109));
  }
  this->m_commandIds.push_back(std::make_pair("/window", 141));
  this->m_commandIds.push_back(std::make_pair("/wi", 141));
  this->m_commandIds.push_back(std::make_pair("/aura", 106));
  this->m_commandIds.push_back(std::make_pair("/aura2", 107));
  this->m_commandIds.push_back(std::make_pair("/doridori", 110));
  this->m_commandIds.push_back(std::make_pair("/bingbing", 111));
  this->m_commandIds.push_back(std::make_pair("/bangbang", 112));
  this->m_commandIds.push_back(std::make_pair("/skillfail", 113));
  this->m_commandIds.push_back(std::make_pair("/sf", 113));
  this->m_commandIds.push_back(std::make_pair("/loginout", 165));
  this->m_commandIds.push_back(std::make_pair("/li", 165));
  this->m_commandIds.push_back(std::make_pair("/shopping", 167));
  this->m_commandIds.push_back(std::make_pair("/sh", 167));
  this->m_commandIds.push_back(std::make_pair("/quickspell", 120));
  this->m_commandIds.push_back(std::make_pair("/q1", 120));
  this->m_commandIds.push_back(std::make_pair("/quickspell2", 121));
  this->m_commandIds.push_back(std::make_pair("/q2", 121));
  this->m_commandIds.push_back(std::make_pair("/q3", 122));
  this->m_commandIds.push_back(std::make_pair("/notalkmsg", 114));
  this->m_commandIds.push_back(std::make_pair("/nm", 114));
  this->m_commandIds.push_back(std::make_pair("/notalkmsg2", 148));
  this->m_commandIds.push_back(std::make_pair("/nm2", 148));
  this->m_commandIds.push_back(std::make_pair("/notrade", 136));
  this->m_commandIds.push_back(std::make_pair("/nt", 136));
  this->m_commandIds.push_back(std::make_pair("/set1", 115));
  this->m_commandIds.push_back(std::make_pair("/set2", 138));
  this->m_commandIds.push_back(std::make_pair("/set3", 166));
  this->m_commandIds.push_back(std::make_pair("/camera", 54));
  this->m_commandIds.push_back(std::make_pair("/tp", 59));
  this->m_commandIds.push_back(std::make_pair("/fog", 60));
  this->m_commandIds.push_back(std::make_pair("/ㅗ", 14));
  this->m_commandIds.push_back(std::make_pair("/채팅방", 23));
  this->m_commandIds.push_back(std::make_pair("/촘ㅅ", 23));
  this->m_commandIds.push_back(std::make_pair("/접속자", 6));
  this->m_commandIds.push_back(std::make_pair("/누구", 6));
  this->m_commandIds.push_back(std::make_pair("/ㅈ", 6));
  this->m_commandIds.push_back(std::make_pair("/좨", 6));
  this->m_commandIds.push_back(std::make_pair("/음악", 7));
  this->m_commandIds.push_back(std::make_pair("/ㅠ흐", 7));
  this->m_commandIds.push_back(std::make_pair("/이펙트", 8));
  this->m_commandIds.push_back(std::make_pair("/효과음", 11));
  this->m_commandIds.push_back(std::make_pair("/나ㅑㅔ", 10));
  this->m_commandIds.push_back(std::make_pair("/차단", 15));
  this->m_commandIds.push_back(std::make_pair("/해제전부", 17));
  this->m_commandIds.push_back(std::make_pair("/차단전부", 16));
  this->m_commandIds.push_back(std::make_pair("/앉기", 22));
  this->m_commandIds.push_back(std::make_pair("/냣", 22));
  this->m_commandIds.push_back(std::make_pair("/서기", 22));
  this->m_commandIds.push_back(std::make_pair("/ㄴㅅ뭉", 22));
  this->m_commandIds.push_back(std::make_pair("/ㅂ", 24));
  this->m_commandIds.push_back(std::make_pair("/탈퇴", 27));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(546), 31));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(547), 32));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(549), 33));
  this->m_commandIds.push_back(std::make_pair("/주먹", 40));
  this->m_commandIds.push_back(std::make_pair("/보", 41));
  this->m_commandIds.push_back(std::make_pair("/가위", 39));
  this->m_commandIds.push_back(std::make_pair("/바위", 40));
  this->m_commandIds.push_back(std::make_pair("/좌표", 9));
  this->m_commandIds.push_back(std::make_pair("/장소", 9));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(553), 37));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(552), 36));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(551), 35));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(550), 34));
  this->m_commandIds.push_back(std::make_pair("/샤ㅔ", 42));
  this->m_commandIds.push_back(std::make_pair("/기억", 43));
  this->m_commandIds.push_back(std::make_pair("/누메", 46));
  this->m_commandIds.push_back(std::make_pair("/스냅", 46));
  this->m_commandIds.push_back(std::make_pair("/나ㅑㅣㅣ누메", 45));
  this->m_commandIds.push_back(std::make_pair("/스킬스냅", 45));
  this->m_commandIds.push_back(std::make_pair("/ㅑㅅ드누메", 47));
  this->m_commandIds.push_back(std::make_pair("/아이템스냅", 47));
  this->m_commandIds.push_back(std::make_pair("/갈무리", 51));
  this->m_commandIds.push_back(std::make_pair("/autosavechat", 104));
  this->m_commandIds.push_back(std::make_pair("/objlight", 65));
  this->m_commandIds.push_back(std::make_pair("/loading", 145));
  this->m_commandIds.push_back(std::make_pair("/minigame", 173));
  this->m_commandIds.push_back(std::make_pair("/battlemode", 135));
  this->m_commandIds.push_back(std::make_pair("/bm", 135));
  this->m_commandIds.push_back(std::make_pair("/battlechat", 186));
  this->m_commandIds.push_back(std::make_pair("/전장", 186));
  this->m_commandIds.push_back(std::make_pair("/자동응답", 153));
  this->m_commandIds.push_back(std::make_pair("/automsg", 153));
  this->m_commandIds.push_back(std::make_pair("/am", 153));
  this->m_commandIds.push_back(std::make_pair("/traceai", 162));
  this->m_commandIds.push_back(std::make_pair("/호문지능", 164));
  this->m_commandIds.push_back(std::make_pair("/hoai", 164));
  this->m_commandIds.push_back(std::make_pair("/용병지능", 174));
  this->m_commandIds.push_back(std::make_pair("/merai", 174));
  this->m_commandIds.push_back(std::make_pair("/상태정보", 201));
  this->m_commandIds.push_back(std::make_pair("/stateinfo", 201));
  this->m_commandIds.push_back(std::make_pair("/monsterhp", 213));
  this->m_commandIds.push_back(std::make_pair("/ㅡㅐㅜㄴㅅㄷ고ㅔ", 213));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(548), 79));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(555), 66));
  this->m_commandIds.push_back(std::make_pair("/ㄳ", 66));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(556), 67));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(557), 68));
  this->m_commandIds.push_back(std::make_pair("/sorry", 68));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(558), 69));
  this->m_commandIds.push_back(std::make_pair("/하하", 69));
  this->m_commandIds.push_back(std::make_pair("/ㅇ", 69));
  this->m_commandIds.push_back(std::make_pair("/smile", 69));
  this->m_commandIds.push_back(std::make_pair("/ㄱ", 31));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(559), 70));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(560), 71));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(561), 72));
  this->m_commandIds.push_back(std::make_pair("/원츄", 72));
  this->m_commandIds.push_back(std::make_pair("/ㅊ", 72));
  this->m_commandIds.push_back(std::make_pair("/emotion", 86));
  this->m_commandIds.push_back(std::make_pair("/lightmap", 119));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(562), 73));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(563), 74));
  this->m_commandIds.push_back(std::make_pair("/ㅎ", 74));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(564), 75));
  this->m_commandIds.push_back(std::make_pair("/o", 75));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(565), 76));
  this->m_commandIds.push_back(std::make_pair("/x", 76));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(566), 77));
  this->m_commandIds.push_back(std::make_pair("/ㅊㅊ", 77));
  this->m_commandIds.push_back(std::make_pair("/help", 77));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(567), 78));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(568), 80));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(569), 81));
  this->m_commandIds.push_back(std::make_pair("/킥", 81));
  this->m_commandIds.push_back(std::make_pair("/ㅋ", 81));
  this->m_commandIds.push_back(std::make_pair("/케케", 81));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(570), 82));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(571), 83));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(572), 84));
  this->m_commandIds.push_back(std::make_pair("/끄덕", 85));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(573), 85));
  this->m_commandIds.push_back(std::make_pair("/이모션", 86));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(772), 123));
  this->m_commandIds.push_back(std::make_pair("/ㅉ", 123));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(773), 124));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(774), 125));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(775), 126));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(776), 127));
  this->m_commandIds.push_back(std::make_pair("/부", 127));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(777), 128));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(778), 129));
  this->m_commandIds.push_back(std::make_pair("/엠", 129));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(779), 130));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(780), 131));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(781), 132));
  this->m_commandIds.push_back(std::make_pair("/ㅈ", 132));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(782), 133));
  this->m_commandIds.push_back(std::make_pair("/ㅊㅋ", 133));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(783), 134));
  this->m_commandIds.push_back(std::make_pair("/피", 134));
  this->m_commandIds.push_back(std::make_pair("/e1", 123));
  this->m_commandIds.push_back(std::make_pair("/e2", 124));
  this->m_commandIds.push_back(std::make_pair("/e3", 125));
  this->m_commandIds.push_back(std::make_pair("/e4", 126));
  this->m_commandIds.push_back(std::make_pair("/e5", 127));
  this->m_commandIds.push_back(std::make_pair("/e6", 128));
  this->m_commandIds.push_back(std::make_pair("/e7", 129));
  this->m_commandIds.push_back(std::make_pair("/e8", 130));
  this->m_commandIds.push_back(std::make_pair("/e9", 131));
  this->m_commandIds.push_back(std::make_pair("/e10", 132));
  this->m_commandIds.push_back(std::make_pair("/e11", 133));
  this->m_commandIds.push_back(std::make_pair("/e12", 134));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(945), 154));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(946), 155));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(947), 156));
  this->m_commandIds.push_back(std::make_pair("/한숨", 156));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(948), 157));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(949), 158));
  this->m_commandIds.push_back(std::make_pair("/시끌시끌", 158));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(950), 159));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(951), 160));
  this->m_commandIds.push_back(std::make_pair("/dice", 160));
  this->m_commandIds.push_back(std::make_pair("/e19", 160));
  this->m_commandIds.push_back(std::make_pair("/e13", 154));
  this->m_commandIds.push_back(std::make_pair("/e14", 155));
  this->m_commandIds.push_back(std::make_pair("/e15", 156));
  this->m_commandIds.push_back(std::make_pair("/e16", 157));
  this->m_commandIds.push_back(std::make_pair("/e17", 158));
  this->m_commandIds.push_back(std::make_pair("/e18", 159));
  this->m_commandIds.push_back(std::make_pair("/otl", 159));
  this->m_commandIds.push_back(std::make_pair("/러브", 171));
  this->m_commandIds.push_back(std::make_pair("/e20", 171));
  this->m_commandIds.push_back(std::make_pair("/e27", 187));
  this->m_commandIds.push_back(std::make_pair("/e28", 188));
  this->m_commandIds.push_back(std::make_pair("/e29", 189));
  this->m_commandIds.push_back(std::make_pair("/e30", 190));
  this->m_commandIds.push_back(std::make_pair("/e31", 191));
  this->m_commandIds.push_back(std::make_pair("/e32", 192));
  this->m_commandIds.push_back(std::make_pair("/e33", 193));
  this->m_commandIds.push_back(std::make_pair("/흠", 187));
  this->m_commandIds.push_back(std::make_pair("/멍", 188));
  this->m_commandIds.push_back(std::make_pair("/웁", 189));
  this->m_commandIds.push_back(std::make_pair("/웩", 190));
  this->m_commandIds.push_back(std::make_pair("/탈력", 191));
  this->m_commandIds.push_back(std::make_pair("/공황", 192));
  this->m_commandIds.push_back(std::make_pair("/위스퍼", 193));
  this->m_commandIds.push_back(std::make_pair("/hum", 187));
  this->m_commandIds.push_back(std::make_pair("/abs", 188));
  this->m_commandIds.push_back(std::make_pair("/oops", 189));
  this->m_commandIds.push_back(std::make_pair("/spit", 190));
  this->m_commandIds.push_back(std::make_pair("/ene", 191));
  this->m_commandIds.push_back(std::make_pair("/panic", 192));
  this->m_commandIds.push_back(std::make_pair("/whisp", 193));
  this->m_commandIds.push_back(std::make_pair("/hw", 163));
  this->m_commandIds.push_back(std::make_pair("/ㅗㅈ", 163));
  this->m_commandIds.push_back(std::make_pair("/blacksmith", 146));
  this->m_commandIds.push_back(std::make_pair("/alchemist", 147));
  this->m_commandIds.push_back(std::make_pair("/taekwon", 152));
  this->m_commandIds.push_back(std::make_pair("/lv", 149));
  this->m_commandIds.push_back(std::make_pair("/sp", 150));
  this->m_commandIds.push_back(std::make_pair("/pk", 161));
  this->m_commandIds.push_back(std::make_pair("/cn", 181));
  this->m_commandIds.push_back(std::make_pair("/초대거부", 183));
  this->m_commandIds.push_back(std::make_pair("/초대수락", 182));
  this->m_commandIds.push_back(std::make_pair("/refuse", 183));
  this->m_commandIds.push_back(std::make_pair("/accept", 182));
  this->m_commandIds.push_back(std::make_pair("/장비공개", 185));
  this->m_commandIds.push_back(std::make_pair("/wkdqlrhdro", 185));
  this->m_commandIds.push_back(std::make_pair("/eqopen", 185));
  this->m_commandIds.push_back(std::make_pair("/show_addressbar", 205));
  this->m_commandIds.push_back(std::make_pair("/buildinfo", 207));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(2861), 223));
  this->m_commandIds.push_back(std::make_pair("/viewpointvalue", 227));
  this->m_commandIds.push_back(std::make_pair("/mable", 240));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(3513), 241));
  this->m_commandIds.push_back(std::make_pair(LookupMessage(3532), 243));
  std::stable_sort(this->m_commandIds.begin(), this->m_commandIds.end(),
                   [](const std::pair<const char*, int>& left, const std::pair<const char*, int>& right) {
                     return std::strcmp(left.first, right.first) < 0;
                   });
  std::vector<std::pair<const char*, int>>& chatCommands =
    reinterpret_cast<std::vector<std::pair<const char*, int>>&>(this->unknown_52E8[0x00]);
  chatCommands.push_back(std::make_pair("/taekwon", 152));
  chatCommands.push_back(std::make_pair("/hi", 151));
  chatCommands.push_back(std::make_pair("/blacksmith", 146));
  chatCommands.push_back(std::make_pair("/alchemist", 147));
  chatCommands.push_back(std::make_pair("/help", 14));
  chatCommands.push_back(std::make_pair("/sit", 22));
  chatCommands.push_back(std::make_pair("/stand", 22));
  chatCommands.push_back(std::make_pair("/q", 24));
  chatCommands.push_back(std::make_pair("/leave", 27));
  chatCommands.push_back(std::make_pair("/where", 9));
  chatCommands.push_back(std::make_pair("/memo", 43));
  chatCommands.push_back(std::make_pair("/chat", 23));
  chatCommands.push_back(std::make_pair("/savechat", 51));
  chatCommands.push_back(std::make_pair("/doridori", 110));
  chatCommands.push_back(std::make_pair("/bangbang", 112));
  chatCommands.push_back(std::make_pair("/bingbing", 111));
  chatCommands.push_back(std::make_pair("/ex", 15));
  chatCommands.push_back(std::make_pair("/inall", 17));
  chatCommands.push_back(std::make_pair("/exall", 16));
  std::vector<std::pair<const char*, int>>& windowCommands =
    reinterpret_cast<std::vector<std::pair<const char*, int>>&>(this->unknown_52E8[0x0C]);
  if (g_serviceType == 12) {
    windowCommands.push_back(std::make_pair("Sala de Chat", 67));
    windowCommands.push_back(std::make_pair("Atributos", 65));
    windowCommands.push_back(std::make_pair("Detalhes", 68));
    windowCommands.push_back(std::make_pair("Configura\xE7\xE3o", 79));
    windowCommands.push_back(std::make_pair("Invent?io", 69));
    windowCommands.push_back(std::make_pair("Carrinho", 87));
    windowCommands.push_back(std::make_pair("Habilidades", 83));
    windowCommands.push_back(std::make_pair("Equipamentos", 81));
    windowCommands.push_back(std::make_pair("Inf. B?icas", 86));
    windowCommands.push_back(std::make_pair("Cl?", 71));
    windowCommands.push_back(std::make_pair("Conf. Grupo", 80));
    windowCommands.push_back(std::make_pair("Conf. Contatos", 73));
    windowCommands.push_back(std::make_pair("Grupo", 90));
    windowCommands.push_back(std::make_pair("Contatos", 72));
    windowCommands.push_back(std::make_pair("Atalhos", 77));
    windowCommands.push_back(std::make_pair("Lista Emoticons", 76));
    windowCommands.push_back(std::make_pair("Bicho de Est.", 74));
  } else {
    windowCommands.push_back(std::make_pair("Chat", 67));
    windowCommands.push_back(std::make_pair("Status", 65));
    windowCommands.push_back(std::make_pair("DetailLevel", 68));
    windowCommands.push_back(std::make_pair("Option", 79));
    windowCommands.push_back(std::make_pair("Items", 69));
    windowCommands.push_back(std::make_pair("Merchant", 87));
    windowCommands.push_back(std::make_pair("Skill", 83));
    windowCommands.push_back(std::make_pair("Equipment", 81));
    windowCommands.push_back(std::make_pair("BasicInfo", 86));
    windowCommands.push_back(std::make_pair("Guild", 71));
    windowCommands.push_back(std::make_pair("PartySetting", 80));
    windowCommands.push_back(std::make_pair("FriendOption", 73));
    windowCommands.push_back(std::make_pair("Messenger", 90));
    windowCommands.push_back(std::make_pair("WhisperList", 72));
    windowCommands.push_back(std::make_pair("Emotion", 77));
    windowCommands.push_back(std::make_pair("EmotionList", 76));
    windowCommands.push_back(std::make_pair("PetInfo", 74));
  }
  std::vector<const char*>& shortcutCommands = reinterpret_cast<std::vector<const char*>&>(this->unknown_52E8[0x18]);
  shortcutCommands.push_back("Alt + C");
  shortcutCommands.push_back("Alt + A");
  shortcutCommands.push_back("Alt + D");
  shortcutCommands.push_back("Alt + O");
  shortcutCommands.push_back("Alt + E");
  shortcutCommands.push_back("Alt + W");
  shortcutCommands.push_back("Alt + S");
  shortcutCommands.push_back("Alt + Q");
  shortcutCommands.push_back("Alt + V");
  shortcutCommands.push_back("Alt + G");
  shortcutCommands.push_back("Alt + P");
  shortcutCommands.push_back("Alt + I");
  shortcutCommands.push_back("Alt + Z");
  shortcutCommands.push_back("Alt + H");
  shortcutCommands.push_back("Alt + M");
  shortcutCommands.push_back("Alt + L");
  shortcutCommands.push_back("Alt + J");
}
