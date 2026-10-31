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
#include "Contents/MapPropertyMgr.h"
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

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  static CEquipmentPropertiesMgr* GetInstance();
  void RefreshWearable(const int& key);
};
}

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
  g_windowMgr.RebuildMiniPartyWindows();
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

// OFFSET: 0x00DFF830
// FUNCTION: fn_23621
void CSession::SetEquipItem2B50(unsigned int index, const ITEM_INFO& item) {
  if (index < 10) {
    this->unknown_2B50[index] = item;
    EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RefreshWearable(reinterpret_cast<const int&>(item.unknown_0000[3]));
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

// OFFSET: 0x00DFF9C0
// FUNCTION: fn_23625
void CSession::SetEquipItem17F0(unsigned int index, const ITEM_INFO& item) {
  if (index < 10) {
    this->unknown_17F0[index] = item;
    EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RefreshWearable(reinterpret_cast<const int&>(item.unknown_0000[3]));
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

// OFFSET: 0x00DE3F20
// FUNCTION: fn_23489
void CSession::Init() {
  char modulePath[MAX_PATH];
  char directory[MAX_PATH];
  std::memset(modulePath, 0, sizeof(modulePath));
  std::memset(directory, 0, sizeof(directory));
  GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
  const char* fileName = std::strrchr(modulePath, '\\');
  std::strncpy(directory, modulePath, std::strlen(modulePath) - std::strlen(fileName + 1));

  const std::uint8_t keySource[] = {
    0xC4, 0x9E, 0x87, 0xA9, 0xA6, 0xF2, 0x9B, 0x4A, 0xCB, 0xB6, 0x93, 0xBC, 0xA7, 0xFA, 0x9D, 0x56,
    0xCB, 0xA3, 0x80, 0xBA, 0xBF, 0xF2, 0x9B, 0x40, 0xFC, 0xBE, 0x8F, 0xB1, 0xB8, 0xFD, 0x8C, 0x2F
  };
  thread_local EncryptedName<sizeof(keySource), 0x2FE993D1DDE1F197ULL> keyName(keySource);
  HKEY key;
  if (RegCreateKeyExA(HKEY_CURRENT_USER, keyName.Text(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS) {
    const std::uint8_t pathSource[] = {0xAF, 0xB6, 0x2A, 0xF9, 0xD4, 0xFB, 0xFF, 0xE7};
    thread_local EncryptedName<sizeof(pathSource), 0xE7978FB5A94DD7FDULL> pathName(pathSource);
    std::string valueName = pathName.Text();
    if (g_serviceMode == 1) {
      const std::uint8_t sakraySource[] = {0x38, 0xD8, 0xAC, 0x7D, 0x06, 0xF6, 0xAD, 0xD6, 0x1F, 0xD1, 0xC7};
      thread_local EncryptedName<sizeof(sakraySource), 0xB7FD8F670FC7B96BULL> sakrayName(sakraySource);
      std::string sakrayValueName = sakrayName.Text();
    }

    RegSetValueExA(key, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(directory), static_cast<DWORD>(std::strlen(directory)));
  }

  this->m_averagePing = 0;
  *reinterpret_cast<int*>(&this->unknown_5490[0x08]) = 0;
  *reinterpret_cast<int*>(this->unknown_546C) = 1;
  this->m_aid = 0;

  SYSTEMTIME systemTime;
  GetSystemTime(&systemTime);
  const int now =
    (((((static_cast<unsigned char>(systemTime.wYear) * 12 +systemTime.wMonth) * 31 + systemTime.wDay) * 24 + systemTime.wHour + 9) * 60 + systemTime.wMinute) * 60 + systemTime.wSecond) * 1000 +
    systemTime.wMilliseconds;
  *reinterpret_cast<int*>(this->unknown_170C) = now - timeGetTime();
  *reinterpret_cast<int*>(this->unknown_0F54) = 0;
  *reinterpret_cast<int*>(&this->unknown_0F64[0x00]) = 0;
  *reinterpret_cast<int*>(&this->unknown_07A4[0x04]) = 0;
  *reinterpret_cast<int*>(&this->unknown_07A4[0x00]) = 0;
  *reinterpret_cast<int*>(&this->unknown_07A4[0x0C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_07A4[0x08]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5454[0x00]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5454[0x04]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5454[0x08]) = 0;
  this->unknown_5460 = 0;
  *reinterpret_cast<int*>(this->unknown_079C) = 0;
  this->m_hotkeyPage = 0;
  *reinterpret_cast<int*>(this->unknown_5554) = 0;
  this->unknown_0000 = 0;
  g_mapPropertyMgr->Clear();
  g_activeSkillEffects.clear();

  *reinterpret_cast<int*>(&this->unknown_0F64[0x04]) = 0;
  *reinterpret_cast<int*>(this->unknown_1014) = 0;
  this->unknown_103C = -1;
  this->unknown_1040 = 1;
  this->unknown_1044 = 100;
  *reinterpret_cast<int*>(this->unknown_1048) = 100;
  this->m_petFriendly = 100;
  this->unknown_1050 = -1;
  this->unknown_1054 = 100;
  this->unknown_15FC[0] = 0;
  *reinterpret_cast<int*>(&this->unknown_15FC[0x04]) = -1;
  std::sprintf(this->unknown_1018, "");

  std::sprintf(reinterpret_cast<char*>(&this->unknown_5570[0x0C]), "");
  int* homunStats = reinterpret_cast<int*>(&this->unknown_5570[0x2C]);
  for (int index = 0; index < 8; ++index) {
    homunStats[index] = 0;
  }
  this->m_homunJob = -1;
  *reinterpret_cast<int*>(this->unknown_55C0) = 1;
  this->m_homunHp = 0;
  *reinterpret_cast<int*>(this->unknown_55C8) = 999999;
  this->m_homunSp = 0;
  this->unknown_55D0 = 999999;
  this->m_homunExp = 0;
  this->m_homunMaxExp = 999999;
  this->unknown_55F0 = 100;
  this->unknown_55F4 = 100;
  this->unknown_55D4 = 100;
  this->unknown_55F8 = 0;
  this->unknown_5614 = 0;
  this->m_traceAi = 0;
  this->m_homunActionState = 0;
  *reinterpret_cast<int*>(&this->unknown_5620[0x00]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5620[0x04]) = 0;
  this->unknown_5608 = 0;
  this->unknown_560C = 0;
  this->unknown_5610 = 0;
  this->unknown_55FC = 0;
  this->unknown_5600 = 0;
  this->unknown_5604 = 0;

  this->m_mercenaryClass = -1;
  std::sprintf(this->m_mercenaryName, "용병");
  for (int index = 0; index < 8; ++index) {
    this->m_mercenaryStats[index] = 0;
  }
  this->m_mercenaryLevel = 1;
  this->m_mercenaryHp = 0;
  this->m_mercenarySp = 0;
  this->m_mercenaryMaxHp = 999999;
  this->m_mercenaryMaxSp = 999999;
  this->m_mercenaryLoyalty = 0;
  this->m_mercenarySummons = 0;
  this->m_mercenaryKills = 0;
  *reinterpret_cast<int*>(this->unknown_5690) = 0;
  this->m_mercenaryExpiration = 0;
  this->m_mercenaryActionState = 0;
  this->unknown_56B8[0] = 0;

  *reinterpret_cast<DWORD*>(&this->unknown_5490[0x04]) = GetTickCount();
  *reinterpret_cast<int*>(&this->unknown_5913[0x01]) = 0;
  std::sprintf(reinterpret_cast<char*>(&this->unknown_5913[0x05]), "");
  reinterpret_cast<bool&>(this->unknown_5938) = false;
  this->unknown_593C = 0;
  this->unknown_5940 = -1;
  this->unknown_5944 = -1;
  this->unknown_5950 = "";
  this->unknown_5968 = 0;
  this->unknown_596C = 0;
  this->unknown_5970 = 0;
  this->unknown_5948 = 0;
  *reinterpret_cast<int*>(this->unknown_5974) = 0;
  this->m_simplifiedSkillList = 0;
  this->m_showSkillDescription = 1;
  this->unknown_59BC = 0;
  this->unknown_59C0 = 0;
  this->unknown_59C4 = 0;
  this->unknown_59C8 = 0;
  this->unknown_59CC = 0;
  this->unknown_59D0 = 0;
  this->unknown_59D4 = 0;
  this->m_noSubChatWnds.clear();
  this->m_subChatWnds.clear();
  this->m_hotkeyScript = nullptr;
  this->unknown_5BC0 = -1;
  this->LoadProhibitionLog();

  this->unknown_5BD4.clear();
  this->unknown_5BE0 = 0;
  this->unknown_5BE4 = 0;
  this->unknown_5BE8 = -1;
  this->unknown_5BEC = -1;
  this->unknown_5BF0 = 0xFFFFFFFF;
  this->ClearPairValues(1, 1, 1);

  this->m_lockItemDrop = false;
  for (std::list<ITEM_INFO>& items : this->unknown_1784) {
    items.clear();
  }

  this->unknown_5C1C = 0;
  *reinterpret_cast<int*>(&this->unknown_5C15[0x03]) = 0;
  this->m_skipPriceWarning = true;
  *reinterpret_cast<int*>(&this->unknown_5C28[0x08]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5C28[0x0C]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5C28[0x10]) = 0;
  this->unknown_06FC = 0;
  this->unknown_06A8 = 0;
  for (ITEM_INFO& item : this->unknown_5C68) {
    item.Reset();
  }

  this->unknown_6140 = 0;
  *reinterpret_cast<int*>(&this->unknown_5C02[0x02]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5C02[0x06]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5C02[0x0A]) = 0;
  *reinterpret_cast<int*>(&this->unknown_5C02[0x0E]) = 1;
  this->m_muteSellSound = true;
  this->ClearAchievements();
  this->unknown_6184 = 0;
  this->m_bodyCostume = 0;
  this->m_lastIntervalTime = timeGetTime();

  this->unknown_61A4 = 0;
  *reinterpret_cast<int*>(this->unknown_61A8_0000) = 0;
  this->unknown_1010 = 0xFFFFFFFF;
  *reinterpret_cast<int*>(&this->unknown_5570[0x08]) = -1;
  *reinterpret_cast<int*>(&this->unknown_5620[0x08]) = -1;
  this->m_guildMemberListSort = true;
  this->m_blockPrivateTab = false;
  this->unknown_7ECC[0] = 0;
  this->m_itemDefaultSort = false;
  this->m_itemAscendingOrder = false;
  this->m_itemDescendingOrder = false;
  this->m_showMenuIcon = true;
  this->m_homunculusAutoFeed = false;
  *reinterpret_cast<int*>(&this->unknown_7F7D[0x03]) = -1;
  *reinterpret_cast<int*>(&this->unknown_7F7D[0x07]) = 50;
  this->unknown_7F7D[0x0B] = 0;
  this->m_showRankingSchedule = -1;
  *reinterpret_cast<int*>(this->unknown_8128) = 0;
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

template <>
const char* EncryptedName<14, 0x072D2D872D259D5BULL>::Text();
template <>
const char* EncryptedName<18, 0x9BE3EF737387AD79ULL>::Text();
template <>
const char* EncryptedName<14, 0xB96B77B7255F8569ULL>::Text();
template <>
const char* EncryptedName<14, 0x11CB35636377DD07ULL>::Text();
template <>
const char* EncryptedName<10, 0x4755C1355575FD37ULL>::Text();
template <>
const char* EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>::Text();
template <>
const char* EncryptedName<10, 0x79DF094DD9616575ULL>::Text();
template <>
const char* EncryptedName<18, 0x038535BB092533B7ULL>::Text();
template <>
const char* EncryptedName<10, 0xFB2BC75769E777D3ULL>::Text();
template <>
const char* EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>::Text();
template <>
const char* EncryptedName<10, 0x9B9D51AD3515F779ULL>::Text();
template <>
const char* EncryptedName<14, 0xB5F3F779237315F3ULL>::Text();
template <>
const char* EncryptedName<10, 0x67DD5BAB1919D9D1ULL>::Text();
template <>
const char* EncryptedName<14, 0x91493751EF33B5FDULL>::Text();
template <>
const char* EncryptedName<10, 0x6BC977B3B5993503ULL>::Text();
template <>
const char* EncryptedName<10, 0xB7A76BF9CD41F99FULL>::Text();
template <>
const char* EncryptedName<10, 0x732B390D63FD47D7ULL>::Text();
template <>
const char* EncryptedName<14, 0x1FDB81175D1557E3ULL>::Text();
template <>
const char* EncryptedName<10, 0xE3FB23537D2785BFULL>::Text();
template <>
const char* EncryptedName<14, 0x8BBF792B47E72173ULL>::Text();
template <>
const char* EncryptedName<10, 0xB3CF9BEB7963DB89ULL>::Text();
template <>
const char* EncryptedName<14, 0x71A509DB43792991ULL>::Text();
template <>
const char* EncryptedName<10, 0xDD69EDFFA18F537DULL>::Text();
template <>
const char* EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>::Text();
template <>
const char* EncryptedName<10, 0xEB3D4FAB0FC183DBULL>::Text();
template <>
const char* EncryptedName<18, 0x8F3F2FA959977F9FULL>::Text();
template <>
const char* EncryptedName<14, 0xE51B29334D353B81ULL>::Text();
template <>
const char* EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>::Text();
template <>
const char* EncryptedName<10, 0x7FE563BD3FB7F925ULL>::Text();
template <>
const char* EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>::Text();
template <>
const char* EncryptedName<28, 0x7DB1050779034DE7ULL>::Text();
template <>
const char* EncryptedName<22, 0x01E7898D9D235DDFULL>::Text();
template <>
const char* EncryptedName<10, 0x0DF5B33D9F21EB79ULL>::Text();
template <>
const char* EncryptedName<14, 0x9DEFC105A1EB2F71ULL>::Text();
template <>
const char* EncryptedName<10, 0x6961FF056999A303ULL>::Text();
template <>
const char* EncryptedName<18, 0xBD3F430159B551FDULL>::Text();
template <>
const char* EncryptedName<10, 0xC7FB6DF18577D34BULL>::Text();
template <>
const char* EncryptedName<10, 0x5B832361BDA77197ULL>::Text();
template <>
const char* EncryptedName<34, 0x2B477F9B5B39B301ULL>::Text();
template <>
const char* EncryptedName<14, 0x2B8725D7E95325F5ULL>::Text();
template <>
const char* EncryptedName<10, 0x899D11BB83695B0DULL>::Text();
template <>
const char* EncryptedName<14, 0xBDADDD87C5A50B6BULL>::Text();
template <>
const char* EncryptedName<10, 0x7989830DE73B9DBDULL>::Text();
template <>
const char* EncryptedName<10, 0xA7471985F93F9BDFULL>::Text();
template <>
const char* EncryptedName<10, 0xF535E561137F138DULL>::Text();
template <>
const char* EncryptedName<10, 0xD3E5516F89F161D1ULL>::Text();
template <>
const char* EncryptedName<18, 0x45E3F103CBB5F157ULL>::Text();
template <>
const char* EncryptedName<14, 0x07112F85A39F4FABULL>::Text();
template <>
const char* EncryptedName<14, 0x2D33791F67AD47DFULL>::Text();
template <>
const char* EncryptedName<10, 0x936F3D2FFDFD87F5ULL>::Text();
template <>
const char* EncryptedName<14, 0x2F2701A7E56D3D89ULL>::Text();
template <>
const char* EncryptedName<28, 0x6B3301E1A3079125ULL>::Text();
template <>
const char* EncryptedName<22, 0x71B7691737C5A1AFULL>::Text();
template <>
const char* EncryptedName<10, 0xC39FB559A17D3963ULL>::Text();
template <>
const char* EncryptedName<14, 0xFF7D83CB791339D9ULL>::Text();
template <>
const char* EncryptedName<10, 0xED5143E7C1B39315ULL>::Text();
template <>
const char* EncryptedName<18, 0x393D916561BDC391ULL>::Text();
template <>
const char* EncryptedName<10, 0x31956551B311FD95ULL>::Text();
template <>
const char* EncryptedName<10, 0x8741152D8FB34B4FULL>::Text();
template <>
const char* EncryptedName<34, 0x955D4DF94DE9BB07ULL>::Text();
template <>
const char* EncryptedName<14, 0xC399B7091589B50BULL>::Text();
template <>
const char* EncryptedName<18, 0xB739073FE921AD6FULL>::Text();
template <>
const char* EncryptedName<18, 0x17FFC98B398FAF61ULL>::Text();
template <>
const char* EncryptedName<18, 0x9B5B959B954D9333ULL>::Text();
template <>
const char* EncryptedName<14, 0xAFA5617F178B6185ULL>::Text();
template <>
const char* EncryptedName<14, 0x078143534999254DULL>::Text();
template <>
const char* EncryptedName<10, 0x3F5315DFE32B4FAFULL>::Text();
template <>
const char* EncryptedName<14, 0xE50B2935073DD3E9ULL>::Text();
template <>
const char* EncryptedName<10, 0x7D750DADBDE9C3F1ULL>::Text();
template <>
const char* EncryptedName<10, 0x33832D4D3FDFBDBFULL>::Text();
template <>
const char* EncryptedName<28, 0x1BF915E1F37DFDFDULL>::Text();
template <>
const char* EncryptedName<28, 0x01FF9313695DD9DFULL>::Text();
template <>
const char* EncryptedName<28, 0x6DED3DD1CF75AD9BULL>::Text();
template <>
const char* EncryptedName<28, 0x49C3759B77DBD3C1ULL>::Text();
template <>
const char* EncryptedName<28, 0xD1538513DF975701ULL>::Text();
template <>
const char* EncryptedName<28, 0x91E147EBBD47C39FULL>::Text();
template <>
const char* EncryptedName<28, 0x4F1791FB1503C713ULL>::Text();
template <>
const char* EncryptedName<28, 0xA1A55B4D1597EDD3ULL>::Text();
template <>
const char* EncryptedName<28, 0x0753A7E1EFA9E139ULL>::Text();
template <>
const char* EncryptedName<28, 0x7FC1015DFFCF6D19ULL>::Text();
template <>
const char* EncryptedName<28, 0xCB3FBD4183DB8323ULL>::Text();
template <>
const char* EncryptedName<18, 0x7FCD77F9C7CBD14DULL>::Text();
template <>
const char* EncryptedName<18, 0x5B8F49AF09390D1BULL>::Text();
template <>
const char* EncryptedName<14, 0x3B915119EFD523EFULL>::Text();
template <>
const char* EncryptedName<14, 0x474DC581D5E98357ULL>::Text();
template <>
const char* EncryptedName<14, 0x3F557BE333F95F6DULL>::Text();
template <>
const char* EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>::Text();
template <>
const char* EncryptedName<18, 0x0BEB63A5C905B3A7ULL>::Text();
template <>
const char* EncryptedName<18, 0x078333BB9725B7C9ULL>::Text();
template <>
const char* EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>::Text();
template <>
const char* EncryptedName<10, 0x69FD1D9733D3CFC1ULL>::Text();
template <>
const char* EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>::Text();
template <>
const char* EncryptedName<22, 0x8B65CD61012B5931ULL>::Text();
template <>
const char* EncryptedName<22, 0xF7530137154D8FE5ULL>::Text();
template <>
const char* EncryptedName<22, 0x61FF87ED7BF39BE1ULL>::Text();
template <>
const char* EncryptedName<14, 0x7D993F650D2785FFULL>::Text();
template <>
const char* EncryptedName<14, 0x2D394DBD0F332733ULL>::Text();
template <>
const char* EncryptedName<22, 0x151F77EBEBBB2147ULL>::Text();
template <>
const char* EncryptedName<22, 0xC7912587FFF31B71ULL>::Text();
template <>
const char* EncryptedName<34, 0x69D345DB950F336DULL>::Text();
template <>
const char* EncryptedName<34, 0x9B17C9EFDBB39F2FULL>::Text();
template <>
const char* EncryptedName<34, 0x2D3B15774149D5DDULL>::Text();
template <>
const char* EncryptedName<10, 0xD98F03A52D35636DULL>::Text();
template <>
const char* EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>::Text();
template <>
const char* EncryptedName<14, 0xD3755DD95F47A909ULL>::Text();
template <>
const char* EncryptedName<14, 0x31D7C1EDA345E36DULL>::Text();
template <>
const char* EncryptedName<10, 0x3FAF85D995034FD7ULL>::Text();
template <>
const char* EncryptedName<10, 0x490FFB43A12F0DB9ULL>::Text();
template <>
const char* EncryptedName<18, 0x91651D81F559F971ULL>::Text();
template <>
const char* EncryptedName<18, 0x73D1674B61DF7B0BULL>::Text();
template <>
const char* EncryptedName<10, 0x2FF3C5036D070127ULL>::Text();
template <>
const char* EncryptedName<10, 0x17359BF953C367B3ULL>::Text();
template <>
const char* EncryptedName<10, 0xC52FB5FBB94FF917ULL>::Text();
template <>
const char* EncryptedName<10, 0x93E7A3A1F587CDC5ULL>::Text();
template <>
const char* EncryptedName<10, 0x59278F63D79F3DA1ULL>::Text();
template <>
const char* EncryptedName<14, 0x812BE72BCD5707B7ULL>::Text();
template <>
const char* EncryptedName<10, 0x3375B1C7DBB5FD55ULL>::Text();
template <>
const char* EncryptedName<18, 0x1D6D75EDE7CD311FULL>::Text();
template <>
const char* EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>::Text();
template <>
const char* EncryptedName<14, 0x7DBBD74FA36F3BCBULL>::Text();
template <>
const char* EncryptedName<22, 0x4917EB6D71B701ABULL>::Text();
template <>
const char* EncryptedName<14, 0xFD4F85BBA7E37D4FULL>::Text();
template <>
const char* EncryptedName<10, 0x51C57BB9F1DD9B0FULL>::Text();
template <>
const char* EncryptedName<10, 0xC733B7A333F9F571ULL>::Text();
template <>
const char* EncryptedName<10, 0x7D61612937C1592DULL>::Text();
template <>
const char* EncryptedName<18, 0x8F4B518D839D1581ULL>::Text();
template <>
const char* EncryptedName<10, 0xCBD7DDAFDF87F549ULL>::Text();
template <>
const char* EncryptedName<14, 0x39E7F13755252F8FULL>::Text();
template <>
const char* EncryptedName<14, 0x3D075BD5256F9B6FULL>::Text();
template <>
const char* EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>::Text();
template <>
const char* EncryptedName<14, 0x03014749B127914FULL>::Text();
template <>
const char* EncryptedName<18, 0x251761D7370BA5C3ULL>::Text();
template <>
const char* EncryptedName<14, 0x2B034DCBB1812785ULL>::Text();
template <>
const char* EncryptedName<14, 0x85FB93CF2F494179ULL>::Text();
template <>
const char* EncryptedName<14, 0x894FEDABAFCF45E3ULL>::Text();
template <>
const char* EncryptedName<14, 0x8B03E17DA967E943ULL>::Text();
template <>
const char* EncryptedName<10, 0x190B8705C141D1CFULL>::Text();
template <>
const char* EncryptedName<10, 0xC98BB32777F94371ULL>::Text();
template <>
const char* EncryptedName<10, 0xA5F51F3989DBB99FULL>::Text();
template <>
const char* EncryptedName<10, 0xB3D5C99B2B05FFEFULL>::Text();
template <>
const char* EncryptedName<10, 0xC71BB3CDEB499DD1ULL>::Text();
template <>
const char* EncryptedName<22, 0x3939079FF3395587ULL>::Text();
template <>
const char* EncryptedName<22, 0xCF391B0F8B9D95DBULL>::Text();
template <>
const char* EncryptedName<10, 0x6967633B9D89092BULL>::Text();
template <>
const char* EncryptedName<10, 0xAFCD4F69FBDFE157ULL>::Text();
template <>
const char* EncryptedName<22, 0x85D9CFCF0FBB71ABULL>::Text();
template <>
const char* EncryptedName<22, 0x795383DD11AF7DA7ULL>::Text();
template <>
const char* EncryptedName<22, 0xD9FF511B2BB921E7ULL>::Text();
template <>
const char* EncryptedName<16, 0x978B51F50DC5FD25ULL>::Text();
template <>
const char* EncryptedName<12, 0xC757D96143298511ULL>::Text();
template <>
const char* EncryptedName<16, 0xE1F181F923B759E7ULL>::Text();
template <>
const char* EncryptedName<12, 0x37D76D8F495113C3ULL>::Text();
template <>
const char* EncryptedName<20, 0x7BAD7791F50F5D8DULL>::Text();
template <>
const char* EncryptedName<18, 0x2F1337956DC3CFD5ULL>::Text();
template <>
const char* EncryptedName<23, 0xC91F9D493F75FDA3ULL>::Text();
template <>
const char* EncryptedName<18, 0x1F01A13D8D915327ULL>::Text();
template <>
const char* EncryptedName<23, 0xF33D3D3B0BD9C53DULL>::Text();
template <>
const char* EncryptedName<10, 0xC74BB3BB110FB10FULL>::Text();
template <>
const char* EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>::Text();
template <>
const char* EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>::Text();
template <>
const char* EncryptedName<12, 0x8773593DB181D573ULL>::Text();
template <>
const char* EncryptedName<16, 0xFBDBDB13A77191BFULL>::Text();
template <>
const char* EncryptedName<12, 0x19BBC5910555FB97ULL>::Text();
template <>
const char* EncryptedName<18, 0xBBD983C7FD611FE3ULL>::Text();
template <>
const char* EncryptedName<18, 0xA1ED494BF319772DULL>::Text();
template <>
const char* EncryptedName<18, 0x798B394FF5FF434DULL>::Text();
template <>
const char* EncryptedName<18, 0x410DD52D6731BFFDULL>::Text();
template <>
const char* EncryptedName<18, 0x07B733370F612D0FULL>::Text();
template <>
const char* EncryptedName<14, 0x113DC179F1C795C9ULL>::Text();
template <>
const char* EncryptedName<14, 0x31F5C32DD5C53779ULL>::Text();
template <>
const char* EncryptedName<10, 0x2F9B717365C1250FULL>::Text();
template <>
const char* EncryptedName<10, 0x4FE54379493FB5D5ULL>::Text();
template <>
const char* EncryptedName<20, 0x656FAB972B3D21BBULL>::Text();
template <>
const char* EncryptedName<20, 0x33CD2BDD5B3927EDULL>::Text();
template <>
const char* EncryptedName<18, 0xD97BB73901ED99BFULL>::Text();
template <>
const char* EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>::Text();
template <>
const char* EncryptedName<18, 0xE5FDE3151BED831FULL>::Text();
template <>
const char* EncryptedName<14, 0x8971CB79C18D8B27ULL>::Text();
template <>
const char* EncryptedName<18, 0x8DC5D10DAB1585BDULL>::Text();
template <>
const char* EncryptedName<18, 0x21156F4141E35DD9ULL>::Text();
template <>
const char* EncryptedName<18, 0x51EB99CBCB075F35ULL>::Text();
template <>
const char* EncryptedName<14, 0x1B3B756FE399E1D9ULL>::Text();
template <>
const char* EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>::Text();
template <>
const char* EncryptedName<14, 0x451FD72783630709ULL>::Text();
template <>
const char* EncryptedName<14, 0x73C32959FDFFA51BULL>::Text();
template <>
const char* EncryptedName<14, 0xA15B555BE9CD2B81ULL>::Text();
template <>
const char* EncryptedName<14, 0xD9115915B1919533ULL>::Text();
template <>
const char* EncryptedName<14, 0xB3F1A10745C1F105ULL>::Text();
template <>
const char* EncryptedName<18, 0x99157F159BB99BD9ULL>::Text();
template <>
const char* EncryptedName<23, 0x7959FFBF652D07BFULL>::Text();
template <>
const char* EncryptedName<25, 0x01C9FBA7819FFBD7ULL>::Text();
template <>
const char* EncryptedName<22, 0xABED6971354375C3ULL>::Text();
template <>
const char* EncryptedName<29, 0x6BA557130BDFE997ULL>::Text();
template <>
const char* EncryptedName<18, 0xFBAD074BDB2B77A1ULL>::Text();
template <>
const char* EncryptedName<25, 0x6D05D5271B5D65C3ULL>::Text();
template <>
const char* EncryptedName<18, 0x050DF16FC75B25B9ULL>::Text();
template <>
const char* EncryptedName<25, 0x0789EBC9F5C1C59FULL>::Text();
template <>
const char* EncryptedName<22, 0x3D8387EF97A37DCFULL>::Text();
template <>
const char* EncryptedName<29, 0x07A7D947CF7323BFULL>::Text();
template <>
const char* EncryptedName<12, 0xD97DC927AFE911F1ULL>::Text();
template <>
const char* EncryptedName<26, 0x8BA52B970DD531ADULL>::Text();
template <>
const char* EncryptedName<16, 0x47B7BF55EB471557ULL>::Text();
template <>
const char* EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>::Text();
template <>
const char* EncryptedName<35, 0x2F2103792383718BULL>::Text();
template <>
const char* EncryptedName<28, 0x999F5191852D2F6BULL>::Text();
template <>
const char* EncryptedName<42, 0x4349ADE96FEB27E5ULL>::Text();
template <>
const char* EncryptedName<44, 0xA55B7BEF3D7377E3ULL>::Text();
template <>
const char* EncryptedName<30, 0x31E3CF9D358D8705ULL>::Text();
template <>
const char* EncryptedName<44, 0x79A58B0F2173CF49ULL>::Text();
template <>
const char* EncryptedName<46, 0xF5496753BDE7C5BDULL>::Text();
template <>
const char* EncryptedName<26, 0x795787B7779D19DBULL>::Text();
template <>
const char* EncryptedName<33, 0x6B3917C939150B9BULL>::Text();
template <>
const char* EncryptedName<26, 0x29AB0B59651BDFD5ULL>::Text();
template <>
const char* EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>::Text();
template <>
const char* EncryptedName<24, 0xA1DFCDAD555BB16DULL>::Text();
template <>
const char* EncryptedName<25, 0xAFF9332FD397533FULL>::Text();
template <>
const char* EncryptedName<31, 0x194F8339AF753FADULL>::Text();
template <>
const char* EncryptedName<26, 0x7DC10333F96DE3E7ULL>::Text();
template <>
const char* EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>::Text();
template <>
const char* EncryptedName<18, 0x51372B6F4BA14B23ULL>::Text();
template <>
const char* EncryptedName<25, 0xB70319D3A78D39CDULL>::Text();
template <>
const char* EncryptedName<18, 0x1F79132B518FD389ULL>::Text();
template <>
const char* EncryptedName<25, 0xC1FD95811D1F4FA1ULL>::Text();
template <>
const char* EncryptedName<24, 0x35F59F6F3DD1C965ULL>::Text();
template <>
const char* EncryptedName<31, 0xD7BBEB2DDB295D11ULL>::Text();
template <>
const char* EncryptedName<26, 0xBF0F412941076FADULL>::Text();
template <>
const char* EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>::Text();
template <>
const char* EncryptedName<30, 0xB59BADC5B365617DULL>::Text();
template <>
const char* EncryptedName<37, 0x77D925073D6F19B9ULL>::Text();
template <>
const char* EncryptedName<12, 0x91E7A7DFA58F9B6BULL>::Text();
template <>
const char* EncryptedName<12, 0x2529F16B0375530DULL>::Text();
template <>
const char* EncryptedName<14, 0x91DF1F5D3B014D45ULL>::Text();
template <>
const char* EncryptedName<14, 0x0D7FA94DD32361A9ULL>::Text();
template <>
const char* EncryptedName<14, 0x2F371DDDADDD53CFULL>::Text();
template <>
const char* EncryptedName<9, 0xA5C5D1A3B9457B53ULL>::Text();
template <>
const char* EncryptedName<11, 0x61F59D5D41E76519ULL>::Text();
template <>
const char* EncryptedName<13, 0x4B670715D543C1C3ULL>::Text();
template <>
const char* EncryptedName<14, 0x7155E5EF0DFB0D03ULL>::Text();
template <>
const char* EncryptedName<14, 0x4B1D078937DB892BULL>::Text();
template <>
const char* EncryptedName<14, 0x875FCDF1E7A1795FULL>::Text();
template <>
const char* EncryptedName<14, 0x5FFD7FABB5D1EF57ULL>::Text();

EncryptedName<14, 0x072D2D872D259D5BULL>& WizardBodyPath1();
EncryptedName<18, 0x9BE3EF737387AD79ULL>& PriestBodyPath1();
EncryptedName<14, 0xB96B77B7255F8569ULL>& AssassinBodyPath1();
EncryptedName<14, 0x11CB35636377DD07ULL>& SageBodyPath1();
EncryptedName<10, 0x4755C1355575FD37ULL>& BardBodyPath1();
EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>& DancerBodyPath1();
EncryptedName<10, 0x79DF094DD9616575ULL>& MonkBodyPath1();
EncryptedName<18, 0x038535BB092533B7ULL>& AlchemistBodyPath1();
EncryptedName<10, 0xFB2BC75769E777D3ULL>& RogueBodyPath1();
EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>& NoviceBodyPath1();
EncryptedName<10, 0x9B9D51AD3515F779ULL>& SwordmanBodyPath1();
EncryptedName<14, 0xB5F3F779237315F3ULL>& MagicianBodyPath1();
EncryptedName<10, 0x67DD5BAB1919D9D1ULL>& ArcherBodyPath1();
EncryptedName<14, 0x91493751EF33B5FDULL>& AcolyteBodyPath1();
EncryptedName<10, 0x6BC977B3B5993503ULL>& MerchantBodyPath1();
EncryptedName<10, 0xB7A76BF9CD41F99FULL>& ThiefBodyPath1();
EncryptedName<10, 0x732B390D63FD47D7ULL>& ThiefBodyPath2();
EncryptedName<14, 0x1FDB81175D1557E3ULL>& NoviceBodyPath2();
EncryptedName<10, 0xE3FB23537D2785BFULL>& SwordmanBodyPath2();
EncryptedName<14, 0x8BBF792B47E72173ULL>& MagicianBodyPath2();
EncryptedName<10, 0xB3CF9BEB7963DB89ULL>& ArcherBodyPath2();
EncryptedName<14, 0x71A509DB43792991ULL>& AcolyteBodyPath2();
EncryptedName<10, 0xDD69EDFFA18F537DULL>& MerchantBodyPath2();
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& ThiefBodyPath3();
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& KnightBodyPath1();
EncryptedName<18, 0x8F3F2FA959977F9FULL>& PriestBodyPath2();
EncryptedName<14, 0xE51B29334D353B81ULL>& WizardBodyPath2();
EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>& BlacksmithBodyPath1();
EncryptedName<10, 0x7FE563BD3FB7F925ULL>& HunterBodyPath1();
EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>& AssassinBodyPath2();
EncryptedName<28, 0x7DB1050779034DE7ULL>& PecoKnightBodyPath1();
EncryptedName<22, 0x01E7898D9D235DDFULL>& CrusaderBodyPath1();
EncryptedName<10, 0x0DF5B33D9F21EB79ULL>& MonkBodyPath2();
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& SageBodyPath2();
EncryptedName<10, 0x6961FF056999A303ULL>& RogueBodyPath2();
EncryptedName<18, 0xBD3F430159B551FDULL>& AlchemistBodyPath2();
EncryptedName<10, 0xC7FB6DF18577D34BULL>& BardBodyPath2();
EncryptedName<10, 0x5B832361BDA77197ULL>& DancerBodyPath2();
EncryptedName<34, 0x2B477F9B5B39B301ULL>& NewPecoCrusaderBodyPath1();
EncryptedName<14, 0x2B8725D7E95325F5ULL>& NoviceBodyPath3();
EncryptedName<10, 0x899D11BB83695B0DULL>& SwordmanBodyPath3();
EncryptedName<14, 0xBDADDD87C5A50B6BULL>& MagicianBodyPath3();
EncryptedName<10, 0x7989830DE73B9DBDULL>& ArcherBodyPath3();
EncryptedName<10, 0xA7471985F93F9BDFULL>& MerchantBodyPath3();
EncryptedName<10, 0xF535E561137F138DULL>& ThiefBodyPath4();
EncryptedName<10, 0xD3E5516F89F161D1ULL>& KnightBodyPath2();
EncryptedName<18, 0x45E3F103CBB5F157ULL>& PriestBodyPath3();
EncryptedName<14, 0x07112F85A39F4FABULL>& WizardBodyPath3();
EncryptedName<14, 0x2D33791F67AD47DFULL>& BlacksmithBodyPath2();
EncryptedName<10, 0x936F3D2FFDFD87F5ULL>& HunterBodyPath2();
EncryptedName<14, 0x2F2701A7E56D3D89ULL>& AssassinBodyPath3();
EncryptedName<28, 0x6B3301E1A3079125ULL>& PecoKnightBodyPath2();
EncryptedName<22, 0x71B7691737C5A1AFULL>& CrusaderBodyPath2();
EncryptedName<10, 0xC39FB559A17D3963ULL>& MonkBodyPath3();
EncryptedName<14, 0xFF7D83CB791339D9ULL>& SageBodyPath3();
EncryptedName<10, 0xED5143E7C1B39315ULL>& RogueBodyPath3();
EncryptedName<18, 0x393D916561BDC391ULL>& AlchemistBodyPath3();
EncryptedName<10, 0x31956551B311FD95ULL>& BardBodyPath3();
EncryptedName<10, 0x8741152D8FB34B4FULL>& DancerBodyPath3();
EncryptedName<34, 0x955D4DF94DE9BB07ULL>& OldPecoCrusaderBodyPath1();
EncryptedName<14, 0xC399B7091589B50BULL>& NoviceBodyPath4();
EncryptedName<18, 0xB739073FE921AD6FULL>& TaekwonBodyPath1();
EncryptedName<18, 0x17FFC98B398FAF61ULL>& TaekwonBodyPath2();
EncryptedName<18, 0x9B5B959B954D9333ULL>& TaekwonBodyPath3();
EncryptedName<14, 0xAFA5617F178B6185ULL>& WizardBodyPath4();
EncryptedName<14, 0x078143534999254DULL>& AcolyteBodyPath3();
EncryptedName<10, 0x3F5315DFE32B4FAFULL>& KnightBodyPath3();
EncryptedName<14, 0xE50B2935073DD3E9ULL>& SageBodyPath4();
EncryptedName<10, 0x7D750DADBDE9C3F1ULL>& KnightBodyPath4();
EncryptedName<10, 0x33832D4D3FDFBDBFULL>& KnightBodyPath5();
EncryptedName<28, 0x1BF915E1F37DFDFDULL>& PecoKnightBodyPath3();
EncryptedName<28, 0x01FF9313695DD9DFULL>& PecoKnightBodyPath4();
EncryptedName<28, 0x6DED3DD1CF75AD9BULL>& PecoKnightBodyPath5();
EncryptedName<28, 0x49C3759B77DBD3C1ULL>& PecoKnightBodyPath6();
EncryptedName<28, 0xD1538513DF975701ULL>& PecoKnightBodyPath7();
EncryptedName<28, 0x91E147EBBD47C39FULL>& PecoKnightBodyPath8();
EncryptedName<28, 0x4F1791FB1503C713ULL>& PecoKnightBodyPath9();
EncryptedName<28, 0xA1A55B4D1597EDD3ULL>& PecoKnightBodyPath10();
EncryptedName<28, 0x0753A7E1EFA9E139ULL>& PecoKnightBodyPath11();
EncryptedName<28, 0x7FC1015DFFCF6D19ULL>& PecoKnightBodyPath12();
EncryptedName<28, 0xCB3FBD4183DB8323ULL>& PecoKnightBodyPath13();
EncryptedName<18, 0x7FCD77F9C7CBD14DULL>& PriestBodyPath4();
EncryptedName<18, 0x5B8F49AF09390D1BULL>& PriestBodyPath5();
EncryptedName<14, 0x3B915119EFD523EFULL>& WizardBodyPath5();
EncryptedName<14, 0x474DC581D5E98357ULL>& WizardBodyPath6();
EncryptedName<14, 0x3F557BE333F95F6DULL>& BlacksmithBodyPath3();
EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>& BlacksmithBodyPath4();
EncryptedName<18, 0x0BEB63A5C905B3A7ULL>& MadogearBodyPath1();
EncryptedName<18, 0x078333BB9725B7C9ULL>& MadogearBodyPath2();
EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>& MadogearBodyPath3();
EncryptedName<10, 0x69FD1D9733D3CFC1ULL>& HunterBodyPath3();
EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>& HunterBodyPath4();
EncryptedName<22, 0x8B65CD61012B5931ULL>& RangerWolfBodyPath1();
EncryptedName<22, 0xF7530137154D8FE5ULL>& RangerWolfBodyPath2();
EncryptedName<22, 0x61FF87ED7BF39BE1ULL>& RangerWolfBodyPath3();
EncryptedName<14, 0x7D993F650D2785FFULL>& AssassinBodyPath4();
EncryptedName<14, 0x2D394DBD0F332733ULL>& AssassinBodyPath5();
EncryptedName<22, 0x151F77EBEBBB2147ULL>& CrusaderBodyPath3();
EncryptedName<22, 0xC7912587FFF31B71ULL>& CrusaderBodyPath4();
EncryptedName<34, 0x69D345DB950F336DULL>& NewPecoCrusaderBodyPath2();
EncryptedName<34, 0x9B17C9EFDBB39F2FULL>& NewPecoCrusaderBodyPath3();
EncryptedName<34, 0x2D3B15774149D5DDULL>& NewPecoCrusaderBodyPath4();
EncryptedName<10, 0xD98F03A52D35636DULL>& MonkBodyPath4();
EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>& MonkBodyPath5();
EncryptedName<14, 0xD3755DD95F47A909ULL>& SageBodyPath5();
EncryptedName<14, 0x31D7C1EDA345E36DULL>& SageBodyPath6();
EncryptedName<10, 0x3FAF85D995034FD7ULL>& RogueBodyPath4();
EncryptedName<10, 0x490FFB43A12F0DB9ULL>& RogueBodyPath5();
EncryptedName<18, 0x91651D81F559F971ULL>& AlchemistBodyPath4();
EncryptedName<18, 0x73D1674B61DF7B0BULL>& AlchemistBodyPath5();
EncryptedName<10, 0x2FF3C5036D070127ULL>& DancerBodyPath4();
EncryptedName<10, 0x17359BF953C367B3ULL>& DancerBodyPath5();
EncryptedName<10, 0xC52FB5FBB94FF917ULL>& BardBodyPath4();
EncryptedName<10, 0x93E7A3A1F587CDC5ULL>& BardBodyPath5();
EncryptedName<10, 0x59278F63D79F3DA1ULL>& KnightBodyPath6();
EncryptedName<14, 0x812BE72BCD5707B7ULL>& WizardBodyPath7();
EncryptedName<10, 0x3375B1C7DBB5FD55ULL>& HunterBodyPath5();
EncryptedName<18, 0x1D6D75EDE7CD311FULL>& PriestBodyPath6();
EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>& BlacksmithBodyPath5();
EncryptedName<14, 0x7DBBD74FA36F3BCBULL>& AssassinBodyPath6();
EncryptedName<22, 0x4917EB6D71B701ABULL>& CrusaderBodyPath5();
EncryptedName<14, 0xFD4F85BBA7E37D4FULL>& SageBodyPath7();
EncryptedName<10, 0x51C57BB9F1DD9B0FULL>& BardBodyPath6();
EncryptedName<10, 0xC733B7A333F9F571ULL>& DancerBodyPath6();
EncryptedName<10, 0x7D61612937C1592DULL>& MonkBodyPath6();
EncryptedName<18, 0x8F4B518D839D1581ULL>& AlchemistBodyPath6();
EncryptedName<10, 0xCBD7DDAFDF87F549ULL>& RogueBodyPath6();
EncryptedName<14, 0x39E7F13755252F8FULL>& NoviceBodyPath5();
EncryptedName<14, 0x3D075BD5256F9B6FULL>& NoviceBodyPath6();
EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>& NoviceBodyPath7();
EncryptedName<14, 0x03014749B127914FULL>& NoviceBodyPath8();
EncryptedName<18, 0x251761D7370BA5C3ULL>& PriestBodyPath7();
EncryptedName<14, 0x2B034DCBB1812785ULL>& WizardBodyPath8();
EncryptedName<14, 0x85FB93CF2F494179ULL>& BlacksmithBodyPath6();
EncryptedName<14, 0x894FEDABAFCF45E3ULL>& BlacksmithBodyPath7();
EncryptedName<14, 0x8B03E17DA967E943ULL>& BlacksmithBodyPath8();
EncryptedName<10, 0x190B8705C141D1CFULL>& HunterBodyPath6();
EncryptedName<10, 0xC98BB32777F94371ULL>& HunterBodyPath7();
EncryptedName<10, 0xA5F51F3989DBB99FULL>& KnightBodyPath7();
EncryptedName<10, 0xB3D5C99B2B05FFEFULL>& KnightBodyPath8();
EncryptedName<10, 0xC71BB3CDEB499DD1ULL>& KnightBodyPath9();
EncryptedName<22, 0x3939079FF3395587ULL>& CrusaderBodyPath6();
EncryptedName<22, 0xCF391B0F8B9D95DBULL>& CrusaderBodyPath7();
EncryptedName<10, 0x6967633B9D89092BULL>& KnightBodyPath10();
EncryptedName<10, 0xAFCD4F69FBDFE157ULL>& KnightBodyPath11();
EncryptedName<22, 0x85D9CFCF0FBB71ABULL>& CrusaderBodyPath8();
EncryptedName<22, 0x795383DD11AF7DA7ULL>& CrusaderBodyPath9();
EncryptedName<22, 0xD9FF511B2BB921E7ULL>& CrusaderBodyPath10();
EncryptedName<16, 0x978B51F50DC5FD25ULL>& KagerouBodyPath1();
EncryptedName<12, 0xC757D96143298511ULL>& OboroBodyPath1();
EncryptedName<16, 0xE1F181F923B759E7ULL>& KagerouBodyPath2();
EncryptedName<12, 0x37D76D8F495113C3ULL>& OboroBodyPath2();
EncryptedName<20, 0x7BAD7791F50F5D8DULL>& RebellionBodyPath1();
EncryptedName<18, 0x2F1337956DC3CFD5ULL>& SummonerBodyPath1();
EncryptedName<23, 0xC91F9D493F75FDA3ULL>& SummonerCartBodyPath1();
EncryptedName<18, 0x1F01A13D8D915327ULL>& SummonerBodyPath2();
EncryptedName<23, 0xF33D3D3B0BD9C53DULL>& SummonerCartBodyPath2();
EncryptedName<10, 0xC74BB3BB110FB10FULL>& NinjaBodyPath1();
EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>& NinjaBodyPath2();
EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>& KagerouBodyPath3();
EncryptedName<12, 0x8773593DB181D573ULL>& OboroBodyPath3();
EncryptedName<16, 0xFBDBDB13A77191BFULL>& KagerouBodyPath4();
EncryptedName<12, 0x19BBC5910555FB97ULL>& OboroBodyPath4();
EncryptedName<18, 0xBBD983C7FD611FE3ULL>& TaekwonBodyPath4();
EncryptedName<18, 0xA1ED494BF319772DULL>& TaekwonBodyPath5();
EncryptedName<18, 0x798B394FF5FF434DULL>& TaekwonBodyPath6();
EncryptedName<18, 0x410DD52D6731BFFDULL>& TaekwonBodyPath7();
EncryptedName<18, 0x07B733370F612D0FULL>& TaekwonBodyPath8();
EncryptedName<14, 0x113DC179F1C795C9ULL>& WizardBodyPath9();
EncryptedName<14, 0x31F5C32DD5C53779ULL>& WizardBodyPath10();
EncryptedName<10, 0x2F9B717365C1250FULL>& GunslingerBodyPath1();
EncryptedName<10, 0x4FE54379493FB5D5ULL>& GunslingerBodyPath2();
EncryptedName<20, 0x656FAB972B3D21BBULL>& RebellionBodyPath2();
EncryptedName<20, 0x33CD2BDD5B3927EDULL>& RebellionBodyPath3();
EncryptedName<18, 0xD97BB73901ED99BFULL>& TaekwonBodyPath9();
EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>& WizardBodyPath11();
EncryptedName<18, 0xE5FDE3151BED831FULL>& TaekwonBodyPath10();
EncryptedName<14, 0x8971CB79C18D8B27ULL>& WizardBodyPath12();
EncryptedName<18, 0x8DC5D10DAB1585BDULL>& TaekwonBodyPath11();
EncryptedName<18, 0x21156F4141E35DD9ULL>& TaekwonBodyPath12();
EncryptedName<18, 0x51EB99CBCB075F35ULL>& TaekwonBodyPath13();
EncryptedName<14, 0x1B3B756FE399E1D9ULL>& WizardBodyPath13();
EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>& TaekwonBodyPath14();
EncryptedName<14, 0x451FD72783630709ULL>& WizardBodyPath14();
EncryptedName<14, 0x73C32959FDFFA51BULL>& WizardBodyPath15();
EncryptedName<14, 0xA15B555BE9CD2B81ULL>& WizardBodyPath16();
EncryptedName<14, 0xD9115915B1919533ULL>& SageBodyPath8();
EncryptedName<14, 0xB3F1A10745C1F105ULL>& SageBodyPath9();
EncryptedName<18, 0x99157F159BB99BD9ULL>& WindhawkBodyPath1();
EncryptedName<23, 0x7959FFBF652D07BFULL>& WolfWindhawkBodyPath1();
EncryptedName<25, 0x01C9FBA7819FFBD7ULL>& WindhawkRidingBodyPath1();
EncryptedName<22, 0xABED6971354375C3ULL>& TroubadourBodyPath1();
EncryptedName<29, 0x6BA557130BDFE997ULL>& TroubadourRidingBodyPath1();
EncryptedName<18, 0xFBAD074BDB2B77A1ULL>& TrouvereBodyPath1();
EncryptedName<25, 0x6D05D5271B5D65C3ULL>& TrouvereRidingBodyPath1();
EncryptedName<18, 0x050DF16FC75B25B9ULL>& CardinalBodyPath1();
EncryptedName<25, 0x0789EBC9F5C1C59FULL>& CardinalRidingBodyPath1();
EncryptedName<22, 0x3D8387EF97A37DCFULL>& InquisitorBodyPath1();
EncryptedName<29, 0x07A7D947CF7323BFULL>& InquisitorRidingBodyPath1();
EncryptedName<12, 0xD97DC927AFE911F1ULL>& BioloBodyPath1();
EncryptedName<26, 0x8BA52B970DD531ADULL>& BioloRidingBodyPath1();
EncryptedName<16, 0x47B7BF55EB471557ULL>& MeisterBodyPath1();
EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>& MeisterRidingBodyPath1();
EncryptedName<35, 0x2F2103792383718BULL>& MeisterMadogear1BodyPath1();
EncryptedName<28, 0x999F5191852D2F6BULL>& DragonKnightBodyPath1();
EncryptedName<42, 0x4349ADE96FEB27E5ULL>& DragonKnightRidingBodyPath1();
EncryptedName<44, 0xA55B7BEF3D7377E3ULL>& DragonKnightChickenBodyPath1();
EncryptedName<30, 0x31E3CF9D358D8705ULL>& ImperialGuardBodyPath1();
EncryptedName<44, 0x79A58B0F2173CF49ULL>& ImperialGuardRidingBodyPath1();
EncryptedName<46, 0xF5496753BDE7C5BDULL>& ImperialGuardChickenBodyPath1();
EncryptedName<26, 0x795787B7779D19DBULL>& ShadowCrossBodyPath1();
EncryptedName<33, 0x6B3917C939150B9BULL>& ShadowCrossRidingBodyPath1();
EncryptedName<26, 0x29AB0B59651BDFD5ULL>& AbyssChaserBodyPath1();
EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>& AbyssChaserRidingBodyPath1();
EncryptedName<24, 0xA1DFCDAD555BB16DULL>& SkyEmperorBodyPath1();
EncryptedName<25, 0xAFF9332FD397533FULL>& SkyEmperor2BodyPath1();
EncryptedName<31, 0x194F8339AF753FADULL>& SkyEmperorRidingBodyPath1();
EncryptedName<26, 0x7DC10333F96DE3E7ULL>& SoulAsceticBodyPath1();
EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>& SoulAsceticRidingBodyPath1();
EncryptedName<18, 0x51372B6F4BA14B23ULL>& ShinkiroBodyPath1();
EncryptedName<25, 0xB70319D3A78D39CDULL>& ShinkiroRidingBodyPath1();
EncryptedName<18, 0x1F79132B518FD389ULL>& ShiranuiBodyPath1();
EncryptedName<25, 0xC1FD95811D1F4FA1ULL>& ShiranuiRidingBodyPath1();
EncryptedName<24, 0x35F59F6F3DD1C965ULL>& NightWatchBodyPath1();
EncryptedName<31, 0xD7BBEB2DDB295D11ULL>& NightWatchRidingBodyPath1();
EncryptedName<26, 0xBF0F412941076FADULL>& HyperNoviceBodyPath1();
EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>& HyperNoviceRidingBodyPath1();
EncryptedName<30, 0xB59BADC5B365617DULL>& SpiritHandlerBodyPath1();
EncryptedName<37, 0x77D925073D6F19B9ULL>& SpiritHandlerRidingBodyPath1();
EncryptedName<12, 0x91E7A7DFA58F9B6BULL>& DruidBodyPath1();
EncryptedName<12, 0x2529F16B0375530DULL>& DruidBodyPath2();
EncryptedName<14, 0x91DF1F5D3B014D45ULL>& KarnosBodyPath1();
EncryptedName<14, 0x0D7FA94DD32361A9ULL>& KarnosBodyPath2();
EncryptedName<14, 0x2F371DDDADDD53CFULL>& AliteaBodyPath1();
EncryptedName<6, 0x2F3D75DFCF678FF3ULL>& DruidJobName1();
EncryptedName<6, 0x3BBB5F0F8F216F31ULL>& DruidJobName2();
EncryptedName<7, 0x6F9D21B96D71C1FFULL>& KarnosJobName1();
EncryptedName<7, 0xED730315539785C7ULL>& KarnosJobName2();
EncryptedName<7, 0x8F61434F0F79DDD7ULL>& AliteaJobName1();
EncryptedName<9, 0xA5C5D1A3B9457B53ULL>& WerewolfJobName1();
EncryptedName<11, 0x61F59D5D41E76519ULL>& WereRaptorJobName1();
EncryptedName<13, 0x4B670715D543C1C3ULL>& DruidRidingName();
EncryptedName<14, 0x7155E5EF0DFB0D03ULL>& KarnosRidingName();
EncryptedName<14, 0x4B1D078937DB892BULL>& AliteaRidingName();

// OFFSET: 0x00DE98B0
// FUNCTION: fn_23502
void CSession::InitResourceNameTables() {
  std::vector<const char*>& bodyJobNames = reinterpret_cast<std::vector<const char*>&>(this->m_bodyJobNames);
  bodyJobNames.resize(28053, "초보자");
  bodyJobNames[0] = "초보자";
  bodyJobNames[26] = "산타";
  bodyJobNames[27] = "여름";
  bodyJobNames[28] = "한복";
  bodyJobNames[29] = "옥토버패스트";
  bodyJobNames[30] = "여름2";
  bodyJobNames[1] = "검사";
  bodyJobNames[2] = "마법사";
  bodyJobNames[3] = "궁수";
  bodyJobNames[4] = "성직자";
  bodyJobNames[5] = "상인";
  bodyJobNames[6] = "도둑";
  bodyJobNames[7] = "기사";
  bodyJobNames[8] = "프리스트";
  bodyJobNames[9] = "위저드";
  bodyJobNames[10] = "제철공";
  bodyJobNames[11] = "헌터";
  bodyJobNames[12] = "어세신";
  bodyJobNames[13] = "페코페코_기사";
  bodyJobNames[14] = "크루세이더";
  bodyJobNames[15] = "몽크";
  bodyJobNames[16] = "세이지";
  bodyJobNames[17] = "로그";
  bodyJobNames[18] = "연금술사";
  bodyJobNames[19] = "바드";
  bodyJobNames[20] = "무희";
  bodyJobNames[21] = "신페코크루세이더";
  bodyJobNames[23] = "슈퍼노비스";
  bodyJobNames[22] = "결혼";
  bodyJobNames[24] = "건너";
  bodyJobNames[25] = "닌자";
  bodyJobNames[164] = "두꺼비닌자";
  bodyJobNames[167] = "두꺼비소울링커";
  bodyJobNames[165] = "페코건너";
  bodyJobNames[166] = "페코검사";
  bodyJobNames[168] = "화이트스미스멧돼지";
  bodyJobNames[169] = "상인멧돼지";
  bodyJobNames[170] = "제네릭멧돼지";
  bodyJobNames[171] = "크리에이터멧돼지";
  bodyJobNames[172] = "타조궁수";
  bodyJobNames[173] = "권성포링";
  bodyJobNames[174] = "노비스포링";
  bodyJobNames[175] = "몽크알파카";
  bodyJobNames[176] = "복사알파카";
  bodyJobNames[177] = "슈라알파카";
  bodyJobNames[178] = "슈퍼노비스포링";
  bodyJobNames[179] = "아크비숍알파카";
  bodyJobNames[180] = "여우마법사";
  bodyJobNames[181] = "여우세이지";
  bodyJobNames[182] = "여우소서러";
  bodyJobNames[183] = "여우워록";
  bodyJobNames[184] = "여우위저드";
  bodyJobNames[185] = "여우프로페서";
  bodyJobNames[186] = "여우하이위저드";
  bodyJobNames[187] = "연금술사멧돼지";
  bodyJobNames[188] = "제철공멧돼지";
  bodyJobNames[189] = "챔피온알파카";
  bodyJobNames[190] = "켈베로스길로틴크로스";
  bodyJobNames[191] = "켈베로스도둑";
  bodyJobNames[192] = "켈베로스로그";
  bodyJobNames[193] = "켈베로스쉐도우체이서";
  bodyJobNames[194] = "켈베로스스토커";
  bodyJobNames[195] = "켈베로스어쎄신";
  bodyJobNames[196] = "켈베로스어쎄신크로스";
  bodyJobNames[197] = "타조무희";
  bodyJobNames[198] = "타조민스트럴";
  bodyJobNames[199] = "타조바드";
  bodyJobNames[200] = "타조스나이퍼";
  bodyJobNames[201] = "타조원더러";
  bodyJobNames[202] = "타조짚시";
  bodyJobNames[203] = "타조크라운";
  bodyJobNames[204] = "타조헌터";
  bodyJobNames[205] = "태권소년포링";
  bodyJobNames[206] = "프리스트알파카";
  bodyJobNames[207] = "하이프리스트알파카";
  bodyJobNames[208] = "노비스포링";
  bodyJobNames[209] = "페코검사";
  bodyJobNames[210] = "여우마법사";
  bodyJobNames[211] = "타조궁수";
  bodyJobNames[212] = "복사알파카";
  bodyJobNames[213] = "상인멧돼지";
  bodyJobNames[214] = "타조헌터";
  bodyJobNames[215] = "켈베로스어쎄신";
  bodyJobNames[216] = "몽크알파카";
  bodyJobNames[217] = "여우세이지";
  bodyJobNames[218] = "켈베로스로그";
  bodyJobNames[219] = "연금술사멧돼지";
  bodyJobNames[220] = "타조바드";
  bodyJobNames[221] = "타조무희";
  bodyJobNames[222] = "슈퍼노비스포링";
  bodyJobNames[223] = "여우워록";
  bodyJobNames[224] = "아크비숍알파카";
  bodyJobNames[225] = "켈베로스길로틴크로스";
  bodyJobNames[226] = "여우소서러";
  bodyJobNames[227] = "타조민스트럴";
  bodyJobNames[228] = "타조원더러";
  bodyJobNames[229] = "슈라알파카";
  bodyJobNames[230] = "제네릭멧돼지";
  bodyJobNames[232] = "켈베로스쉐도우체이서";
  bodyJobNames[233] = "노비스포링";
  bodyJobNames[234] = "페코검사";
  bodyJobNames[235] = "여우마법사";
  bodyJobNames[236] = "타조궁수";
  bodyJobNames[237] = "복사알파카";
  bodyJobNames[238] = "상인멧돼지";
  bodyJobNames[239] = "켈베로스도둑";
  bodyJobNames[231] = "켈베로스도둑";
  bodyJobNames[51] = "초보자";
  bodyJobNames[52] = "검사";
  bodyJobNames[53] = "마법사";
  bodyJobNames[54] = "궁수";
  bodyJobNames[55] = "성직자";
  bodyJobNames[56] = "상인";
  bodyJobNames[57] = "도둑";
  bodyJobNames[58] = "로드나이트";
  bodyJobNames[64] = "로드페코";
  bodyJobNames[60] = "하이위저드";
  bodyJobNames[61] = "화이트스미스";
  bodyJobNames[63] = "어쌔신크로스";
  bodyJobNames[59] = "하이프리";
  bodyJobNames[62] = "스나이퍼";
  bodyJobNames[68] = "스토커";
  bodyJobNames[67] = "프로페서";
  bodyJobNames[69] = "크리에이터";
  bodyJobNames[66] = "챔피온";
  bodyJobNames[70] = "클라운";
  bodyJobNames[71] = "집시";
  bodyJobNames[65] = "팔라딘";
  bodyJobNames[72] = "페코팔라딘";
  bodyJobNames[73] = "초보자";
  bodyJobNames[100] = "성직자";
  bodyJobNames[101] = "기사";
  bodyJobNames[102] = "세이지";
  bodyJobNames[74] = "검사";
  bodyJobNames[75] = "마법사";
  bodyJobNames[76] = "궁수";
  bodyJobNames[77] = "성직자";
  bodyJobNames[78] = "상인";
  bodyJobNames[79] = "도둑";
  bodyJobNames[80] = "기사";
  bodyJobNames[81] = "프리스트";
  bodyJobNames[82] = "위저드";
  bodyJobNames[83] = "제철공";
  bodyJobNames[84] = "헌터";
  bodyJobNames[85] = "어세신";
  bodyJobNames[86] = "페코페코_기사";
  bodyJobNames[87] = "크루세이더";
  bodyJobNames[88] = "몽크";
  bodyJobNames[89] = "세이지";
  bodyJobNames[90] = "로그";
  bodyJobNames[91] = "연금술사";
  bodyJobNames[92] = "바드";
  bodyJobNames[93] = "무희바지";
  bodyJobNames[94] = "구페코크루세이더";
  bodyJobNames[95] = "슈퍼노비스";
  bodyJobNames[96] = "태권소년";
  bodyJobNames[97] = "권성";
  bodyJobNames[98] = "권성융합";
  bodyJobNames[99] = "소울링커";
  bodyJobNames[104] = "룬나이트";
  bodyJobNames[110] = "룬나이트";
  bodyJobNames[130] = "룬나이트쁘띠";
  bodyJobNames[131] = "룬나이트쁘띠";
  bodyJobNames[159] = "룬나이트쁘띠";
  bodyJobNames[138] = "룬나이트쁘띠2";
  bodyJobNames[139] = "룬나이트쁘띠2";
  bodyJobNames[140] = "룬나이트쁘띠3";
  bodyJobNames[141] = "룬나이트쁘띠3";
  bodyJobNames[142] = "룬나이트쁘띠4";
  bodyJobNames[143] = "룬나이트쁘띠4";
  bodyJobNames[144] = "룬나이트쁘띠5";
  bodyJobNames[145] = "룬나이트쁘띠5";
  bodyJobNames[105] = "워록";
  bodyJobNames[111] = "워록";
  bodyJobNames[106] = "레인져";
  bodyJobNames[112] = "레인져";
  bodyJobNames[134] = "레인져늑대";
  bodyJobNames[135] = "레인져늑대";
  bodyJobNames[161] = "레인져늑대";
  bodyJobNames[107] = "아크비숍";
  bodyJobNames[113] = "아크비숍";
  bodyJobNames[108] = "미케닉";
  bodyJobNames[114] = "미케닉";
  bodyJobNames[136] = "마도기어";
  bodyJobNames[137] = "마도기어";
  bodyJobNames[162] = "마도기어";
  bodyJobNames[109] = "길로틴크로스";
  bodyJobNames[115] = "길로틴크로스";
  bodyJobNames[116] = "가드";
  bodyJobNames[123] = "가드";
  bodyJobNames[132] = "그리폰가드";
  bodyJobNames[133] = "그리폰가드";
  bodyJobNames[160] = "그리폰가드";
  bodyJobNames[117] = "소서러";
  bodyJobNames[124] = "소서러";
  bodyJobNames[118] = "민스트럴";
  bodyJobNames[125] = "민스트럴";
  bodyJobNames[119] = "원더러";
  bodyJobNames[126] = "원더러";
  bodyJobNames[120] = "슈라";
  bodyJobNames[127] = "슈라";
  bodyJobNames[121] = "제네릭";
  bodyJobNames[128] = "제네릭";
  bodyJobNames[122] = "쉐도우체이서";
  bodyJobNames[129] = "쉐도우체이서";
  bodyJobNames[146] = "룬나이트";
  bodyJobNames[147] = "워록";
  bodyJobNames[148] = "레인져";
  bodyJobNames[149] = "아크비숍";
  bodyJobNames[150] = "미케닉";
  bodyJobNames[151] = "길로틴크로스";
  bodyJobNames[152] = "가드";
  bodyJobNames[153] = "소서러";
  bodyJobNames[154] = "민스트럴";
  bodyJobNames[155] = "원더러";
  bodyJobNames[156] = "슈라";
  bodyJobNames[157] = "제네릭";
  bodyJobNames[158] = "쉐도우체이서";
  bodyJobNames[240] = "슈퍼노비스";
  bodyJobNames[241] = "슈퍼노비스";
  bodyJobNames[242] = "슈퍼노비스포링";
  bodyJobNames[243] = "슈퍼노비스포링";
  bodyJobNames[244] = "프리스트알파카";
  bodyJobNames[245] = "여우위저드";
  bodyJobNames[246] = "제철공멧돼지";
  bodyJobNames[247] = "미케닉멧돼지";
  bodyJobNames[255] = "미케닉멧돼지";
  bodyJobNames[248] = "타조레인져";
  bodyJobNames[256] = "타조레인져";
  bodyJobNames[249] = "사자기사";
  bodyJobNames[257] = "사자기사";
  bodyJobNames[250] = "사자로드나이트";
  bodyJobNames[251] = "사자로얄가드";
  bodyJobNames[258] = "사자로얄가드";
  bodyJobNames[252] = "사자룬나이트";
  bodyJobNames[259] = "사자룬나이트";
  bodyJobNames[253] = "사자크루세이더";
  bodyJobNames[260] = "사자크루세이더";
  bodyJobNames[254] = "사자팔라딘";
  bodyJobNames[261] = "kagerou";
  bodyJobNames[262] = "oboro";
  bodyJobNames[263] = "frog_kagerou";
  bodyJobNames[264] = "frog_oboro";
  bodyJobNames[265] = "rebellion";
  bodyJobNames[266] = "peco_rebellion";
  bodyJobNames[268] = "summoner";
  bodyJobNames[269] = "cart_summoner";
  bodyJobNames[270] = "summoner";
  bodyJobNames[271] = "cart_summoner";
  bodyJobNames[272] = "닌자";
  bodyJobNames[280] = "두꺼비닌자";
  bodyJobNames[273] = "kagerou";
  bodyJobNames[281] = "frog_kagerou";
  bodyJobNames[274] = "oboro";
  bodyJobNames[282] = "frog_oboro";
  bodyJobNames[275] = "태권소년";
  bodyJobNames[283] = "태권소년포링";
  bodyJobNames[276] = "권성";
  bodyJobNames[288] = "권성융합";
  bodyJobNames[284] = "권성포링";
  bodyJobNames[277] = "소울링커";
  bodyJobNames[285] = "두꺼비소울링커";
  bodyJobNames[278] = "건너";
  bodyJobNames[286] = "페코건너";
  bodyJobNames[279] = "rebellion";
  bodyJobNames[287] = "peco_rebellion";
  bodyJobNames[289] = "성제";
  bodyJobNames[290] = "소울리퍼";
  bodyJobNames[291] = "성제";
  bodyJobNames[292] = "소울리퍼";
  bodyJobNames[293] = "성제융합";
  bodyJobNames[294] = "성제융합";
  bodyJobNames[295] = "해태성제";
  bodyJobNames[296] = "해태소울리퍼";
  bodyJobNames[297] = "해태성제";
  bodyJobNames[298] = "해태소울리퍼";
  bodyJobNames[302] = "DRAGON_KNIGHT";
  bodyJobNames[315] = "DRAGON_KNIGHT_RIDING";
  bodyJobNames[330] = "DRAGON_KNIGHT_CHICKEN";
  bodyJobNames[308] = "IMPERIAL_GUARD";
  bodyJobNames[321] = "IMPERIAL_GUARD_RIDING";
  bodyJobNames[331] = "IMPERIAL_GUARD_CHICKEN";
  bodyJobNames[305] = "ARCH_MAGE";
  bodyJobNames[318] = "ARCH_MAGE_RIDING";
  bodyJobNames[311] = "ELEMETAL_MASTER";
  bodyJobNames[324] = "ELEMETAL_MASTER_RIDING";
  bodyJobNames[307] = "WINDHAWK";
  bodyJobNames[328] = "WOLF_WINDHAWK";
  bodyJobNames[320] = "WINDHAWK_RIDING";
  bodyJobNames[313] = "TROUBADOUR";
  bodyJobNames[326] = "TROUBADOUR_RIDING";
  bodyJobNames[314] = "TROUVERE";
  bodyJobNames[327] = "TROUVERE_RIDING";
  bodyJobNames[306] = "CARDINAL";
  bodyJobNames[319] = "CARDINAL_RIDING";
  bodyJobNames[312] = "INQUISITOR";
  bodyJobNames[325] = "INQUISITOR_RIDING";
  bodyJobNames[309] = "BIOLO";
  bodyJobNames[322] = "BIOLO_RIDING";
  bodyJobNames[303] = "MEISTER";
  bodyJobNames[316] = "MEISTER_RIDING";
  bodyJobNames[329] = "MEISTER_MADOGEAR1";
  bodyJobNames[304] = "SHADOW_CROSS";
  bodyJobNames[317] = "SHADOW_CROSS_RIDING";
  bodyJobNames[310] = "ABYSS_CHASER";
  bodyJobNames[323] = "ABYSS_CHASER_RIDING";
  bodyJobNames[352] = "SKY_EMPEROR";
  bodyJobNames[366] = "SKY_EMPEROR2";
  bodyJobNames[359] = "SKY_EMPEROR_RIDING";
  bodyJobNames[353] = "SOUL_ASCETIC";
  bodyJobNames[360] = "SOUL_ASCETIC_RIDING";
  bodyJobNames[354] = "SHINKIRO";
  bodyJobNames[361] = "SHINKIRO_RIDING";
  bodyJobNames[355] = "SHIRANUI";
  bodyJobNames[362] = "SHIRANUI_RIDING";
  bodyJobNames[356] = "NIGHT_WATCH";
  bodyJobNames[363] = "NIGHT_WATCH_RIDING";
  bodyJobNames[357] = "HYPER_NOVICE";
  bodyJobNames[364] = "HYPER_NOVICE_RIDING";
  bodyJobNames[358] = "SPIRIT_HANDLER";
  bodyJobNames[365] = "SPIRIT_HANDLER_RIDING";
  {
    const std::uint8_t source[] = {0xC9, 0xE9, 0x9A, 0x32, 0xD5, 0xA7};
    thread_local EncryptedName<sizeof(source), 0xB39FA7917BCFBB8DULL> name(source);
    bodyJobNames[401] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xB3, 0x41, 0x7E, 0xE6, 0xB5, 0x0F};
    thread_local EncryptedName<sizeof(source), 0x8D1D0FF1AF2B13F7ULL> name(source);
    bodyJobNames[402] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x12, 0xF6, 0x55, 0x5D, 0x8A, 0x2E, 0x63};
    thread_local EncryptedName<sizeof(source), 0x4F637DC51307B759ULL> name(source);
    bodyJobNames[403] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE0, 0x80, 0x1F, 0x77, 0x28, 0x26, 0xCB};
    thread_local EncryptedName<sizeof(source), 0x2DCB7567394DC1ABULL> name(source);
    bodyJobNames[404] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x34, 0x77, 0x4A, 0x7B, 0x56, 0x80, 0xA1};
    thread_local EncryptedName<sizeof(source), 0x6BA1C1132F033B75ULL> name(source);
    bodyJobNames[405] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x78, 0x92, 0x17, 0x60, 0x6E, 0xE4, 0x61, 0xE7, 0x2F};
    thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
    bodyJobNames[406] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9A, 0x46, 0x9F, 0x3E, 0xC5, 0x7A, 0x43, 0x67, 0x82, 0x51, 0xCD};
    thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
    bodyJobNames[407] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x95, 0x09, 0xC2, 0xAC, 0x7D, 0xD8, 0xB9, 0x66, 0x95, 0x12, 0xD9, 0xA2, 0x39};
    thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
    bodyJobNames[408] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x10, 0x12, 0x0D, 0x93, 0xC8, 0xCC, 0xFC, 0x6B, 0x12, 0x17, 0x16, 0x93, 0xC0, 0x9F};
    thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
    bodyJobNames[409] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x36, 0xDB, 0xCE, 0x49, 0x66, 0x12, 0xBE, 0x3D, 0x3E, 0xD3, 0xCE, 0x53, 0x64, 0x53};
    thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
    bodyJobNames[410] = name.Text();
  }
  std::vector<const char*>& imfNames = reinterpret_cast<std::vector<const char*>&>(this->m_imfNames);
  imfNames.resize(28053, "초보자");
  imfNames[1] = "검사";
  imfNames[2] = "마법사";
  imfNames[3] = "궁수";
  imfNames[4] = "성직자";
  imfNames[5] = "상인";
  imfNames[6] = "도둑";
  imfNames[7] = "기사";
  imfNames[8] = "프리스트";
  imfNames[9] = "위저드";
  imfNames[10] = "제철공";
  imfNames[11] = "헌터";
  imfNames[12] = "어세신";
  imfNames[13] = "페코페코_기사";
  imfNames[14] = "크루세이더";
  imfNames[15] = "몽크";
  imfNames[16] = "세이지";
  imfNames[17] = "로그";
  imfNames[18] = "연금술사";
  imfNames[19] = "바드";
  imfNames[20] = "무희";
  imfNames[21] = "신페코크루세이더";
  imfNames[23] = "초보자";
  imfNames[22] = "결혼";
  imfNames[26] = "결혼";
  imfNames[27] = "결혼";
  imfNames[28] = "결혼";
  imfNames[29] = "결혼";
  imfNames[30] = "결혼";
  imfNames[24] = "건너";
  imfNames[25] = "닌자";
  imfNames[164] = "두꺼비닌자";
  imfNames[167] = "두꺼비소울링커";
  imfNames[165] = "페코건너";
  imfNames[166] = "페코페코_기사";
  imfNames[168] = "화이트스미스멧돼지";
  imfNames[169] = "상인멧돼지";
  imfNames[170] = "제네릭멧돼지";
  imfNames[171] = "크리에이터멧돼지";
  imfNames[172] = "타조궁수";
  imfNames[173] = "권성포링";
  imfNames[174] = "노비스포링";
  imfNames[175] = "몽크알파카";
  imfNames[176] = "복사알파카";
  imfNames[177] = "슈라알파카";
  imfNames[178] = "슈퍼노비스포링";
  imfNames[179] = "아크비숍알파카";
  imfNames[180] = "여우마법사";
  imfNames[181] = "여우세이지";
  imfNames[182] = "여우소서러";
  imfNames[183] = "여우워록";
  imfNames[184] = "여우위저드";
  imfNames[185] = "여우프로페서";
  imfNames[186] = "여우하이위저드";
  imfNames[187] = "연금술사멧돼지";
  imfNames[188] = "제철공멧돼지";
  imfNames[189] = "챔피온알파카";
  imfNames[190] = "켈베로스길로틴크로스";
  imfNames[191] = "켈베로스도둑";
  imfNames[192] = "켈베로스로그";
  imfNames[193] = "켈베로스쉐도우체이서";
  imfNames[194] = "켈베로스스토커";
  imfNames[195] = "켈베로스어쎄신";
  imfNames[196] = "켈베로스어쎄신크로스";
  imfNames[197] = "타조무희";
  imfNames[198] = "타조민스트럴";
  imfNames[199] = "타조바드";
  imfNames[200] = "타조스나이퍼";
  imfNames[201] = "타조원더러";
  imfNames[202] = "타조짚시";
  imfNames[203] = "타조크라운";
  imfNames[204] = "타조헌터";
  imfNames[205] = "태권소년포링";
  imfNames[206] = "프리스트알파카";
  imfNames[207] = "하이프리스트알파카";
  imfNames[208] = "노비스포링";
  imfNames[209] = "페코페코_기사";
  imfNames[210] = "여우마법사";
  imfNames[211] = "타조궁수";
  imfNames[212] = "복사알파카";
  imfNames[213] = "상인멧돼지";
  imfNames[214] = "타조헌터";
  imfNames[215] = "켈베로스어쎄신";
  imfNames[216] = "몽크알파카";
  imfNames[217] = "여우세이지";
  imfNames[218] = "켈베로스로그";
  imfNames[219] = "연금술사멧돼지";
  imfNames[220] = "타조바드";
  imfNames[221] = "타조무희";
  imfNames[222] = "슈퍼노비스포링";
  imfNames[223] = "여우워록";
  imfNames[224] = "아크비숍알파카";
  imfNames[225] = "켈베로스길로틴크로스";
  imfNames[226] = "여우소서러";
  imfNames[227] = "타조민스트럴";
  imfNames[228] = "타조원더러";
  imfNames[229] = "슈라알파카";
  imfNames[230] = "제네릭멧돼지";
  imfNames[232] = "켈베로스쉐도우체이서";
  imfNames[233] = "노비스포링";
  imfNames[234] = "페코페코_기사";
  imfNames[235] = "여우마법사";
  imfNames[236] = "타조궁수";
  imfNames[237] = "복사알파카";
  imfNames[238] = "상인멧돼지";
  imfNames[239] = "켈베로스도둑";
  imfNames[231] = "켈베로스도둑";
  imfNames[51] = "초보자";
  imfNames[52] = "검사";
  imfNames[53] = "마법사";
  imfNames[54] = "궁수";
  imfNames[55] = "성직자";
  imfNames[56] = "상인";
  imfNames[57] = "도둑";
  imfNames[58] = "기사";
  imfNames[59] = "프리스트";
  imfNames[60] = "하이위저드";
  imfNames[61] = "제철공";
  imfNames[62] = "헌터";
  imfNames[63] = "어세신크로스";
  imfNames[64] = "페코페코_기사";
  imfNames[65] = "크루세이더";
  imfNames[66] = "몽크";
  imfNames[67] = "세이지";
  imfNames[68] = "로그";
  imfNames[69] = "연금술사";
  imfNames[70] = "바드";
  imfNames[71] = "무희";
  imfNames[72] = "신페코크루세이더";
  imfNames[73] = "초보자";
  imfNames[74] = "검사";
  imfNames[75] = "마법사";
  imfNames[76] = "궁수";
  imfNames[77] = "성직자";
  imfNames[78] = "상인";
  imfNames[79] = "도둑";
  imfNames[80] = "기사";
  imfNames[81] = "프리스트";
  imfNames[82] = "위저드";
  imfNames[83] = "제철공";
  imfNames[84] = "헌터";
  imfNames[85] = "어세신";
  imfNames[86] = "페코페코_기사";
  imfNames[87] = "크루세이더";
  imfNames[88] = "몽크";
  imfNames[89] = "세이지";
  imfNames[90] = "로그";
  imfNames[91] = "연금술사";
  imfNames[92] = "바드";
  imfNames[93] = "무희";
  imfNames[94] = "신페코크루세이더";
  imfNames[95] = "초보자";
  imfNames[96] = "태권소년";
  imfNames[97] = "태권소년";
  imfNames[98] = "태권소년";
  imfNames[99] = "위저드";
  imfNames[100] = "성직자";
  imfNames[101] = "기사";
  imfNames[102] = "세이지";
  imfNames[104] = "기사";
  imfNames[110] = "기사";
  imfNames[130] = "페코페코_기사";
  imfNames[131] = "페코페코_기사";
  imfNames[159] = "페코페코_기사";
  imfNames[138] = "페코페코_기사";
  imfNames[139] = "페코페코_기사";
  imfNames[140] = "페코페코_기사";
  imfNames[141] = "페코페코_기사";
  imfNames[142] = "페코페코_기사";
  imfNames[143] = "페코페코_기사";
  imfNames[144] = "페코페코_기사";
  imfNames[145] = "페코페코_기사";
  imfNames[107] = "프리스트";
  imfNames[113] = "프리스트";
  imfNames[105] = "워록";
  imfNames[111] = "워록";
  imfNames[108] = "마도기어";
  imfNames[114] = "마도기어";
  imfNames[136] = "마도기어";
  imfNames[137] = "마도기어";
  imfNames[162] = "마도기어";
  imfNames[106] = "헌터";
  imfNames[112] = "헌터";
  imfNames[134] = "레인져늑대";
  imfNames[135] = "레인져늑대";
  imfNames[161] = "레인져늑대";
  imfNames[109] = "어세신크로스";
  imfNames[115] = "어세신크로스";
  imfNames[123] = "가드";
  imfNames[116] = "크루세이더";
  imfNames[120] = "몽크";
  imfNames[127] = "몽크";
  imfNames[117] = "프로페서";
  imfNames[124] = "프로페서";
  imfNames[122] = "로그";
  imfNames[129] = "로그";
  imfNames[121] = "연금술사";
  imfNames[128] = "연금술사";
  imfNames[119] = "무희";
  imfNames[126] = "무희";
  imfNames[118] = "바드";
  imfNames[125] = "바드";
  imfNames[132] = "신페코크루세이더";
  imfNames[133] = "신페코크루세이더";
  imfNames[160] = "신페코크루세이더";
  imfNames[146] = "기사";
  imfNames[147] = "위저드";
  imfNames[148] = "헌터";
  imfNames[149] = "프리스트";
  imfNames[150] = "마도기어";
  imfNames[151] = "어세신크로스";
  imfNames[152] = "가드";
  imfNames[153] = "프로페서";
  imfNames[154] = "바드";
  imfNames[155] = "무희";
  imfNames[156] = "몽크";
  imfNames[157] = "연금술사";
  imfNames[158] = "로그";
  imfNames[240] = "초보자";
  imfNames[241] = "초보자";
  imfNames[242] = "슈퍼노비스포링";
  imfNames[243] = "슈퍼노비스포링";
  imfNames[244] = "프리스트알파카";
  imfNames[245] = "여우위저드";
  imfNames[246] = "제철공멧돼지";
  imfNames[247] = "마도기어";
  imfNames[255] = "마도기어";
  imfNames[248] = "타조레인져";
  imfNames[256] = "타조레인져";
  imfNames[249] = "사자기사";
  imfNames[257] = "사자기사";
  imfNames[250] = "사자로드나이트";
  imfNames[251] = "사자로얄가드";
  imfNames[258] = "사자로얄가드";
  imfNames[252] = "사자룬나이트";
  imfNames[259] = "사자룬나이트";
  imfNames[253] = "사자크루세이더";
  imfNames[260] = "사자크루세이더";
  imfNames[254] = "사자팔라딘";
  imfNames[261] = "닌자";
  imfNames[262] = "닌자";
  imfNames[263] = "frog_kagerou";
  imfNames[264] = "frog_oboro";
  imfNames[265] = "rebellion";
  imfNames[266] = "peco_rebellion";
  imfNames[268] = "summoner";
  imfNames[269] = "cart_summoner";
  imfNames[270] = "summoner";
  imfNames[271] = "cart_summoner";
  imfNames[272] = "닌자";
  imfNames[280] = "두꺼비닌자";
  imfNames[273] = "닌자";
  imfNames[281] = "frog_kagerou";
  imfNames[274] = "닌자";
  imfNames[282] = "frog_oboro";
  imfNames[275] = "태권소년";
  imfNames[283] = "태권소년포링";
  imfNames[276] = "태권소년";
  imfNames[288] = "태권소년";
  imfNames[284] = "권성포링";
  imfNames[277] = "위저드";
  imfNames[285] = "두꺼비소울링커";
  imfNames[278] = "건너";
  imfNames[286] = "페코건너";
  imfNames[279] = "rebellion";
  imfNames[287] = "peco_rebellion";
  imfNames[289] = "태권소년";
  imfNames[290] = "위저드";
  imfNames[291] = "태권소년";
  imfNames[292] = "위저드";
  imfNames[293] = "태권소년";
  imfNames[294] = "태권소년";
  imfNames[295] = "태권소년";
  imfNames[296] = "태권소년";
  imfNames[297] = "태권소년";
  imfNames[298] = "태권소년";
  imfNames[302] = "기사";
  imfNames[308] = "크루세이더";
  imfNames[305] = "워록";
  imfNames[318] = "워록";
  imfNames[311] = "프로페서";
  imfNames[324] = "프로페서";
  imfNames[307] = "WINDHAWK";
  imfNames[328] = "WOLF_WINDHAWK";
  imfNames[320] = "WINDHAWK_RIDING";
  imfNames[313] = "TROUBADOUR";
  imfNames[326] = "TROUBADOUR_RIDING";
  imfNames[314] = "TROUVERE";
  imfNames[327] = "TROUVERE_RIDING";
  imfNames[306] = "CARDINAL";
  imfNames[319] = "CARDINAL_RIDING";
  imfNames[312] = "INQUISITOR";
  imfNames[325] = "INQUISITOR_RIDING";
  imfNames[309] = "BIOLO";
  imfNames[322] = "BIOLO_RIDING";
  imfNames[303] = "MEISTER";
  imfNames[316] = "MEISTER_RIDING";
  imfNames[329] = "MEISTER_MADOGEAR1";
  imfNames[302] = "DRAGON_KNIGHT";
  imfNames[315] = "DRAGON_KNIGHT_RIDING";
  imfNames[330] = "DRAGON_KNIGHT_CHICKEN";
  imfNames[308] = "IMPERIAL_GUARD";
  imfNames[321] = "IMPERIAL_GUARD_RIDING";
  imfNames[331] = "IMPERIAL_GUARD_CHICKEN";
  imfNames[304] = "SHADOW_CROSS";
  imfNames[317] = "SHADOW_CROSS_RIDING";
  imfNames[310] = "ABYSS_CHASER";
  imfNames[323] = "ABYSS_CHASER_RIDING";
  imfNames[352] = "SKY_EMPEROR";
  imfNames[366] = "SKY_EMPEROR2";
  imfNames[359] = "SKY_EMPEROR_RIDING";
  imfNames[353] = "SOUL_ASCETIC";
  imfNames[360] = "SOUL_ASCETIC_RIDING";
  imfNames[354] = "SHINKIRO";
  imfNames[361] = "SHINKIRO_RIDING";
  imfNames[355] = "SHIRANUI";
  imfNames[362] = "SHIRANUI_RIDING";
  imfNames[356] = "NIGHT_WATCH";
  imfNames[363] = "NIGHT_WATCH_RIDING";
  imfNames[357] = "HYPER_NOVICE";
  imfNames[364] = "HYPER_NOVICE_RIDING";
  imfNames[358] = "SPIRIT_HANDLER";
  imfNames[365] = "SPIRIT_HANDLER_RIDING";
  imfNames[401] = "DRUID";
  imfNames[402] = "DRUID";
  imfNames[403] = "KARNOS";
  imfNames[404] = "KARNOS";
  imfNames[405] = "ALITEA";
  imfNames[406] = "WEREWOLF";
  imfNames[407] = "WERERAPTOR";
  imfNames[408] = "DRUID_RIDING";
  imfNames[409] = "KARNOS_RIDING";
  imfNames[410] = "ALITEA_RIDING";
  std::vector<const char*>& sexNames = reinterpret_cast<std::vector<const char*>&>(this->m_sexNames);
  sexNames.resize(2, "여");
  sexNames[0] = "여";
  sexNames[1] = "남";
  std::vector<const char*>& humanMaleHeadNames = reinterpret_cast<std::vector<const char*>&>(this->m_humanMaleHeadNames);
  humanMaleHeadNames.resize(46, "2");
  humanMaleHeadNames[0] = "2";
  humanMaleHeadNames[1] = "2";
  humanMaleHeadNames[2] = "1";
  humanMaleHeadNames[3] = "7";
  humanMaleHeadNames[4] = "5";
  humanMaleHeadNames[5] = "4";
  humanMaleHeadNames[6] = "3";
  humanMaleHeadNames[7] = "6";
  humanMaleHeadNames[8] = "8";
  humanMaleHeadNames[9] = "9";
  humanMaleHeadNames[10] = "10";
  humanMaleHeadNames[11] = "12";
  humanMaleHeadNames[12] = "11";
  humanMaleHeadNames[13] = "13";
  humanMaleHeadNames[14] = "14";
  humanMaleHeadNames[15] = "15";
  humanMaleHeadNames[16] = "16";
  humanMaleHeadNames[17] = "17";
  humanMaleHeadNames[18] = "18";
  humanMaleHeadNames[19] = "19";
  humanMaleHeadNames[20] = "20";
  humanMaleHeadNames[21] = "21";
  humanMaleHeadNames[22] = "22";
  humanMaleHeadNames[23] = "23";
  humanMaleHeadNames[24] = "24";
  humanMaleHeadNames[25] = "25";
  humanMaleHeadNames[26] = "26";
  humanMaleHeadNames[27] = "27";
  humanMaleHeadNames[28] = "28";
  humanMaleHeadNames[29] = "29";
  humanMaleHeadNames[30] = "30";
  humanMaleHeadNames[31] = "31";
  humanMaleHeadNames[32] = "32";
  humanMaleHeadNames[33] = "33";
  humanMaleHeadNames[34] = "34";
  humanMaleHeadNames[35] = "35";
  humanMaleHeadNames[36] = "36";
  humanMaleHeadNames[37] = "37";
  humanMaleHeadNames[38] = "38";
  humanMaleHeadNames[39] = "39";
  humanMaleHeadNames[40] = "40";
  humanMaleHeadNames[41] = "41";
  humanMaleHeadNames[42] = "42";
  humanMaleHeadNames[43] = "43";
  humanMaleHeadNames[44] = "44";
  humanMaleHeadNames[45] = "45";
  std::vector<const char*>& humanFemaleHeadNames = reinterpret_cast<std::vector<const char*>&>(this->m_humanFemaleHeadNames);
  humanFemaleHeadNames.resize(46, "2");
  humanFemaleHeadNames[0] = "2";
  humanFemaleHeadNames[1] = "2";
  humanFemaleHeadNames[2] = "4";
  humanFemaleHeadNames[3] = "7";
  humanFemaleHeadNames[4] = "1";
  humanFemaleHeadNames[5] = "5";
  humanFemaleHeadNames[6] = "3";
  humanFemaleHeadNames[7] = "6";
  humanFemaleHeadNames[8] = "12";
  humanFemaleHeadNames[9] = "10";
  humanFemaleHeadNames[10] = "9";
  humanFemaleHeadNames[11] = "11";
  humanFemaleHeadNames[12] = "8";
  humanFemaleHeadNames[13] = "13";
  humanFemaleHeadNames[14] = "14";
  humanFemaleHeadNames[15] = "15";
  humanFemaleHeadNames[16] = "16";
  humanFemaleHeadNames[17] = "17";
  humanFemaleHeadNames[18] = "18";
  humanFemaleHeadNames[19] = "19";
  humanFemaleHeadNames[20] = "20";
  humanFemaleHeadNames[21] = "21";
  humanFemaleHeadNames[22] = "22";
  humanFemaleHeadNames[23] = "23";
  humanFemaleHeadNames[24] = "24";
  humanFemaleHeadNames[25] = "25";
  humanFemaleHeadNames[26] = "26";
  humanFemaleHeadNames[27] = "27";
  humanFemaleHeadNames[28] = "28";
  humanFemaleHeadNames[29] = "29";
  humanFemaleHeadNames[30] = "30";
  humanFemaleHeadNames[31] = "31";
  humanFemaleHeadNames[32] = "32";
  humanFemaleHeadNames[33] = "33";
  humanFemaleHeadNames[34] = "34";
  humanFemaleHeadNames[35] = "35";
  humanFemaleHeadNames[36] = "36";
  humanFemaleHeadNames[37] = "37";
  humanFemaleHeadNames[38] = "38";
  humanFemaleHeadNames[39] = "39";
  humanFemaleHeadNames[40] = "40";
  humanFemaleHeadNames[41] = "41";
  humanFemaleHeadNames[42] = "42";
  humanFemaleHeadNames[43] = "43";
  humanFemaleHeadNames[44] = "44";
  humanFemaleHeadNames[45] = "45";
  std::vector<const char*>& doramMaleHeadNames = reinterpret_cast<std::vector<const char*>&>(this->m_doramMaleHeadNames);
  doramMaleHeadNames.resize(11, "1");
  doramMaleHeadNames[1] = "1";
  doramMaleHeadNames[2] = "2";
  doramMaleHeadNames[3] = "3";
  doramMaleHeadNames[4] = "4";
  doramMaleHeadNames[5] = "5";
  doramMaleHeadNames[6] = "6";
  doramMaleHeadNames[7] = "7";
  doramMaleHeadNames[8] = "8";
  doramMaleHeadNames[9] = "9";
  doramMaleHeadNames[10] = "10";
  std::vector<const char*>& doramFemaleHeadNames = reinterpret_cast<std::vector<const char*>&>(this->m_doramFemaleHeadNames);
  doramFemaleHeadNames.resize(11, "1");
  doramFemaleHeadNames[1] = "1";
  doramFemaleHeadNames[2] = "2";
  doramFemaleHeadNames[3] = "3";
  doramFemaleHeadNames[4] = "4";
  doramFemaleHeadNames[5] = "5";
  doramFemaleHeadNames[6] = "6";
  doramFemaleHeadNames[7] = "7";
  doramFemaleHeadNames[8] = "8";
  doramFemaleHeadNames[9] = "9";
  doramFemaleHeadNames[10] = "10";
  std::vector<const char*>& accessoryNames = this->m_accessoryNames;
  accessoryNames.resize(3000, "_병아리모자");
  accessoryNames[0] = "";
  for (int index = 1; index < 3000; ++index) {
    const char* name = nullptr;
    if (this->m_hotkeyScript->Call(std::string("ReqAccName", 10), "d>s", index, &name)) {
      accessoryNames[index] = name;
    }
  }
  std::vector<const char*>& shieldJobNames = reinterpret_cast<std::vector<const char*>&>(this->m_shieldJobNames);
  {
    const std::uint8_t source[] = {0xDE, 0x0D, 0xF1, 0x0D, 0x1D, 0xAD, 0x15, 0xE0, 0xD7, 0x7D, 0xF3, 0x75, 0x07, 0x77};
    thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
    shieldJobNames.resize(28053, name.Text());
  }
  {
    const std::uint8_t source[] = {0x06, 0xE7, 0x91, 0x87, 0x49, 0xC7, 0x31, 0x2C, 0x0F, 0x97, 0x93, 0xFF, 0x53, 0x1D};
    thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
    shieldJobNames[0] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xAD, 0xCE, 0x28, 0xCE, 0x2D, 0x93, 0x9C, 0x5E, 0xFA, 0x05};
    thread_local EncryptedName<sizeof(source), 0xE55723712993051DULL> name(source);
    shieldJobNames[1] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xF7, 0xD1, 0x54, 0x78, 0xB2, 0xD0, 0x63, 0xC1, 0xF9, 0xDE, 0x10, 0x3E, 0xEE, 0x37};
    thread_local EncryptedName<sizeof(source), 0x793F370985ED674FULL> name(source);
    shieldJobNames[2] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xD2, 0x36, 0x5D, 0x89, 0x67, 0x94, 0xDE, 0x77, 0x95, 0xF5};
    thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> name(source);
    shieldJobNames[3] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x75, 0xA9, 0x38, 0x84, 0x39, 0x03, 0x49, 0xCD, 0x73, 0xD2, 0x0E, 0xB3, 0x23, 0xD9};
    thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> name(source);
    shieldJobNames[4] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x5A, 0x6A, 0x47, 0x81, 0xDD, 0x78, 0xD2, 0xB3, 0x2F, 0x99};
    thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
    shieldJobNames[5] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xFA, 0x14, 0x7A, 0xFA, 0x3F, 0x62, 0xF4, 0xCC, 0x80, 0xA1};
    thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
    shieldJobNames[6] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xCE, 0x37, 0x14, 0xBC, 0x37, 0x00, 0xF7, 0xB4, 0x98, 0xD5};
    thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
    shieldJobNames[7] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE4, 0x30, 0x6F, 0xB9, 0x6A, 0xF3, 0x25, 0xD3, 0x7F, 0x36, 0x16, 0xAF, 0x79, 0xF4, 0x59, 0xBB, 0x8D, 0xF1};
    thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
    shieldJobNames[8] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9D, 0xAA, 0x89, 0x73, 0x14, 0x98, 0xB1, 0x5F, 0xFA, 0xCD, 0xB3, 0x3C, 0x44, 0x7D};
    thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
    shieldJobNames[9] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xA4, 0xF3, 0x0A, 0xC9, 0xBD, 0x17, 0x73, 0x84, 0xC3, 0x96, 0x7F, 0xCF, 0xF5, 0xEF};
    thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
    shieldJobNames[10] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xAA, 0xFE, 0x62, 0xDC, 0x2B, 0xA8, 0x2E, 0x36, 0xA0, 0x1B};
    thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
    shieldJobNames[11] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x0D, 0x89, 0xE5, 0x33, 0x62, 0xE2, 0xC7, 0x3D, 0x5D, 0xDB, 0xE5, 0x32, 0x1A, 0x27};
    thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
    shieldJobNames[12] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xD1, 0xFF, 0xE9, 0x79, 0x7B, 0x0F, 0x85, 0xD5, 0x48, 0xAA, 0xCF, 0x18, 0x5A, 0xB7, 0x87, 0xEB, 0xD3, 0xC1, 0xEB, 0x47, 0x79, 0x31, 0x1E, 0xBE, 0xF5, 0xA0, 0xCA, 0xA3};
    thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
    shieldJobNames[13] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x28, 0xDC, 0xD6, 0xC4, 0x0B, 0x5F, 0x91, 0xE1, 0x59, 0x80, 0x3D, 0xE6, 0x1E, 0x54, 0xB6, 0x91, 0x51, 0xB5, 0xAD, 0x97, 0x42, 0xE3};
    thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> name(source);
    shieldJobNames[14] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xDD, 0x0A, 0x12, 0xF4, 0xD1, 0xAB, 0x96, 0xD2, 0xCC, 0xF3};
    thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
    shieldJobNames[15] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x7D, 0xFB, 0x05, 0xDB, 0x20, 0x6F, 0x89, 0x47, 0x7D, 0x87, 0x09, 0xD6, 0x17, 0x99};
    thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
    shieldJobNames[16] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x52, 0xB5, 0x82, 0xFE, 0x11, 0xD2, 0xE3, 0xA8, 0x32, 0x7B};
    thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
    shieldJobNames[17] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x5C, 0x79, 0xE2, 0x2A, 0x19, 0xFD, 0x58, 0x54, 0xBF, 0x6A, 0xFF, 0x46, 0x78, 0xBB, 0x19, 0x08, 0x04, 0xD5};
    thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
    shieldJobNames[18] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x64, 0x8C, 0xDA, 0x4A, 0xB5, 0xAC, 0x30, 0xCC, 0x38, 0x55};
    thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
    shieldJobNames[19] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xB8, 0xDE, 0x35, 0x28, 0x73, 0x04, 0x04, 0x7F, 0xF0, 0x75};
    thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
    shieldJobNames[20] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xEC, 0x68, 0x3B, 0xCF, 0x8B, 0xE9, 0x1A, 0xC2, 0xE6, 0x4A, 0x41, 0x97, 0x8F, 0xFF, 0x6B, 0x9E, 0x0D, 0x10, 0x38, 0xED, 0xAB, 0xF7, 0x05, 0xAE, 0xF8, 0x1A, 0x1A, 0x97, 0xF3, 0xF3, 0x13, 0xDF, 0xA4, 0xAD};
    thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
    shieldJobNames[21] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x82, 0xF7, 0xED, 0xCD, 0xF9, 0x45, 0x11, 0xA4, 0x8B, 0x87, 0xEF, 0xB5, 0xE3, 0x9F};
    thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
    shieldJobNames[23] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9B, 0x36, 0x81, 0x8A, 0x53, 0x61, 0xC2, 0xC3, 0x8E, 0xD7};
    thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
    shieldJobNames[22] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x7F, 0x3A, 0x19, 0xB0, 0xC5, 0x9F, 0x8E, 0xDB, 0x6A, 0xDB};
    thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
    shieldJobNames[26] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xF9, 0x8C, 0xB7, 0x46, 0x9B, 0xB1, 0xA8, 0xD9, 0xEC, 0x6D};
    thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
    shieldJobNames[27] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xC1, 0x62, 0xA3, 0xAE, 0x4D, 0xBF, 0xBC, 0xB5, 0xD4, 0x83};
    thread_local EncryptedName<sizeof(source), 0x7D5D0F110B6B8371ULL> name(source);
    shieldJobNames[28] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x8D, 0xA0, 0x43, 0x6C, 0xA5, 0xAB, 0x24, 0x65, 0x98, 0x41};
    thread_local EncryptedName<sizeof(source), 0xADC51BF9C98B413DULL> name(source);
    shieldJobNames[29] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE1, 0x14, 0x07, 0xEC, 0xA3, 0xA7, 0x60, 0x17, 0xF4, 0xF5};
    thread_local EncryptedName<sizeof(source), 0xDF8117FF49CFF551ULL> name(source);
    shieldJobNames[30] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x5F, 0xEA, 0xE4, 0xC7, 0x99, 0x61, 0x24, 0x34, 0x25, 0x2D};
    thread_local EncryptedName<sizeof(source), 0x87E3D1C50D572DEFULL> name(source);
    shieldJobNames[24] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x39, 0x6C, 0x7B, 0xB9, 0xEB, 0x8D, 0x6A, 0x91, 0x57, 0xBD};
    thread_local EncryptedName<sizeof(source), 0x51BB39B763BBBD8DULL> name(source);
    shieldJobNames[25] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xED, 0x56, 0xC3, 0xA7, 0x09, 0x7F, 0xF6, 0x1B, 0x83, 0x87};
    thread_local EncryptedName<sizeof(source), 0xDB27CB557D038759ULL> name(source);
    shieldJobNames[164] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x3F, 0x44, 0x9D, 0xF9, 0x12, 0x4A, 0xFF, 0xFF, 0x58, 0x23, 0xA7, 0xB6, 0x42, 0xAF};
    thread_local EncryptedName<sizeof(source), 0x3FA3AFA7035DE3FFULL> name(source);
    shieldJobNames[167] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xBF, 0x88, 0xF6, 0x65, 0xC1, 0xF5, 0x5E, 0xB0, 0xC5, 0x4F};
    thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
    shieldJobNames[165] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x1B, 0x64, 0x36, 0x6A, 0xEF, 0x3D, 0x6C, 0x6E, 0x4C, 0xAF};
    thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
    shieldJobNames[166] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x14, 0x9F, 0xE8, 0xFF, 0x57, 0x5F, 0xCB, 0x64, 0x73, 0xFA, 0x9D, 0xF9, 0x1F, 0xA7};
    thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
    shieldJobNames[168] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xC8, 0x1C, 0x7B, 0x1F, 0x89, 0x58, 0x58, 0x3F, 0xBD, 0xEF};
    thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
    shieldJobNames[169] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x30, 0xF5, 0xD0, 0x36, 0x09, 0xE3, 0xB8, 0x20, 0xD3, 0xE6, 0xCD, 0x5A, 0x68, 0xA5, 0xF9, 0x7C, 0x68, 0x59};
    thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
    shieldJobNames[170] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x68, 0x39, 0xAE, 0x46, 0xED, 0xBF, 0x3C, 0xEC, 0x8B, 0x2A, 0xB3, 0x2A, 0x8C, 0xF9, 0x7D, 0xB0, 0x30, 0x95};
    thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
    shieldJobNames[171] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xA6, 0x92, 0x5D, 0xB7, 0x65, 0x72, 0x40, 0x3B, 0xE1, 0x51};
    thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
    shieldJobNames[172] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x5A, 0x1F, 0x32, 0xD8, 0xCB, 0x97, 0xA8, 0xC5, 0xC3, 0x18, 0x41, 0xAE, 0xB0, 0xF9, 0xC9, 0x94, 0x7D, 0xDD};
    thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
    shieldJobNames[173] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x22, 0x49, 0x6B, 0x55, 0xD1, 0x5B, 0xE7, 0x02, 0x2B, 0x39, 0x69, 0x2D, 0xCB, 0x81};
    thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
    shieldJobNames[174] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x33, 0x64, 0xB2, 0x24, 0xBD, 0x65, 0x9E, 0xD2, 0x22, 0x9D};
    thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
    shieldJobNames[175] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x8D, 0x43, 0x12, 0x0E, 0xDD, 0x95, 0x97, 0xBD, 0x8B, 0x38, 0x24, 0x39, 0xC7, 0x4F};
    thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
    shieldJobNames[176] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x2F, 0x30, 0x0C, 0x32, 0x91, 0x03, 0x48, 0x00, 0x3E, 0xC9};
    thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
    shieldJobNames[177] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x76, 0x45, 0xFB, 0x9D, 0xD3, 0x27, 0x73, 0xBA, 0x7F, 0x35, 0xF9, 0xE5, 0xC9, 0xFD};
    thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
    shieldJobNames[178] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xF4, 0xA2, 0xED, 0x5F, 0x94, 0x8F, 0x15, 0xE7, 0x6F, 0xA4, 0x94, 0x49, 0x87, 0x88, 0x69, 0x8F, 0x9D, 0x63};
    thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
    shieldJobNames[179] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE1, 0x33, 0x3E, 0x84, 0x4C, 0xC8, 0x63, 0x5B, 0xEF, 0x3C, 0x7A, 0xC2, 0x10, 0x2F};
    thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
    shieldJobNames[180] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x03, 0xD5, 0x23, 0x01, 0x44, 0x71, 0x03, 0x4D, 0x03, 0xA9, 0x2F, 0x0C, 0x73, 0x87};
    thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
    shieldJobNames[181] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xBD, 0xB1, 0x87, 0x91, 0x2C, 0x7B, 0xA9, 0x3D, 0xBD, 0xCD, 0x8B, 0x9C, 0x1B, 0x8D};
    thread_local EncryptedName<sizeof(source), 0x81F58DED5D470D01ULL> name(source);
    shieldJobNames[182] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x0B, 0x4A, 0x73, 0xD1, 0x32, 0xD4, 0x11, 0x85, 0x6C, 0x2D, 0x49, 0x9E, 0x62, 0x31};
    thread_local EncryptedName<sizeof(source), 0x454D31872BB3EDCBULL> name(source);
    shieldJobNames[183] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x51, 0xCC, 0xAB, 0xE5, 0x36, 0x2E, 0x93, 0x81, 0x36, 0xAB, 0x91, 0xAA, 0x66, 0xCB};
    thread_local EncryptedName<sizeof(source), 0x41CFCB831F6B6B91ULL> name(source);
    shieldJobNames[184] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xC9, 0x83, 0xA1, 0x67, 0xEE, 0x87, 0xBB, 0xE1, 0xC9, 0xFF, 0xAD, 0x6A, 0xD9, 0x71};
    thread_local EncryptedName<sizeof(source), 0x5DE7712FAB613F75ULL> name(source);
    shieldJobNames[185] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xD7, 0x7E, 0xF3, 0x53, 0xCC, 0x02, 0xCB, 0xCF, 0xB0, 0x19, 0xC9, 0x1C, 0x9C, 0xE7};
    thread_local EncryptedName<sizeof(source), 0x0F97E779A933D917ULL> name(source);
    shieldJobNames[186] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xFE, 0x7D, 0x50, 0x3E, 0xD7, 0xB3, 0x32, 0xD0, 0x1D, 0x6E, 0x4D, 0x52, 0xB6, 0xF5, 0x73, 0x8C, 0xA6, 0xD1};
    thread_local EncryptedName<sizeof(source), 0x3789496BE3E1D141ULL> name(source);
    shieldJobNames[187] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x2A, 0xF9, 0xCA, 0x85, 0xCF, 0x59, 0x6D, 0x82, 0x4D, 0x9C, 0xBF, 0x83, 0x87, 0xA1};
    thread_local EncryptedName<sizeof(source), 0x4331A17F33095FEBULL> name(source);
    shieldJobNames[188] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xCB, 0xDE, 0x88, 0xE4, 0xA5, 0xD7, 0xFC, 0x14, 0xDA, 0x27};
    thread_local EncryptedName<sizeof(source), 0xD1056FF94D4D2773ULL> name(source);
    shieldJobNames[189] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xEF, 0x19, 0xE7, 0xCB, 0x20, 0x64, 0x7B, 0x0F, 0xBF, 0x4B, 0xE7, 0xCA, 0x58, 0xA1};
    thread_local EncryptedName<sizeof(source), 0xB127A19D775BF751ULL> name(source);
    shieldJobNames[190] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE8, 0x5E, 0xC6, 0x86, 0xC7, 0xFC, 0xD4, 0xBA, 0x92, 0xEB};
    thread_local EncryptedName<sizeof(source), 0x0F61499B4973EB5DULL> name(source);
    shieldJobNames[191] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xDC, 0x85, 0x0A, 0xD6, 0xDB, 0xD8, 0x67, 0x06, 0xBC, 0x4B};
    thread_local EncryptedName<sizeof(source), 0xB7A96F8701BB4B6BULL> name(source);
    shieldJobNames[192] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x7E, 0xCF, 0x5C, 0x38, 0xBB, 0xDE, 0xD1, 0x12, 0x1E, 0x01};
    thread_local EncryptedName<sizeof(source), 0xA31F69E7EFED01C9ULL> name(source);
    shieldJobNames[193] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xEE, 0xB1, 0x16, 0x76, 0x4B, 0x72, 0x3B, 0x0E, 0x8E, 0x7F};
    thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
    shieldJobNames[194] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x83, 0x93, 0xE9, 0x9D, 0x7C, 0xB0, 0xC5, 0xDB, 0xD3, 0xC1, 0xE9, 0x9C, 0x04, 0x75};
    thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
    shieldJobNames[195] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xC1, 0xB5, 0xC9, 0x19, 0x1A, 0x30, 0xBD, 0x3B, 0x91, 0xE7, 0xC9, 0x18, 0x62, 0xF5};
    thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
    shieldJobNames[196] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9E, 0x0E, 0x5B, 0x6E, 0x5D, 0xEA, 0xCA, 0x65, 0xD6, 0xA5};
    thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
    shieldJobNames[197] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xEC, 0xE2, 0x34, 0x12, 0x11, 0x52, 0x74, 0x18, 0xB0, 0x3B};
    thread_local EncryptedName<sizeof(source), 0xADADEB4DF7813B55ULL> name(source);
    shieldJobNames[198] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x78, 0x40, 0x78, 0x00, 0xDD, 0x94, 0x9C, 0x6C, 0x24, 0x99};
    thread_local EncryptedName<sizeof(source), 0xD9452D81E5CD99C1ULL> name(source);
    shieldJobNames[199] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xAA, 0x5A, 0x10, 0xEA, 0xB7, 0x8E, 0x42, 0x12, 0xA0, 0xBF};
    thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> name(source);
    shieldJobNames[200] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xE0, 0xA6, 0x03, 0x5E, 0x21, 0xDA, 0x3E, 0x79, 0xA8, 0x0D};
    thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> name(source);
    shieldJobNames[201] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x6A, 0x92, 0xA7, 0xE0, 0xF5, 0xA6, 0x36, 0x79, 0x22, 0x39};
    thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> name(source);
    shieldJobNames[202] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x16, 0xD0, 0xBC, 0x02, 0x9B, 0x70, 0xB0, 0x20, 0x4A, 0x09};
    thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> name(source);
    shieldJobNames[203] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9C, 0x52, 0xA4, 0x94, 0x13, 0x96, 0xF0, 0x32, 0x96, 0xB7};
    thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> name(source);
    shieldJobNames[204] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x5C, 0xFB, 0x4C, 0xF6, 0xE9, 0x69, 0x22, 0xA3, 0xC5, 0xFC, 0x3F, 0x80, 0x92, 0x07, 0x43, 0xF2, 0x7B, 0x39};
    thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> name(source);
    shieldJobNames[205] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xFC, 0x26, 0xB9, 0xB5, 0x7C, 0x1B, 0xED, 0x13, 0x67, 0x20, 0xC0, 0xA3, 0x6F, 0x1C, 0x91, 0x7B, 0x95, 0xE7};
    thread_local EncryptedName<sizeof(source), 0xBD2BA1C11B01E73BULL> name(source);
    shieldJobNames[206] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xD6, 0x22, 0xEB, 0x37, 0x0C, 0xE7, 0xB3, 0xE5, 0x4D, 0x24, 0x92, 0x21, 0x1F, 0xE0, 0xCF, 0x8D, 0xBF, 0xE3};
    thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> name(source);
    shieldJobNames[207] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x80, 0xF3, 0xE1, 0xD3, 0xA7, 0xF7, 0x2D, 0x0A, 0x89, 0x83, 0xE3, 0xAB, 0xBD, 0x2D};
    thread_local EncryptedName<sizeof(source), 0xC9712D676B5B3943ULL> name(source);
    shieldJobNames[208] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x09, 0xE6, 0x86, 0xD4, 0xD1, 0x23, 0xDE, 0x56, 0x5E, 0x2D};
    thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
    shieldJobNames[209] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xB5, 0xD5, 0x34, 0xF4, 0xFE, 0xB4, 0xD3, 0x61, 0xBB, 0xDA, 0x70, 0xB2, 0xA2, 0x53};
    thread_local EncryptedName<sizeof(source), 0xD98F5345098D630DULL> name(source);
    shieldJobNames[210] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x22, 0xB6, 0x63, 0x19, 0x11, 0x92, 0xCC, 0x27, 0x65, 0x75};
    thread_local EncryptedName<sizeof(source), 0x9B0F234DEFDF7593ULL> name(source);
    shieldJobNames[211] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x31, 0x3B, 0xD0, 0x04, 0x73, 0x73, 0x35, 0x41, 0x37, 0x40, 0xE6, 0x33, 0x69, 0xA9};
    thread_local EncryptedName<sizeof(source), 0xFD69A9B3F311818DULL> name(source);
    shieldJobNames[212] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x90, 0xEC, 0x29, 0x65, 0x9D, 0x44, 0xDE, 0xBF, 0xE5, 0x1F};
    thread_local EncryptedName<sizeof(source), 0x7F2DFFC1ABE91F2BULL> name(source);
    shieldJobNames[213] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x3A, 0x24, 0xB6, 0x8E, 0x7B, 0x96, 0xF8, 0xDA, 0x30, 0xC1};
    thread_local EncryptedName<sizeof(source), 0x1F1D51274373C1FDULL> name(source);
    shieldJobNames[214] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x2D, 0x75, 0x99, 0x65, 0x98, 0xE8, 0xF1, 0xE3, 0x7D, 0x27, 0x99, 0x64, 0xE0, 0x2D};
    thread_local EncryptedName<sizeof(source), 0x5DAD2D25D9259B93ULL> name(source);
    shieldJobNames[215] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x91, 0x36, 0xEC, 0xAE, 0xBF, 0x91, 0x0E, 0xAE, 0x80, 0xCF};
    thread_local EncryptedName<sizeof(source), 0x6BF729E30729CF29ULL> name(source);
    shieldJobNames[216] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9B, 0x2F, 0xBF, 0xC3, 0x9A, 0xE9, 0x6B, 0x81, 0x9B, 0x53, 0xB3, 0xCE, 0xAD, 0x1F};
    thread_local EncryptedName<sizeof(source), 0x3D371F5B0F7F9327ULL> name(source);
    shieldJobNames[217] = name.Text();
  }
  {
    const std::uint8_t source[] = {0xAC, 0x07, 0x70, 0x38, 0x8D, 0xCA, 0xC1, 0xD0, 0xCC, 0xC9};
    thread_local EncryptedName<sizeof(source), 0x610F7DD1EFC1C91BULL> name(source);
    shieldJobNames[218] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x36, 0xC7, 0x58, 0x42, 0xF7, 0xA5, 0x22, 0x22, 0xD5, 0xD4, 0x45, 0x2E, 0x96, 0xE3, 0x63, 0x7E, 0x6E, 0x6B};
    thread_local EncryptedName<sizeof(source), 0xC5995F4B9FE96B89ULL> name(source);
    shieldJobNames[219] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x3E, 0x7E, 0xF0, 0x96, 0x49, 0x30, 0xAA, 0xBC, 0x62, 0xA7};
    thread_local EncryptedName<sizeof(source), 0x097389157345A787ULL> name(source);
    shieldJobNames[220] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x8E, 0xDE, 0x65, 0x56, 0x9F, 0x18, 0x38, 0x97, 0xC6, 0x75};
    thread_local EncryptedName<sizeof(source), 0x5F93A1C3A7AD7537ULL> name(source);
    shieldJobNames[221] = name.Text();
  }
  {
    const std::uint8_t source[] = {0x9C, 0xB3, 0x1B, 0x5F, 0x31, 0x17, 0x03, 0x44, 0x95, 0xC3, 0x19, 0x27, 0x2B, 0xCD};
    thread_local EncryptedName<sizeof(source), 0x875FCDF1E7A1795FULL> name(source);
    shieldJobNames[222] = name.Text();
  }
  shieldJobNames[223] = WizardBodyPath1().Text();
  shieldJobNames[224] = PriestBodyPath1().Text();
  shieldJobNames[225] = AssassinBodyPath1().Text();
  shieldJobNames[226] = SageBodyPath1().Text();
  shieldJobNames[227] = BardBodyPath1().Text();
  shieldJobNames[228] = DancerBodyPath1().Text();
  shieldJobNames[229] = MonkBodyPath1().Text();
  shieldJobNames[230] = AlchemistBodyPath1().Text();
  shieldJobNames[232] = RogueBodyPath1().Text();
  shieldJobNames[233] = NoviceBodyPath1().Text();
  shieldJobNames[234] = SwordmanBodyPath1().Text();
  shieldJobNames[235] = MagicianBodyPath1().Text();
  shieldJobNames[236] = ArcherBodyPath1().Text();
  shieldJobNames[237] = AcolyteBodyPath1().Text();
  shieldJobNames[238] = MerchantBodyPath1().Text();
  shieldJobNames[239] = ThiefBodyPath1().Text();
  shieldJobNames[231] = ThiefBodyPath2().Text();
  shieldJobNames[51] = NoviceBodyPath2().Text();
  shieldJobNames[52] = SwordmanBodyPath2().Text();
  shieldJobNames[53] = MagicianBodyPath2().Text();
  shieldJobNames[54] = ArcherBodyPath2().Text();
  shieldJobNames[55] = AcolyteBodyPath2().Text();
  shieldJobNames[56] = MerchantBodyPath2().Text();
  shieldJobNames[57] = ThiefBodyPath3().Text();
  shieldJobNames[58] = KnightBodyPath1().Text();
  shieldJobNames[59] = PriestBodyPath2().Text();
  shieldJobNames[60] = WizardBodyPath2().Text();
  shieldJobNames[61] = BlacksmithBodyPath1().Text();
  shieldJobNames[62] = HunterBodyPath1().Text();
  shieldJobNames[63] = AssassinBodyPath2().Text();
  shieldJobNames[64] = PecoKnightBodyPath1().Text();
  shieldJobNames[65] = CrusaderBodyPath1().Text();
  shieldJobNames[66] = MonkBodyPath2().Text();
  shieldJobNames[67] = SageBodyPath2().Text();
  shieldJobNames[68] = RogueBodyPath2().Text();
  shieldJobNames[69] = AlchemistBodyPath2().Text();
  shieldJobNames[70] = BardBodyPath2().Text();
  shieldJobNames[71] = DancerBodyPath2().Text();
  shieldJobNames[72] = NewPecoCrusaderBodyPath1().Text();
  shieldJobNames[73] = NoviceBodyPath3().Text();
  shieldJobNames[74] = SwordmanBodyPath3().Text();
  shieldJobNames[75] = MagicianBodyPath3().Text();
  shieldJobNames[76] = ArcherBodyPath3().Text();
  {
    const std::uint8_t source[] = {0xEB, 0x55, 0x10, 0x42, 0x6B, 0xA5, 0xA1, 0xE3, 0xED, 0x2E, 0x26, 0x75, 0x71, 0x7F};
    thread_local EncryptedName<sizeof(source), 0x5FFD7FABB5D1EF57ULL> name(source);
    shieldJobNames[77] = name.Text();
  }
  shieldJobNames[78] = MerchantBodyPath3().Text();
  shieldJobNames[79] = ThiefBodyPath4().Text();
  shieldJobNames[80] = KnightBodyPath2().Text();
  shieldJobNames[81] = PriestBodyPath3().Text();
  shieldJobNames[82] = WizardBodyPath3().Text();
  shieldJobNames[83] = BlacksmithBodyPath2().Text();
  shieldJobNames[84] = HunterBodyPath2().Text();
  shieldJobNames[85] = AssassinBodyPath3().Text();
  shieldJobNames[86] = PecoKnightBodyPath2().Text();
  shieldJobNames[87] = CrusaderBodyPath2().Text();
  shieldJobNames[88] = MonkBodyPath3().Text();
  shieldJobNames[89] = SageBodyPath3().Text();
  shieldJobNames[90] = RogueBodyPath3().Text();
  shieldJobNames[91] = AlchemistBodyPath3().Text();
  shieldJobNames[92] = BardBodyPath3().Text();
  shieldJobNames[93] = DancerBodyPath3().Text();
  shieldJobNames[94] = OldPecoCrusaderBodyPath1().Text();
  shieldJobNames[95] = NoviceBodyPath4().Text();
  shieldJobNames[96] = TaekwonBodyPath1().Text();
  shieldJobNames[97] = TaekwonBodyPath2().Text();
  shieldJobNames[98] = TaekwonBodyPath3().Text();
  shieldJobNames[99] = WizardBodyPath4().Text();
  shieldJobNames[100] = AcolyteBodyPath3().Text();
  shieldJobNames[101] = KnightBodyPath3().Text();
  shieldJobNames[102] = SageBodyPath4().Text();
  shieldJobNames[104] = KnightBodyPath4().Text();
  shieldJobNames[110] = KnightBodyPath5().Text();
  shieldJobNames[130] = PecoKnightBodyPath3().Text();
  shieldJobNames[131] = PecoKnightBodyPath4().Text();
  shieldJobNames[159] = PecoKnightBodyPath5().Text();
  shieldJobNames[138] = PecoKnightBodyPath6().Text();
  shieldJobNames[139] = PecoKnightBodyPath7().Text();
  shieldJobNames[140] = PecoKnightBodyPath8().Text();
  shieldJobNames[141] = PecoKnightBodyPath9().Text();
  shieldJobNames[142] = PecoKnightBodyPath10().Text();
  shieldJobNames[143] = PecoKnightBodyPath11().Text();
  shieldJobNames[144] = PecoKnightBodyPath12().Text();
  shieldJobNames[145] = PecoKnightBodyPath13().Text();
  shieldJobNames[107] = PriestBodyPath4().Text();
  shieldJobNames[113] = PriestBodyPath5().Text();
  shieldJobNames[105] = WizardBodyPath5().Text();
  shieldJobNames[111] = WizardBodyPath6().Text();
  shieldJobNames[108] = BlacksmithBodyPath3().Text();
  shieldJobNames[114] = BlacksmithBodyPath4().Text();
  shieldJobNames[136] = MadogearBodyPath1().Text();
  shieldJobNames[137] = MadogearBodyPath2().Text();
  shieldJobNames[162] = MadogearBodyPath3().Text();
  shieldJobNames[106] = HunterBodyPath3().Text();
  shieldJobNames[112] = HunterBodyPath4().Text();
  shieldJobNames[134] = RangerWolfBodyPath1().Text();
  shieldJobNames[135] = RangerWolfBodyPath2().Text();
  shieldJobNames[161] = RangerWolfBodyPath3().Text();
  shieldJobNames[109] = AssassinBodyPath4().Text();
  shieldJobNames[115] = AssassinBodyPath5().Text();
  shieldJobNames[116] = CrusaderBodyPath3().Text();
  shieldJobNames[123] = CrusaderBodyPath4().Text();
  shieldJobNames[132] = NewPecoCrusaderBodyPath2().Text();
  shieldJobNames[133] = NewPecoCrusaderBodyPath3().Text();
  shieldJobNames[160] = NewPecoCrusaderBodyPath4().Text();
  shieldJobNames[120] = MonkBodyPath4().Text();
  shieldJobNames[127] = MonkBodyPath5().Text();
  shieldJobNames[117] = SageBodyPath5().Text();
  shieldJobNames[124] = SageBodyPath6().Text();
  shieldJobNames[122] = RogueBodyPath4().Text();
  shieldJobNames[129] = RogueBodyPath5().Text();
  shieldJobNames[121] = AlchemistBodyPath4().Text();
  shieldJobNames[128] = AlchemistBodyPath5().Text();
  shieldJobNames[119] = DancerBodyPath4().Text();
  shieldJobNames[126] = DancerBodyPath5().Text();
  shieldJobNames[118] = BardBodyPath4().Text();
  shieldJobNames[125] = BardBodyPath5().Text();
  shieldJobNames[146] = KnightBodyPath6().Text();
  shieldJobNames[147] = WizardBodyPath7().Text();
  shieldJobNames[148] = HunterBodyPath5().Text();
  shieldJobNames[149] = PriestBodyPath6().Text();
  shieldJobNames[150] = BlacksmithBodyPath5().Text();
  shieldJobNames[151] = AssassinBodyPath6().Text();
  shieldJobNames[152] = CrusaderBodyPath5().Text();
  shieldJobNames[153] = SageBodyPath7().Text();
  shieldJobNames[154] = BardBodyPath6().Text();
  shieldJobNames[155] = DancerBodyPath6().Text();
  shieldJobNames[156] = MonkBodyPath6().Text();
  shieldJobNames[157] = AlchemistBodyPath6().Text();
  shieldJobNames[158] = RogueBodyPath6().Text();
  shieldJobNames[240] = NoviceBodyPath5().Text();
  shieldJobNames[241] = NoviceBodyPath6().Text();
  shieldJobNames[242] = NoviceBodyPath7().Text();
  shieldJobNames[243] = NoviceBodyPath8().Text();
  shieldJobNames[244] = PriestBodyPath7().Text();
  shieldJobNames[245] = WizardBodyPath8().Text();
  shieldJobNames[246] = BlacksmithBodyPath6().Text();
  shieldJobNames[247] = BlacksmithBodyPath7().Text();
  shieldJobNames[255] = BlacksmithBodyPath8().Text();
  shieldJobNames[248] = HunterBodyPath6().Text();
  shieldJobNames[256] = HunterBodyPath7().Text();
  shieldJobNames[249] = KnightBodyPath7().Text();
  shieldJobNames[257] = KnightBodyPath8().Text();
  shieldJobNames[250] = KnightBodyPath9().Text();
  shieldJobNames[251] = CrusaderBodyPath6().Text();
  shieldJobNames[258] = CrusaderBodyPath7().Text();
  shieldJobNames[252] = KnightBodyPath10().Text();
  shieldJobNames[259] = KnightBodyPath11().Text();
  shieldJobNames[253] = CrusaderBodyPath8().Text();
  shieldJobNames[260] = CrusaderBodyPath9().Text();
  shieldJobNames[254] = CrusaderBodyPath10().Text();
  shieldJobNames[261] = KagerouBodyPath1().Text();
  shieldJobNames[262] = OboroBodyPath1().Text();
  shieldJobNames[263] = KagerouBodyPath2().Text();
  shieldJobNames[264] = OboroBodyPath2().Text();
  shieldJobNames[265] = RebellionBodyPath1().Text();
  shieldJobNames[268] = SummonerBodyPath1().Text();
  shieldJobNames[269] = SummonerCartBodyPath1().Text();
  shieldJobNames[270] = SummonerBodyPath2().Text();
  shieldJobNames[271] = SummonerCartBodyPath2().Text();
  shieldJobNames[272] = NinjaBodyPath1().Text();
  shieldJobNames[280] = NinjaBodyPath2().Text();
  shieldJobNames[273] = KagerouBodyPath3().Text();
  shieldJobNames[274] = OboroBodyPath3().Text();
  shieldJobNames[281] = KagerouBodyPath4().Text();
  shieldJobNames[282] = OboroBodyPath4().Text();
  shieldJobNames[275] = TaekwonBodyPath4().Text();
  shieldJobNames[283] = TaekwonBodyPath5().Text();
  shieldJobNames[276] = TaekwonBodyPath6().Text();
  shieldJobNames[288] = TaekwonBodyPath7().Text();
  shieldJobNames[284] = TaekwonBodyPath8().Text();
  shieldJobNames[277] = WizardBodyPath9().Text();
  shieldJobNames[285] = WizardBodyPath10().Text();
  shieldJobNames[278] = GunslingerBodyPath1().Text();
  shieldJobNames[286] = GunslingerBodyPath2().Text();
  shieldJobNames[279] = RebellionBodyPath2().Text();
  shieldJobNames[287] = RebellionBodyPath3().Text();
  shieldJobNames[289] = TaekwonBodyPath9().Text();
  shieldJobNames[290] = WizardBodyPath11().Text();
  shieldJobNames[291] = TaekwonBodyPath10().Text();
  shieldJobNames[292] = WizardBodyPath12().Text();
  shieldJobNames[293] = TaekwonBodyPath11().Text();
  shieldJobNames[294] = TaekwonBodyPath12().Text();
  shieldJobNames[295] = TaekwonBodyPath13().Text();
  shieldJobNames[296] = WizardBodyPath13().Text();
  shieldJobNames[297] = TaekwonBodyPath14().Text();
  shieldJobNames[298] = WizardBodyPath14().Text();
  shieldJobNames[305] = WizardBodyPath15().Text();
  shieldJobNames[318] = WizardBodyPath16().Text();
  shieldJobNames[311] = SageBodyPath8().Text();
  shieldJobNames[324] = SageBodyPath9().Text();
  shieldJobNames[307] = WindhawkBodyPath1().Text();
  shieldJobNames[328] = WolfWindhawkBodyPath1().Text();
  shieldJobNames[320] = WindhawkRidingBodyPath1().Text();
  shieldJobNames[313] = TroubadourBodyPath1().Text();
  shieldJobNames[326] = TroubadourRidingBodyPath1().Text();
  shieldJobNames[314] = TrouvereBodyPath1().Text();
  shieldJobNames[327] = TrouvereRidingBodyPath1().Text();
  shieldJobNames[306] = CardinalBodyPath1().Text();
  shieldJobNames[319] = CardinalRidingBodyPath1().Text();
  shieldJobNames[312] = InquisitorBodyPath1().Text();
  shieldJobNames[325] = InquisitorRidingBodyPath1().Text();
  shieldJobNames[309] = BioloBodyPath1().Text();
  shieldJobNames[322] = BioloRidingBodyPath1().Text();
  shieldJobNames[303] = MeisterBodyPath1().Text();
  shieldJobNames[316] = MeisterRidingBodyPath1().Text();
  shieldJobNames[329] = MeisterMadogear1BodyPath1().Text();
  shieldJobNames[302] = DragonKnightBodyPath1().Text();
  shieldJobNames[315] = DragonKnightRidingBodyPath1().Text();
  shieldJobNames[330] = DragonKnightChickenBodyPath1().Text();
  shieldJobNames[308] = ImperialGuardBodyPath1().Text();
  shieldJobNames[321] = ImperialGuardRidingBodyPath1().Text();
  shieldJobNames[331] = ImperialGuardChickenBodyPath1().Text();
  shieldJobNames[304] = ShadowCrossBodyPath1().Text();
  shieldJobNames[317] = ShadowCrossRidingBodyPath1().Text();
  shieldJobNames[310] = AbyssChaserBodyPath1().Text();
  shieldJobNames[323] = AbyssChaserRidingBodyPath1().Text();
  shieldJobNames[352] = SkyEmperorBodyPath1().Text();
  shieldJobNames[366] = SkyEmperor2BodyPath1().Text();
  shieldJobNames[359] = SkyEmperorRidingBodyPath1().Text();
  shieldJobNames[353] = SoulAsceticBodyPath1().Text();
  shieldJobNames[360] = SoulAsceticRidingBodyPath1().Text();
  shieldJobNames[354] = ShinkiroBodyPath1().Text();
  shieldJobNames[361] = ShinkiroRidingBodyPath1().Text();
  shieldJobNames[355] = ShiranuiBodyPath1().Text();
  shieldJobNames[362] = ShiranuiRidingBodyPath1().Text();
  shieldJobNames[356] = NightWatchBodyPath1().Text();
  shieldJobNames[363] = NightWatchRidingBodyPath1().Text();
  shieldJobNames[357] = HyperNoviceBodyPath1().Text();
  shieldJobNames[364] = HyperNoviceRidingBodyPath1().Text();
  shieldJobNames[358] = SpiritHandlerBodyPath1().Text();
  shieldJobNames[365] = SpiritHandlerRidingBodyPath1().Text();
  shieldJobNames[401] = DruidBodyPath1().Text();
  shieldJobNames[402] = DruidBodyPath2().Text();
  shieldJobNames[403] = KarnosBodyPath1().Text();
  shieldJobNames[404] = KarnosBodyPath2().Text();
  shieldJobNames[405] = AliteaBodyPath1().Text();
  std::vector<const char*>& unknownNames = reinterpret_cast<std::vector<const char*>&>(this->unknown_0F88[0x08]);
  unknownNames.resize(2, "f");
  unknownNames[0] = "f";
  unknownNames[1] = "";
  std::vector<const char*>& weaponNames = reinterpret_cast<std::vector<const char*>&>(this->m_weaponNamesBegin);
  weaponNames.resize(105, "");
  for (int index = 0; index < 105; ++index) {
    const char* name = nullptr;
    if (this->m_hotkeyScript->Call(std::string("ReqWeaponName"), "d>s", index, &name)) {
      weaponNames[index] = name;
    }
  }
  std::vector<const char*>& palJobNames = this->m_palJobNames;
  palJobNames.resize(28053, "초보자");
  palJobNames[401] = DruidJobName1().Text();
  palJobNames[402] = DruidJobName2().Text();
  palJobNames[403] = KarnosJobName1().Text();
  palJobNames[404] = KarnosJobName2().Text();
  palJobNames[405] = AliteaJobName1().Text();
  palJobNames[406] = WerewolfJobName1().Text();
  palJobNames[407] = WereRaptorJobName1().Text();
  palJobNames[408] = DruidRidingName().Text();
  palJobNames[409] = KarnosRidingName().Text();
  palJobNames[410] = AliteaRidingName().Text();
  palJobNames[268] = "묘족";
  palJobNames[269] = "고양이카트";
  palJobNames[270] = "묘족";
  palJobNames[271] = "고양이카트";
  palJobNames[272] = "닌자";
  palJobNames[280] = "닌자";
  palJobNames[273] = "KAGEROU";
  palJobNames[274] = "OBORO";
  palJobNames[281] = "KAGEROU";
  palJobNames[282] = "OBORO";
  palJobNames[275] = "태권소년";
  palJobNames[283] = "태권소년";
  palJobNames[276] = "권성";
  palJobNames[288] = "권성";
  palJobNames[284] = "권성";
  palJobNames[277] = "소울링커";
  palJobNames[285] = "태권소년";
  palJobNames[278] = "건너";
  palJobNames[286] = "건너";
  palJobNames[279] = "리벨리온";
  palJobNames[287] = "리벨리온";
  palJobNames[14] = "크루";
  palJobNames[15] = "몽크";
  palJobNames[16] = "세이지";
  palJobNames[17] = "로그";
  palJobNames[18] = "연금술사";
  palJobNames[19] = "바드";
  palJobNames[20] = "댄서";
  palJobNames[21] = "페코페코_크루";
  palJobNames[87] = "크루";
  palJobNames[88] = "몽크";
  palJobNames[89] = "세이지";
  palJobNames[90] = "로그";
  palJobNames[91] = "연금술사";
  palJobNames[92] = "바드";
  palJobNames[93] = "댄서";
  palJobNames[94] = "페코페코_크루";
  palJobNames[24] = "건너";
  palJobNames[97] = "권성";
  palJobNames[98] = "권성";
  palJobNames[25] = "닌자";
  palJobNames[99] = "소울링커";
  palJobNames[58] = "로드나이트";
  palJobNames[64] = "페코로나";
  palJobNames[72] = "페코팔라";
  palJobNames[62] = "스나이퍼";
  palJobNames[68] = "스토커";
  palJobNames[63] = "어세신크로스";
  palJobNames[71] = "집시";
  palJobNames[66] = "챔피온";
  palJobNames[70] = "크라운";
  palJobNames[69] = "크리에이터";
  palJobNames[65] = "팔라딘";
  palJobNames[67] = "프로페서";
  palJobNames[60] = "하이위저드";
  palJobNames[59] = "하이프리스트";
  palJobNames[61] = "화이트스미스";
  palJobNames[14] = "기사";
  palJobNames[15] = "프리스트";
  palJobNames[16] = "위저드";
  palJobNames[17] = "어세신";
  palJobNames[18] = "제철공";
  palJobNames[19] = "헌터";
  palJobNames[20] = "헌터";
  palJobNames[21] = "페코페코_기사";
  palJobNames[87] = "기사";
  palJobNames[88] = "프리스트";
  palJobNames[89] = "위저드";
  palJobNames[90] = "어세신";
  palJobNames[91] = "제철공";
  palJobNames[92] = "헌터";
  palJobNames[93] = "헌터";
  palJobNames[94] = "페코페코_기사";
  palJobNames[24] = "초보자";
  palJobNames[97] = "태권소년";
  palJobNames[98] = "태권소년";
  palJobNames[25] = "초보자";
  palJobNames[99] = "태권소년";
  palJobNames[58] = "기사";
  palJobNames[64] = "페코페코_기사";
  palJobNames[72] = "페코페코_기사";
  palJobNames[62] = "헌터";
  palJobNames[68] = "기사";
  palJobNames[63] = "어세신";
  palJobNames[71] = "헌터";
  palJobNames[66] = "프리스트";
  palJobNames[70] = "헌터";
  palJobNames[69] = "제철공";
  palJobNames[65] = "기사";
  palJobNames[67] = "위저드";
  palJobNames[60] = "위저드";
  palJobNames[59] = "프리스트";
  palJobNames[61] = "제철공";
  palJobNames[0] = "초보자";
  palJobNames[51] = "초보자";
  palJobNames[73] = "초보자";
  palJobNames[23] = "슈퍼노비스";
  palJobNames[95] = "슈퍼노비스";
  palJobNames[240] = "슈퍼노비스";
  palJobNames[241] = "슈퍼노비스";
  palJobNames[96] = "태권소년";
  palJobNames[100] = "성직자";
  palJobNames[101] = "기사";
  palJobNames[102] = "위저드";
  palJobNames[265] = "리벨리온";
  palJobNames[1] = "검사";
  palJobNames[52] = "검사";
  palJobNames[74] = "검사";
  palJobNames[7] = "기사";
  palJobNames[80] = "기사";
  palJobNames[13] = "페코페코_기사";
  palJobNames[86] = "페코페코_기사";
  palJobNames[104] = "룬나이트";
  palJobNames[146] = "룬나이트";
  palJobNames[110] = "룬나이트";
  palJobNames[130] = "룬드래곤";
  palJobNames[159] = "룬드래곤";
  palJobNames[131] = "룬드래곤";
  palJobNames[138] = "룬드래곤";
  palJobNames[139] = "룬드래곤";
  palJobNames[140] = "룬드래곤";
  palJobNames[141] = "룬드래곤";
  palJobNames[142] = "룬드래곤";
  palJobNames[143] = "룬드래곤";
  palJobNames[144] = "룬드래곤";
  palJobNames[145] = "룬드래곤";
  palJobNames[116] = "로얄가드";
  palJobNames[152] = "로얄가드";
  palJobNames[123] = "로얄가드";
  palJobNames[132] = "그리폰로얄";
  palJobNames[160] = "그리폰로얄";
  palJobNames[133] = "그리폰로얄";
  palJobNames[2] = "마법사";
  palJobNames[53] = "마법사";
  palJobNames[117] = "소서러";
  palJobNames[124] = "소서러";
  palJobNames[153] = "소서러";
  palJobNames[105] = "워록";
  palJobNames[111] = "워록";
  palJobNames[147] = "워록";
  palJobNames[3] = "궁수";
  palJobNames[54] = "궁수";
  palJobNames[76] = "궁수";
  palJobNames[11] = "헌터";
  palJobNames[84] = "헌터";
  palJobNames[106] = "레인저";
  palJobNames[148] = "레인저";
  palJobNames[112] = "레인저";
  palJobNames[134] = "울프레인저";
  palJobNames[161] = "울프레인저";
  palJobNames[135] = "울프레인저";
  palJobNames[118] = "민스트럴";
  palJobNames[154] = "민스트럴";
  palJobNames[125] = "민스트럴";
  palJobNames[119] = "원더러";
  palJobNames[126] = "원더러";
  palJobNames[155] = "원더러";
  palJobNames[4] = "성직자";
  palJobNames[55] = "성직자";
  palJobNames[77] = "성직자";
  palJobNames[8] = "프리스트";
  palJobNames[81] = "프리스트";
  palJobNames[107] = "아크비숍";
  palJobNames[149] = "아크비숍";
  palJobNames[113] = "아크비숍";
  palJobNames[120] = "슈라";
  palJobNames[156] = "슈라";
  palJobNames[127] = "슈라";
  palJobNames[5] = "상인";
  palJobNames[56] = "상인";
  palJobNames[78] = "상인";
  palJobNames[10] = "제철공";
  palJobNames[83] = "제철공";
  palJobNames[121] = "제네릭";
  palJobNames[128] = "제네릭";
  palJobNames[157] = "제네릭";
  palJobNames[108] = "미케닉";
  palJobNames[114] = "미케닉";
  palJobNames[150] = "미케닉";
  palJobNames[136] = "미케닉_마도기어";
  palJobNames[137] = "미케닉_마도기어";
  palJobNames[162] = "미케닉_마도기어";
  palJobNames[6] = "도둑";
  palJobNames[57] = "도둑";
  palJobNames[79] = "도둑";
  palJobNames[12] = "어세신";
  palJobNames[85] = "어세신";
  palJobNames[122] = "쉐도우체이서";
  palJobNames[129] = "쉐도우체이서";
  palJobNames[158] = "쉐도우체이서";
  palJobNames[109] = "길로틴크로스";
  palJobNames[115] = "길로틴크로스";
  palJobNames[151] = "길로틴크로스";
  palJobNames[75] = "마법사";
  palJobNames[9] = "위저드";
  palJobNames[82] = "위저드";
  palJobNames[22] = "결혼";
  palJobNames[26] = "프리스트";
  palJobNames[27] = "프리스트";
  palJobNames[28] = "프리스트";
  palJobNames[29] = "프리스트";
  palJobNames[30] = "프리스트";
  palJobNames[174] = "초보자";
  palJobNames[208] = "초보자";
  palJobNames[233] = "초보자";
  palJobNames[178] = "슈퍼노비스";
  palJobNames[222] = "슈퍼노비스";
  palJobNames[242] = "슈퍼노비스";
  palJobNames[243] = "슈퍼노비스";
  palJobNames[173] = "권성";
  palJobNames[205] = "태권소년";
  palJobNames[167] = "태권소년";
  palJobNames[164] = "닌자";
  palJobNames[261] = "KAGEROU";
  palJobNames[262] = "OBORO";
  palJobNames[263] = "KAGEROU";
  palJobNames[264] = "OBORO";
  palJobNames[165] = "건너";
  palJobNames[266] = "리벨리온";
  palJobNames[289] = "성제";
  palJobNames[290] = "소울리퍼";
  palJobNames[291] = "성제";
  palJobNames[292] = "소울리퍼";
  palJobNames[293] = "성제";
  palJobNames[294] = "성제";
  palJobNames[166] = "검사";
  palJobNames[209] = "검사";
  palJobNames[234] = "검사";
  palJobNames[249] = "기사";
  palJobNames[257] = "기사";
  palJobNames[250] = "기사";
  palJobNames[253] = "크루";
  palJobNames[260] = "크루";
  palJobNames[254] = "팔라딘";
  palJobNames[176] = "성직자";
  palJobNames[212] = "성직자";
  palJobNames[237] = "성직자";
  palJobNames[206] = "프리스트";
  palJobNames[244] = "프리스트";
  palJobNames[207] = "하이프리스트";
  palJobNames[175] = "몽크";
  palJobNames[216] = "몽크";
  palJobNames[189] = "챔피온";
  palJobNames[180] = "마법사";
  palJobNames[210] = "마법사";
  palJobNames[235] = "마법사";
  palJobNames[184] = "위저드";
  palJobNames[245] = "위저드";
  palJobNames[186] = "하이위저드";
  palJobNames[181] = "세이지";
  palJobNames[217] = "세이지";
  palJobNames[185] = "프로페서";
  palJobNames[169] = "상인";
  palJobNames[213] = "상인";
  palJobNames[238] = "상인";
  palJobNames[188] = "제철공";
  palJobNames[246] = "제철공";
  palJobNames[168] = "화이트스미스";
  palJobNames[187] = "연금술사";
  palJobNames[219] = "연금술사";
  palJobNames[171] = "크리에이터";
  palJobNames[191] = "도둑";
  palJobNames[231] = "도둑";
  palJobNames[239] = "도둑";
  palJobNames[192] = "로그";
  palJobNames[218] = "로그";
  palJobNames[194] = "스토커";
  palJobNames[195] = "어세신";
  palJobNames[215] = "어세신";
  palJobNames[196] = "어세신크로스";
  palJobNames[172] = "궁수";
  palJobNames[211] = "궁수";
  palJobNames[236] = "궁수";
  palJobNames[204] = "헌터";
  palJobNames[214] = "헌터";
  palJobNames[200] = "스나이퍼";
  palJobNames[197] = "댄서";
  palJobNames[221] = "댄서";
  palJobNames[202] = "집시";
  palJobNames[199] = "바드";
  palJobNames[220] = "바드";
  palJobNames[203] = "크라운";
  palJobNames[26] = "SANTA";
  palJobNames[27] = "SUMMER";
  palJobNames[28] = "HANBOK";
  palJobNames[29] = "OKTOBERFEST";
  palJobNames[30] = "SUMMER2";
  palJobNames[174] = "노비스포링";
  palJobNames[178] = "슈퍼노비스포링";
  palJobNames[165] = "페코건너";
  palJobNames[266] = "peco_rebellion";
  palJobNames[164] = "두꺼비닌자";
  palJobNames[263] = "frog_kagerou";
  palJobNames[264] = "frog_oboro";
  palJobNames[205] = "태권소년포링";
  palJobNames[173] = "권성포링";
  palJobNames[295] = "해태성제";
  palJobNames[167] = "두꺼비소울링커";
  palJobNames[296] = "해태소울리퍼";
  palJobNames[13] = "페코페코_기사";
  palJobNames[64] = "로드페코";
  palJobNames[21] = "신페코크루세이더";
  palJobNames[72] = "페코팔라딘";
  palJobNames[132] = "그리폰가드";
  palJobNames[166] = "페코검사";
  palJobNames[249] = "사자기사";
  palJobNames[250] = "사자로드나이트";
  palJobNames[253] = "사자크루세이더";
  palJobNames[254] = "사자팔라딘";
  palJobNames[180] = "여우마법사";
  palJobNames[184] = "여우위저드";
  palJobNames[186] = "여우하이위저드";
  palJobNames[181] = "여우세이지";
  palJobNames[185] = "여우프로페서";
  palJobNames[176] = "복사알파카";
  palJobNames[206] = "프리스트알파카";
  palJobNames[207] = "하이프리스트알파카";
  palJobNames[175] = "몽크알파카";
  palJobNames[189] = "챔피온알파카";
  palJobNames[169] = "상인멧돼지";
  palJobNames[188] = "제철공멧돼지";
  palJobNames[168] = "화이트스미스멧돼지";
  palJobNames[187] = "연금술사멧돼지";
  palJobNames[171] = "크리에이터멧돼지";
  palJobNames[191] = "켈베로스도둑";
  palJobNames[195] = "켈베로스어쎄신";
  palJobNames[196] = "켈베로스어쎄신크로스";
  palJobNames[192] = "켈베로스로그";
  palJobNames[194] = "켈베로스스토커";
  palJobNames[172] = "타조궁수";
  palJobNames[204] = "타조헌터";
  palJobNames[200] = "타조스나이퍼";
  palJobNames[199] = "타조바드";
  palJobNames[203] = "타조크라운";
  palJobNames[197] = "타조무희";
  palJobNames[202] = "타조짚시";
  palJobNames[133] = palJobNames[132];
  palJobNames[233] = palJobNames[174];
  palJobNames[242] = palJobNames[178];
  palJobNames[234] = palJobNames[166];
  palJobNames[235] = palJobNames[180];
  palJobNames[237] = palJobNames[176];
  palJobNames[238] = palJobNames[169];
  palJobNames[239] = palJobNames[191];
  palJobNames[236] = palJobNames[172];
  palJobNames[86] = palJobNames[13];
  palJobNames[208] = palJobNames[174];
  palJobNames[209] = palJobNames[166];
  palJobNames[210] = palJobNames[180];
  palJobNames[211] = palJobNames[172];
  palJobNames[212] = palJobNames[176];
  palJobNames[213] = palJobNames[169];
  palJobNames[214] = palJobNames[204];
  palJobNames[215] = palJobNames[195];
  palJobNames[216] = palJobNames[175];
  palJobNames[217] = palJobNames[181];
  palJobNames[218] = palJobNames[192];
  palJobNames[219] = palJobNames[187];
  palJobNames[220] = palJobNames[199];
  palJobNames[221] = palJobNames[197];
  palJobNames[222] = palJobNames[178];
  palJobNames[231] = palJobNames[191];
  palJobNames[243] = palJobNames[242];
  palJobNames[244] = palJobNames[206];
  palJobNames[245] = palJobNames[184];
  palJobNames[246] = palJobNames[188];
  palJobNames[257] = palJobNames[249];
  palJobNames[280] = palJobNames[164];
  palJobNames[281] = palJobNames[263];
  palJobNames[282] = palJobNames[264];
  palJobNames[283] = palJobNames[205];
  palJobNames[284] = palJobNames[173];
  palJobNames[285] = palJobNames[167];
  palJobNames[286] = palJobNames[165];
  palJobNames[287] = palJobNames[266];
  palJobNames[160] = palJobNames[132];
  palJobNames[193] = "켈베로스쉐도우체이서";
  palJobNames[232] = palJobNames[193];
  palJobNames[190] = "켈베로스길로틴크로스";
  palJobNames[225] = palJobNames[190];
  palJobNames[170] = "제네릭멧돼지";
  palJobNames[230] = palJobNames[170];
  palJobNames[247] = "미케닉멧돼지";
  palJobNames[255] = palJobNames[247];
  palJobNames[251] = "사자로얄가드";
  palJobNames[258] = palJobNames[251];
  palJobNames[252] = "사자룬나이트";
  palJobNames[259] = palJobNames[252];
  palJobNames[248] = "타조레인져";
  palJobNames[256] = palJobNames[248];
  palJobNames[198] = "타조민스트럴";
  palJobNames[227] = palJobNames[198];
  palJobNames[201] = "타조원더러";
  palJobNames[228] = palJobNames[201];
  palJobNames[179] = "아크비숍알파카";
  palJobNames[224] = palJobNames[179];
  palJobNames[177] = "슈라알파카";
  palJobNames[229] = palJobNames[177];
  palJobNames[183] = "여우워록";
  palJobNames[223] = palJobNames[183];
  palJobNames[182] = "여우소서러";
  palJobNames[226] = palJobNames[182];
  palJobNames[295] = "해태성제";
  palJobNames[297] = "해태성제";
  palJobNames[296] = "해태소울리퍼";
  palJobNames[298] = "해태소울리퍼";
  palJobNames[302] = "DRAGON_KNIGHT";
  palJobNames[315] = "DRAGON_KNIGHT_RIDING";
  palJobNames[330] = "DRAGON_KNIGHT_CHICKEN";
  palJobNames[308] = "IMPERIAL_GUARD";
  palJobNames[321] = "IMPERIAL_GUARD_RIDING";
  palJobNames[331] = "IMPERIAL_GUARD_CHICKEN";
  palJobNames[307] = "WINDHAWK";
  palJobNames[328] = "WOLF_WINDHAWK";
  palJobNames[320] = "WINDHAWK_RIDING";
  palJobNames[313] = "TROUBADOUR";
  palJobNames[326] = "TROUBADOUR_RIDING";
  palJobNames[314] = "TROUVERE";
  palJobNames[327] = "TROUVERE_RIDING";
  palJobNames[305] = "ARCH_MAGE";
  palJobNames[318] = "ARCH_MAGE_RIDING";
  palJobNames[311] = "ELEMETAL_MASTER";
  palJobNames[324] = "ELEMETAL_MASTER_RIDING";
  palJobNames[306] = "CARDINAL";
  palJobNames[319] = "CARDINAL_RIDING";
  palJobNames[312] = "INQUISITOR";
  palJobNames[325] = "INQUISITOR_RIDING";
  palJobNames[309] = "BIOLO";
  palJobNames[322] = "BIOLO_RIDING";
  palJobNames[303] = "MEISTER";
  palJobNames[316] = "MEISTER_RIDING";
  palJobNames[329] = "MEISTER_MADOGEAR1";
  palJobNames[304] = "SHADOW_CROSS";
  palJobNames[317] = "SHADOW_CROSS_RIDING";
  palJobNames[310] = "ABYSS_CHASER";
  palJobNames[323] = "ABYSS_CHASER_RIDING";
  palJobNames[352] = "SKY_EMPEROR";
  palJobNames[366] = "SKY_EMPEROR2";
  palJobNames[359] = "SKY_EMPEROR_RIDING";
  palJobNames[353] = "SOUL_ASCETIC";
  palJobNames[360] = "SOUL_ASCETIC_RIDING";
  palJobNames[354] = "SHINKIRO";
  palJobNames[361] = "SHINKIRO_RIDING";
  palJobNames[355] = "SHIRANUI";
  palJobNames[362] = "SHIRANUI_RIDING";
  palJobNames[356] = "NIGHT_WATCH";
  palJobNames[363] = "NIGHT_WATCH_RIDING";
  palJobNames[357] = "HYPER_NOVICE";
  palJobNames[364] = "HYPER_NOVICE_RIDING";
  palJobNames[358] = "SPIRIT_HANDLER";
  palJobNames[365] = "SPIRIT_HANDLER_RIDING";
  palJobNames[26] = "SANTA";
  palJobNames[27] = "SUMMER";
  palJobNames[28] = "HANBOK";
  palJobNames[29] = "OKTOBERFEST";
  palJobNames[30] = "SUMMER2";
  std::vector<const char*>& robeNames = this->m_robeNames;
  robeNames.resize(5, "");
  robeNames[0] = "";
  robeNames[1] = "_가드";
  robeNames[3] = "_쉴드";
  robeNames[4] = "_미러쉴드";
  robeNames[2] = "_버클러";
  palJobNames[93] = "무희바지";
  palJobNames[260] = "사자크루세이더";
  palJobNames[221] = "dancer_b_riding";
  bodyJobNames[221] = "dancer_b_riding";
  this->InitJobKeyNames();
}
