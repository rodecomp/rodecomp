#include "Client/Session/Session.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/CombatTypes.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/World/World.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/TextEncoding.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/XorBytes.h"
#include "Script/LuaScript.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Application/ApplicationWindow.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "File/CommonObject.h"
#include "Contents/World/WorldViewData.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Events/CheckAttendanceMgr.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/Map/PrivateAirplaneMgr.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <mbstring.h>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>
#include "Utility/StdStringCompare.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Cipher/CryptName.h"

using SessionShopTree = std::_Tree<std::_Tmap_traits<
  int, SessionShop, std::less<int>, std::allocator<std::pair<const int, SessionShop>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, SessionShop>, void*>*>
SessionShopTree::_Find_lower_bound<int>(const int&) const;

using NameToIdTree = std::_Tree<std::_Tmap_traits<
  std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, int>, void*>*>
NameToIdTree::_Find_lower_bound<std::string>(const std::string&) const;

using ShopEntryTree = std::_Tree<std::_Tmap_traits<
  int, std::pair<int, int>, std::less<int>, std::allocator<std::pair<const int, std::pair<int, int>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::pair<int, int>>, void*>*>
ShopEntryTree::_Find_lower_bound<int>(const int&) const;

using ActorIdTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
ActorIdTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;

extern template void std::vector<std::string>::resize(size_type, const std::string&);
extern template void std::vector<unsigned char>::_Tidy() noexcept;
extern template std::string& std::string::operator=(std::string&&) noexcept;

using PendingActorNameTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;

// STATIC: 0x01732638
// GLOBAL: gl_24474
CSession g_session;

// GLOBAL: gl_25140
static std::string s_messageEndTag = "</MSG>";

class CTokenizer {
public:
  std::string& operator[](int index);
  void Parse(const std::string& text);

  /* 0000 */ std::vector<std::string> tokens;
};

class CUserHotkeyMgr {
public:
  static bool Initialize();
};

// GLOBAL: gl_25445
int g_sessionTableLoadCount;


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
  skillId -= 8200;
  if (skillId > -1) {
    return this->m_mercSkillLevels[skillId];
  }

  return 0;
}

// OFFSET: 0x00E13B00
// FUNCTION: fn_23826
int CSession::GetCurrentSex() {
  if (this->cachedSex != this->currentSex) {
    this->cachedSex = this->currentSex;
  }

  return this->cachedSex;
}

// OFFSET: 0x00DF9B90
// FUNCTION: fn_23562
int CSession::IsExchangeItemListFull() {
  return this->GetFilledExchangeItemList() == 10;
}

// OFFSET: 0x00DFFCF0
// FUNCTION: fn_23633
void CSession::ClearItemSelection() {
  for (auto it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
    if (it->flags[1] == 1) {
      it->flags[1] = 0;
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
  if (now >= this->m_lastIntervalTime + interval) {
    this->m_lastIntervalTime = now;
    return true;
  }

  return false;
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
bool CSession::IsRestrictedCharacterSlot(int value) const {
  if (value == -1) {
    return false;
  }

  return this->m_normalSlotCount - this->restrictedCharacterSlots <= value;
}

// OFFSET: 0x00E3D390
// FUNCTION: fn_24189
__declspec(noinline) int CSession::HasSwapEquipmentItem(std::uint32_t id) const {
  if (id == 0) {
    return 0;
  }

  for (int i = 0; i < 10; ++i) {
    if (id == this->swapCostumeItems[i].identity[1] || id == this->swapEquippedItems[i].identity[1]) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00E3DB20
// FUNCTION: fn_24194
bool CSession::ResetCharacterSlotState() {
  this->m_normalSlotCount = 1;
  this->m_premiumSlotCount = 1;
  this->m_billingSlotCount = 1;
  this->m_producibleSlotCount = 1;
  this->m_validSlotCount = 1;
  this->characterSlotState = 1;
  this->characterCount = 1;
  this->petJob = -1;
  this->guildLeader = 0;
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
ITEM_INFO CSession::FindMerchantPurchaseSavedItem(std::uint32_t id) const {
  for (auto item = this->merchantPurchaseSavedItems.begin(); item != this->merchantPurchaseSavedItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDE920
// FUNCTION: fn_23403
ITEM_INFO CSession::GetItem1768At(int index) const {
  if (this->merchantPurchaseSavedItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->merchantPurchaseSavedItems.begin();
  int position = 0;
  while (it != this->merchantPurchaseSavedItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDE9C0
// FUNCTION: fn_23404
int CSession::GetItem1768Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->merchantPurchaseSavedItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDE9F0
// FUNCTION: fn_23405
ITEM_INFO CSession::FindMerchantShopItem(std::uint32_t id) const {
  for (auto item = this->merchantShopItems.begin(); item != this->merchantShopItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEA80
// FUNCTION: fn_23406
ITEM_INFO CSession::GetItem1740At(int index) const {
  if (this->merchantShopItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->merchantShopItems.begin();
  int position = 0;
  while (it != this->merchantShopItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEB20
// FUNCTION: fn_23407
int CSession::GetItem1740Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->merchantShopItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDEB50
// FUNCTION: fn_23408
ITEM_INFO CSession::FindMerchantMyShopItem(std::uint32_t id) const {
  for (auto item = this->merchantMyShopItems.begin(); item != this->merchantMyShopItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEBE0
// FUNCTION: fn_23409
ITEM_INFO CSession::GetItem1750At(int index) const {
  if (this->merchantMyShopItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->merchantMyShopItems.begin();
  int position = 0;
  while (it != this->merchantMyShopItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEC80
// FUNCTION: fn_23410
int CSession::GetItem1750Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->merchantMyShopItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDECB0
// FUNCTION: fn_23411
ITEM_INFO CSession::FindMerchantPurchaseItem(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->merchantPurchaseItems) {
    if (item.identity[1] == id) {
      return item;
    }
  }

  return ITEM_INFO();
}

// OFFSET: 0x00DDED40
// FUNCTION: fn_23412
ITEM_INFO CSession::FindItem1758ByItemId(int itemId) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->merchantPurchaseItems.begin(); it != this->merchantPurchaseItems.end(); ++it) {
    const ITEM_INFO& item = *it;
    if (std::atoi(item.itemId.c_str()) == itemId) {
      return item;
    }
  }

  return ITEM_INFO();
}

// OFFSET: 0x00DDEDE0
// FUNCTION: fn_23413
ITEM_INFO CSession::GetItem1758At(int index) const {
  if (this->merchantPurchaseItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->merchantPurchaseItems.begin();
  int position = 0;
  while (it != this->merchantPurchaseItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDEEB0
// FUNCTION: fn_23414
ITEM_INFO CSession::FindMerchantHopeToSellItem(std::uint32_t id) const {
  for (auto item = this->merchantHopeToSellItems.begin(); item != this->merchantHopeToSellItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF000
// FUNCTION: fn_23416
ITEM_INFO CSession::GetItem1760At(int index) const {
  std::list<ITEM_INFO>::const_iterator it = this->merchantHopeToSellItems.begin();
  int position = 0;
  while (it != this->merchantHopeToSellItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF0F0
// FUNCTION: fn_23418
int CSession::GetItem1760Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->merchantHopeToSellItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDF120
// FUNCTION: fn_23419
ITEM_INFO CSession::FindFilteredInventoryItem(std::uint32_t id) const {
  for (auto item = this->filteredInventoryItems.begin(); item != this->filteredInventoryItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF1B0
// FUNCTION: fn_23420
long long CSession::GetItem1760Total() const {
  long long total = 0;
  for (auto it = this->merchantHopeToSellItems.begin(); it != this->merchantHopeToSellItems.end(); ++it) {
    total += static_cast<long long>(static_cast<int>(it->prices[1])) * it->quantity;
  }

  return total;
}

// OFFSET: 0x00DDF200
// FUNCTION: fn_23421
ITEM_INFO CSession::GetItem1748At(int index) const {
  if (this->merchantMirrorItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->merchantMirrorItems.begin();
  int position = 0;
  while (it != this->merchantMirrorItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DF9C50
// FUNCTION: fn_23563
bool CSession::HasShopItemIdentity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->shopItems) {
    if (item.identity[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DF9C80
// FUNCTION: fn_23564
bool CSession::HasExchangeItem(std::uint32_t id) const {
  for (int index = 0; index < 10; ++index) {
    if (this->m_exchangeItems[index].quantity != 0 && this->m_exchangeItems[index].identity[1] == id) {
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
  if (rswName.size() >= 5) {
    rswName.resize(rswName.size() - 3);
    rswName += "rsw";
  }

  return rswName.c_str();
}

// OFFSET: 0x00DDEF40
// FUNCTION: fn_23415
ITEM_INFO CSession::FindItem1760ByItemId(const char* itemId) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->merchantHopeToSellItems.begin(); it != this->merchantHopeToSellItems.end(); ++it) {
    if (it->itemId == itemId) {
      return *it;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDF090
// FUNCTION: fn_23417
int CSession::GetItem1760QuantityByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->merchantHopeToSellItems) {
    if (item.itemId == itemId) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDF2D0
// FUNCTION: fn_23422
SessionRecord0734 CSession::FindMiniPartyRecord(int id) const {
  for (const SessionRecord0734& record : this->miniPartyRecords) {
    if (id == record.id) {
      return record;
    }
  }

  return SessionRecord0734();
}

// OFFSET: 0x00DDF320
// FUNCTION: fn_23423
const char* CSession::GetBgmFileName(const char* rswName) {
  std::string name = rswName;
  const std::size_t position = name.rfind("@");
  if (position != std::string::npos && position >= 4) {
    name.erase(position - 4, 3);
  }

  auto found = std::lower_bound(this->m_bgmFileNames.begin(), this->m_bgmFileNames.end(), name.c_str(),
                                [](const std::pair<const char*, const char*>& entry, const char* value) { return std::strcmp(entry.first, value) < 0; });
  if (found != this->m_bgmFileNames.end() && std::strcmp(found->first, name.c_str()) == 0) {
    return found->second;
  }

  return "bgm\\01.mp3";
}

// OFFSET: 0x00DF9CD0
// FUNCTION: fn_23565
bool CSession::HasInventorySearchItem(int itemId) const {
  if (!this->inventoryCategories[10].empty()) {
    for (std::list<ITEM_INFO>::const_iterator item = this->inventoryCategories[10].begin();
         item != this->inventoryCategories[10].end(); ++item) {
      if (std::atoi(item->itemId.c_str()) == itemId) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00DF9D30
// FUNCTION: fn_23566
int CSession::IsFriendName(const char* name) const {
  for (const MessengerMember& member : this->m_friends) {
    if (member.name == name) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00E1DFB0
// FUNCTION: fn_23889
int CSession::GetSiegeMode() const {
  return static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->isSiegeMode;
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
int CSession::FindSkillEffectId(int key) const {
  auto found = this->skillEffectIds.find(key);
  if (this->skillEffectIds.end() != found) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00DDC0A0
// FUNCTION: fn_23362
int CSession::FindSkillFollowEffect(int key) const {
  auto found = this->skillFollowEffects.find(key);
  if (this->skillFollowEffects.end() != found) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00DDC0F0
// FUNCTION: fn_23363
const char* CSession::GetEnemyHitWaveName(int element) {
  switch (element) {
  case ELE_FIRE:
    switch (std::max(std::rand() % 2, 0)) {
    case 0:
      return "_enemy_hit_fire1.wav";
    case 1:
      return "_enemy_hit_fire2.wav";
    }
    break;
  case ELE_WIND:
    switch (std::max(std::rand() % 2, 0)) {
    case 0:
      return "_enemy_hit_wind1.wav";
    case 1:
      return "_enemy_hit_wind2.wav";
    }
    break;
  }

  switch (std::max(std::rand() % 4, 0)) {
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

  if (item->identity[3] != 0) {
    if (item->identity[3] != 0x8000 || this->equippedArrowIndex == item->identity[1]) {
      return nullptr;
    }

    for (auto it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
      if (it->identity[1] == this->equippedArrowIndex) {
        return &*it;
      }
    }

    return nullptr;
  }

  ITEM_INFO* equipped = nullptr;
  switch (item->identity[2]) {
  case EQP_HEAD_LOW:
    equipped = &this->equippedItems[0];
    break;
  case EQP_HAND_R:
  case EQP_HAND_R | EQP_HAND_L:
    equipped = &this->equippedItems[1];
    break;
  case EQP_GARMENT:
    equipped = &this->equippedItems[2];
    break;
  case EQP_ACC_RL:
    equipped = &this->equippedItems[3];
    break;
  case EQP_ARMOR:
    equipped = &this->equippedItems[4];
    break;
  case EQP_HAND_L:
    equipped = &this->equippedItems[5];
    break;
  case EQP_SHOES:
    equipped = &this->equippedItems[6];
    break;
  case EQP_HEAD_TOP:
  case EQP_HEAD_TOP | EQP_HEAD_LOW:
  case EQP_HEAD_TOP | EQP_HEAD_MID:
  case EQP_HEAD_TOP | EQP_HEAD_MID | EQP_HEAD_LOW:
    equipped = &this->equippedItems[8];
    break;
  case EQP_HEAD_MID:
  case EQP_HEAD_MID | EQP_HEAD_LOW:
    equipped = &this->equippedItems[9];
    break;
  case EQP_COSTUME_HEAD_LOW:
    equipped = &this->costumeItems[0];
    break;
  case EQP_SHADOW_WEAPON:
    equipped = &this->costumeItems[1];
    break;
  case EQP_COSTUME_GARMENT:
    equipped = &this->costumeItems[2];
    break;
  case EQP_SHADOW_ACC_R:
    equipped = &this->costumeItems[3];
    break;
  case EQP_SHADOW_ARMOR:
    equipped = &this->costumeItems[4];
    break;
  case EQP_SHADOW_SHIELD:
    equipped = &this->costumeItems[5];
    break;
  case EQP_SHADOW_SHOES:
    equipped = &this->costumeItems[6];
    break;
  case EQP_SHADOW_ACC_L:
    equipped = &this->costumeItems[7];
    break;
  case EQP_COSTUME_HEAD_TOP:
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID:
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_LOW:
  case EQP_COSTUME_HEAD_TOP | EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW:
    equipped = &this->costumeItems[8];
    break;
  case EQP_COSTUME_HEAD_MID:
  case EQP_COSTUME_HEAD_MID | EQP_COSTUME_HEAD_LOW:
    equipped = &this->costumeItems[9];
    break;
  }

  if (equipped != nullptr && equipped->quantity != 0) {
    return equipped;
  }

  return nullptr;
}

// OFFSET: 0x00DDC580
// FUNCTION: fn_23367
ITEM_INFO CSession::FindCostumeItem(std::uint32_t id, int* index) const {
  for (int i = 0; i < 10; ++i) {
    const ITEM_INFO& item = this->costumeItems[i];
    if (item.identity[1] == id) {
      *index = i;
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDC7A0
// FUNCTION: fn_23369
ITEM_INFO CSession::FindEquippedItem(std::uint32_t id, int* index) const {
  for (int i = 0; i < 10; ++i) {
    const ITEM_INFO& item = this->equippedItems[i];
    if (item.identity[1] == id) {
      *index = i;
      return item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDC830
// FUNCTION: fn_23370
ITEM_INFO CSession::GetExchangeItem(int index) const {
  return this->m_exchangeItems[index];
}

// OFFSET: 0x00DDD300
// FUNCTION: fn_23384
std::uint16_t CSession::FindInventoryIndexByItemId(int itemId) {
  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (std::atoi(item.itemId.c_str()) == itemId) {
      return static_cast<std::uint16_t>(item.identity[1]);
    }
  }

  return 0xFFFF;
}

// OFFSET: 0x00DDD400
// FUNCTION: fn_23385
std::vector<int> CSession::FindInventoryIndicesByItemId(int itemId, unsigned char identifiedOnly) {
  std::vector<int> indices;
  for (auto it = this->m_inventoryItems.begin(); it != this->m_inventoryItems.end(); ++it) {
    if (itemId == std::atoi(it->itemId.c_str()) && (identifiedOnly != 1 || it->identified == 1)) {
      indices.push_back(static_cast<int>(it->identity[1]));
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

  std::list<ITEM_INFO>::const_iterator it = this->m_inventoryItems.begin();
  int position = 0;
  while (it != this->m_inventoryItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
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

  if (static_cast<unsigned int>(list) < 11 && !this->inventoryCategories[list].empty()) {
    int position = 0;
    for (const ITEM_INFO& item : this->inventoryCategories[list]) {
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

// INLINED
template <std::size_t Length>
static bool IsTalkCommand(const std::string& token, const char (&command)[Length]) {
  return token.size() == Length && std::memcmp(token.data(), command, Length) == 0;
}

// OFFSET: 0x00DE1010
// FUNCTION: fn_23478
int CSession::GetTalkType(const char* text, int* talkType, std::string* parts) {
  if (text == nullptr) {
    *talkType = 3;
    return -1;
  }

  CTokenizer tokenizer;
  tokenizer.Parse(std::string(text));
  if (tokenizer.tokens.size() == 0) {
    *talkType = 3;
    return -1;
  }

  LowercaseText(tokenizer[0]);
  const int count = static_cast<int>(tokenizer.tokens.size());
  int offset = 0;
  if (text[0] != '/') {
    *talkType = 0;
  } else if (IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xD0'})) {
    *talkType = 0x1;
    offset = 3;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'b'})) {
    *talkType = 0x1;
    offset = 2;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'l', 'b'})) {
    *talkType = 0x57;
    offset = 3;
  } else if (IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xD3', '\xA4', '\xD0'})) {
    *talkType = 0x57;
    offset = 5;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'n', 'b'})) {
    *talkType = 0x5A;
    offset = 3;
  } else if (IsTalkCommand(tokenizer[0], {'/', '\xC6', '\xC4', '\xB6', '\xF5', '\xB8', '\xBB'})) {
    *talkType = 0x66;
    offset = 7;
  } else if (IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xCC', '\xA4', '\xD0'})) {
    *talkType = 0x5A;
    offset = 5;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'n', 'l', 'b'})) {
    *talkType = 0x5B;
    offset = 4;
  } else if (IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xCF', '\xA4', '\xD0'})) {
    *talkType = 0x5B;
    offset = 5;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'b', 'b'})) {
    *talkType = 0x66;
    offset = 3;
  } else if (tokenizer[0] == "/지역파란말") {
    *talkType = 0x67;
    offset = 11;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'm', 'b'})) {
    *talkType = 0x67;
    offset = 3;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'k', 'i', 'l', 'l'})) {
    *talkType = 0x4;
    offset = 5;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'v'}) || IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xBD'})) {
    *talkType = 0xC;
    if (count >= 2) {
      parts[0] = tokenizer[1];
      return -1;
    }
    char volume[20];
    _itoa(g_soundMgr->sampleVolume, volume, 10);
    parts[0].assign(volume, std::strlen(volume));
    return -1;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'b', 'v'}) || IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xD0', '\xA4', '\xBD'})) {
    *talkType = 0xD;
    if (count >= 2) {
      parts[0] = tokenizer[1];
      return -1;
    }
    char volume[20];
    _itoa(CSoundMgr::Volume(g_soundMgr), volume, 10);
    parts[0].assign(volume, std::strlen(volume));
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', 'g', 'c'}) || IsTalkCommand(tokenizer[0], {'/', 'g', 'u', 'i', 'l', 'd', 'c', 'h', 'a', 't'})) && count >= 2) {
    *talkType = 0x39;
    parts[0] = text + (IsTalkCommand(tokenizer[0], {'/', 'g', 'c'}) ? 4 : 11);
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', 'e', 'x'}) || IsTalkCommand(tokenizer[0], {'/', '\xC2', '\xF7', '\xB4', '\xDC'})) && count >= 2) {
    *talkType = 0x12;
    parts[0] = text + (IsTalkCommand(tokenizer[0], {'/', 'e', 'x'}) ? 4 : 6);
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', 'i', 'n'}) || IsTalkCommand(tokenizer[0], {'/', '\xC7', '\xD8', '\xC1', '\xA6'})) && count >= 2) {
    *talkType = 0x13;
    parts[0] = text + (IsTalkCommand(tokenizer[0], {'/', 'i', 'n'}) ? 4 : 6);
    return -1;
  } else if (IsTalkCommand(tokenizer[0], {'/', 'r', 'e', 'd'})) {
    *talkType = 0x14;
    for (int index = 1; index < static_cast<int>(tokenizer.tokens.size()); ++index) {
      const std::string& token = tokenizer[index];
      parts[0].append(token.c_str(), token.size());
      parts[0].append(" ");
    }
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', '\xB0', '\xC5', '\xB7', '\xA1'}) || IsTalkCommand(tokenizer[0], {'/', 'r', 'j', 'f', 'o'}) || IsTalkCommand(tokenizer[0], {'/', '\xB1', '\xB3', '\xC8', '\xAF'}) || IsTalkCommand(tokenizer[0], {'/', 'r', 'y', 'g', 'h', 'k', 's'}) || IsTalkCommand(tokenizer[0], {'/', 'd', 'e', 'a', 'l'}) || IsTalkCommand(tokenizer[0], {'/', '\xA4', '\xB7', '\xA4', '\xA7', '\xB9', '\xCC'})) && count == 2) {
    *talkType = 0x19;
    parts[0] = tokenizer[1];
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', '\xB1', '\xE6', '\xB5', '\xE5'}) || IsTalkCommand(tokenizer[0], {'/', 'r', 'l', 'f', 'e', 'm'}) || IsTalkCommand(tokenizer[0], {'/', 'g', 'u', 'i', 'l', 'd'})) && count == 2) {
    *talkType = 0x37;
    parts[0] = tokenizer[1];
    return -1;
  } else if ((IsTalkCommand(tokenizer[0], {'/', '\xC7', '\xD8', '\xC3', '\xBC'}) || tokenizer[0] == "/gocp" || tokenizer[0] == "/breakguild") && count == 2) {
    *talkType = 0x38;
    parts[0] = tokenizer[1];
    return -1;
  } else if ((tokenizer[0] == "/결성" || tokenizer[0] == "/ruftjd" || tokenizer[0] == "/organize") && count == 2) {
    *talkType = 0x1A;
    parts[0] = tokenizer[1];
    return -1;
  } else if ((tokenizer[0] == "/몬스터" || tokenizer[0] == "/아이템" || tokenizer[0] == "/item" || tokenizer[0] == "/monster") && count >= 2) {
    *talkType = 0x30;
    parts[0] = tokenizer[1];
    if (count > 2) {
      std::string joined;
      for (int index = 1; index < static_cast<int>(tokenizer.tokens.size()); ++index) {
        if (index + 1 != static_cast<int>(tokenizer.tokens.size())) {
          joined.append(tokenizer[index] + " ");
        } else {
          joined.append(tokenizer[index]);
        }
      }
      parts[0] = joined;
    }
    return -1;
  } else if ((tokenizer[0] == "/맵이동" || tokenizer[0] == "/mapmove" || tokenizer[0] == "/mm") && count > 1) {
    *talkType = 0x31;
    parts[0] = tokenizer[1];
    if (tokenizer.tokens.size() == 4) {
      parts[1] = tokenizer[2];
      parts[2] = tokenizer[3];
    } else {
      parts[1] = parts[2].assign("0");
    }
    return -1;
  } else if ((tokenizer[0] == "/제명" || tokenizer[0] == "/wpaud" || tokenizer[0] == "/expel") && count == 2) {
    *talkType = 0x1C;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/sm" && count == 2) {
    *talkType = 0x3A;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/resetstate") {
    *talkType = 0x3D;
    return -1;
  } else if (tokenizer[0] == "/resetskill") {
    *talkType = 0x3E;
    if (tokenizer.tokens.size() >= 2) {
      parts[0] = tokenizer[1];
    } else {
      parts[0].assign("1");
    }
    return -1;
  } else if ((tokenizer[0] == "/changemaptype" || tokenizer[0] == "/cmt") && count == 4) {
    *talkType = 0x3F;
    parts[0] = tokenizer[1];
    parts[1] = tokenizer[2];
    parts[2] = tokenizer[3];
    return -1;
  } else if (tokenizer[0] == "/emblem" || tokenizer[0] == "/엠블렘" || tokenizer[0] == "/앰블렘" || tokenizer[0] == "/dpaqmffpa" || tokenizer[0] == "/doaqmffpa") {
    *talkType = 0x40;
    return -1;
  } else if (tokenizer[0] == "/hide") {
    *talkType = 0x58;
    return -1;
  } else if (tokenizer[0] == "/폰트" || tokenizer[0] == "/font") {
    *talkType = 0x5C;
    return -1;
  } else if (tokenizer[0] == "/mineffect" || tokenizer[0] == LookupMessage(MSI_MINEFFECT)) {
    *talkType = 0xE5;
    return -1;
  } else if (tokenizer[0] == "/remove" && count == 2) {
    *talkType = 0x5F;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/summon" && count == 2) {
    *talkType = 0x62;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/shift") {
    *talkType = 0x60;
    for (int index = 0; index < static_cast<int>(tokenizer.tokens.size()) - 1; ++index) {
      parts[index] = tokenizer[index + 1];
    }
    return -1;
  } else if (tokenizer[0] == "/recall" && count == 2) {
    *talkType = 0x61;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/check" && count == 2) {
    *talkType = 0x8F;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/rc" && count == 2) {
    *talkType = 0x90;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/요금") {
    *talkType = 0x65;
    return -1;
  } else if (tokenizer[0] == "/이름표시" || tokenizer[0] == "/showname") {
    *talkType = 0x6C;
    offset = 0;
  } else if (tokenizer[0] == "/str+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0xD, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/agi+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0xE, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/luk+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0x12, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/int+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0x10, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/vit+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0xF, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/dex+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_STATUS_CHANGE, 0x11, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/pow+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xDB, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/sta+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xDC, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/wis+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xDD, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/spl+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xDE, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/con+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xDF, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/crt+" && count > 1) {
    *talkType = 0x89;
    g_modeMgr.GetGameMode()->SendMsg(MM_ADVANCED_STATUS_CHANGE, 0xE0, std::atoi(tokenizer[1].c_str()), 0, 0);
  } else if (tokenizer[0] == "/hi") {
    std::string message("");
    *talkType = 0x97;
    if (count > 1) {
      parts[0] = tokenizer[1];
      for (int index = 1; index < static_cast<int>(tokenizer.tokens.size()); ++index) {
        message.append(tokenizer[index]);
        message.append(" ");
      }
      g_windowMgr.SetChatInputText(message.c_str());
    }
    return -1;
  } else if (tokenizer[0] == "/초대" || tokenizer[0] == "/cheo" || tokenizer[0] == "/invite") {
    *talkType = 0xB8;
    std::string command(text);
    const std::size_t space = command.find(" ", 0);
    if (space != std::string::npos) {
      parts[0] = command.erase(0, space + 1);
    } else {
      g_windowMgr.SetChatInputText(LookupMessage(MSI_NOWARPSKILL2));
    }
    return -1;
  } else if (tokenizer[0] == "/길드초대" || tokenizer[0] == "/rlfemcheo" || tokenizer[0] == "/guildinvite") {
    *talkType = 0xD3;
    std::string command(text);
    const std::size_t space = command.find(" ", 0);
    if (space != std::string::npos) {
      parts[0] = command.erase(0, space + 1);
    } else {
      g_windowMgr.SetChatInputText(LookupMessage(MSI_PARA_PURCHASE_ITEM_WEIGHT));
    }
    return -1;
  } else if (tokenizer[0] == "/web" && count >= 2) {
    *talkType = 0xD0;
    parts[0] = tokenizer[1];
  } else if ((tokenizer[0] == "/navi" || tokenizer[0] == "/navigation") && count >= 3) {
    *talkType = 0xD1;
    parts[0] = tokenizer[1];
    parts[1] = tokenizer[2];
    return -1;
  } else if ((tokenizer[0] == "/navi2" || tokenizer[0] == "/navigation2") && count >= 4) {
    *talkType = 0xD2;
    parts[0] = tokenizer[1];
    parts[1] = tokenizer[2];
    parts[2] = tokenizer[3];
    return -1;
  } else if (tokenizer[0] == "/naviopen") {
    *talkType = 0xD9;
    return -1;
  } else if ((tokenizer[0] == "/cl" || tokenizer[0] == "/clanchat") && count >= 2) {
    *talkType = 0xD6;
    parts[0] = text + (tokenizer[0] == "/cl" ? 4 : 10);
    return -1;
  } else if (tokenizer[0] == LookupMessage(MSI_PARA_RESULT_TITLE)) {
    *talkType = 0xD8;
    return -1;
  } else if (tokenizer[0] == "/cashshop") {
    *talkType = 0xDA;
    return -1;
  } else if (tokenizer[0] == "/goldpc") {
    *talkType = 0xDB;
    return -1;
  } else if (tokenizer[0] == "/roulette") {
    *talkType = 0xDC;
    return -1;
  } else if (tokenizer[0] == "/quake") {
    *talkType = 0xDD;
    return -1;
  } else if (tokenizer[0] == "/매크로등록" || tokenizer[0] == "/macro_register" || tokenizer[0] == "/mr") {
    *talkType = 0xE0;
    return -1;
  } else if (tokenizer[0] == "/매크로확인" || tokenizer[0] == "/macro_preview" || tokenizer[0] == "/mp") {
    *talkType = 0xE1;
    if (count >= 2) {
      parts[0] = tokenizer[1];
    }
    return -1;
  } else if (tokenizer[0] == "/매크로신고" || tokenizer[0] == "/macro_detector" || tokenizer[0] == "/md") {
    *talkType = 0xE2;
    if (count >= 2) {
      parts[0] = tokenizer[1];
    }
    return -1;
  } else if (tokenizer[0] == "/setcamera" && count > 2) {
    *talkType = 0xE4;
    parts[0] = tokenizer[1];
    parts[1] = tokenizer[2];
    if (tokenizer.tokens.size() == 4) {
      parts[2] = tokenizer[3];
    }
    return -1;
  } else if (tokenizer[0] == "/resetcooltime" || tokenizer[0] == "/rct") {
    *talkType = 0xE6;
    return -1;
  } else if (tokenizer[0] == "/call" || tokenizer[0] == "/호출") {
    *talkType = 0xE7;
    return -1;
  } else if (tokenizer[0] == "/환복" || tokenizer[0] == "/changedress" || tokenizer[0] == "/nocosplay") {
    *talkType = 0xEF;
    return -1;
  } else if (tokenizer[0] == "/노점보기" || tokenizer[0] == "/showshop") {
    *talkType = 0xFA;
    return -1;
  } else if (tokenizer[0] == "/prison") {
    *talkType = 0xFB;
    if (count >= 2) {
      parts[0] = tokenizer[1];
    }
    return -1;
  } else if (tokenizer[0] == "/계정한정" || tokenizer[0] == "/accountlimitedtool") {
    *talkType = 0xFC;
    return -1;
  } else if (tokenizer[0] == "/모험가중개소" || tokenizer[0] == "/중개소" || tokenizer[0] == "/agency") {
    *talkType = 0xED;
    return -1;
  } else if (tokenizer[0] == "/removeon") {
    *talkType = 0xFE;
    if (tokenizer.tokens.size() >= 2) {
      parts[0] = tokenizer[1];
    } else {
      parts[0] = "0";
    }
    if (tokenizer.tokens.size() >= 3) {
      parts[1] = tokenizer[2];
    } else {
      parts[1] = "0";
    }
    return -1;
  } else if (tokenizer[0] == "/removeoff") {
    *talkType = 0xFF;
    if (count >= 2) {
      parts[0] = tokenizer[1];
    }
    return -1;
  } else if ((tokenizer[0] == "/MacroChecker" || tokenizer[0] == "/macrochecker") && count > 1) {
    *talkType = 0x104;
    parts[0] = tokenizer[1];
    return -1;
  } else if (tokenizer[0] == "/ProbabilityTest" || tokenizer[0] == "/확률테스트") {
    *talkType = 0x117;
    return -1;
  } else {
    *talkType = this->GetCommandId(tokenizer[0].c_str());
    return -1;
  }

  if (*talkType != 0) {
    while (text[offset] == ' ') {
      ++offset;
    }
  }

  return offset;
}

// OFFSET: 0x00DDF890
// FUNCTION: fn_23426
int CSession::GetItem1728Count() const {
  return static_cast<int>(this->sellItems.size());
}

// OFFSET: 0x00DDFA50
// FUNCTION: fn_23442
ITEM_INFO CSession::GetItemSlot(int index) const {
  return this->m_itemSlots[index];
}

// OFFSET: 0x00DDFAC0
// FUNCTION: fn_23443
ITEM_INFO CSession::GetAlternateEquippedItemAt(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (index >= 0 && index < 10) {
    return this->alternateEquippedItems[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDFD30
// FUNCTION: fn_23445
int CSession::GetItem1718QuantityByItemId(const ITEM_INFO& item) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    const int itemId = std::atoi(it->itemId.c_str());
    if (itemId == std::atoi(item.itemId.c_str())) {
      return it->quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFDB0
// FUNCTION: fn_23446
int CSession::GetItem1718QuantityOf(const ITEM_INFO& item) const {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      return it->quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFDE0
// FUNCTION: fn_23447
std::uint16_t CSession::GetShopItemPriceFactorByItemId(const ITEM_INFO& item) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    const int itemId = std::atoi(it->itemId.c_str());
    if (itemId == std::atoi(item.itemId.c_str())) {
      return it->priceFactor;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFE60
// FUNCTION: fn_23448
std::uint16_t CSession::GetShopItemPriceFactor(const ITEM_INFO& item) const {
  for (auto it = this->shopItems.begin(); it != this->shopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      return it->priceFactor;
    }
  }

  return 0;
}

// OFFSET: 0x00DDFE90
// FUNCTION: fn_23449
std::uint16_t CSession::SumPurchasePriceFactorByItemId() const {
  std::uint16_t total = 0;
  for (std::list<ITEM_INFO>::const_iterator it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    total += it->quantity * this->GetShopItemPriceFactorByItemId(*it);
  }

  return total;
}

// OFFSET: 0x00DDFF60
// FUNCTION: fn_23450
std::uint16_t CSession::SumPurchasePriceFactor() const {
  std::uint16_t total = 0;
  for (auto it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    total += it->quantity * this->GetShopItemPriceFactor(*it);
  }

  return total;
}

// OFFSET: 0x00DE0620
// FUNCTION: fn_23456
ITEM_INFO CSession::FindItem1720ByItemId(const char* itemId) const {
  for (std::list<ITEM_INFO>::const_iterator it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    if (it->itemId == itemId) {
      return *it;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE06E0
// FUNCTION: fn_23457
ITEM_INFO CSession::GetItem1720At(int index) const {
  std::list<ITEM_INFO>::const_iterator it = this->purchaseItems.begin();
  int position = 0;
  while (it != this->purchaseItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0770
// FUNCTION: fn_23458
int CSession::GetItem1720QuantityByItemId(const char* itemId) const {
  for (const ITEM_INFO& item : this->purchaseItems) {
    if (item.itemId == itemId) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE07D0
// FUNCTION: fn_23459
int CSession::GetItem1720Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->purchaseItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0800
// FUNCTION: fn_23460
int CSession::GetItem1720Total() const {
  int total = 0;
  for (auto item = this->purchaseItems.begin(); item != this->purchaseItems.end(); ++item) {
    total += item->prices[1] * item->quantity;
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
ITEM_INFO CSession::FindStorageItem(std::uint32_t id) const {
  for (auto item = this->storageItems.begin(); item != this->storageItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0980
// FUNCTION: fn_23464
ITEM_INFO CSession::GetItem1730At(int index) const {
  if (this->storageItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->storageItems.begin();
  int position = 0;
  while (it != this->storageItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0A20
// FUNCTION: fn_23465
int CSession::GetItem1730Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->storageItems) {
    if (item.identity[1] == id) {
      return item.quantity;
    }
  }

  return 0;
}

// OFFSET: 0x00DE0D30
// FUNCTION: fn_23470
ITEM_INFO CSession::GetItem1718At(int index) const {
  if (this->shopItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->shopItems.begin();
  int position = 0;
  while (it != this->shopItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0DD0
// FUNCTION: fn_23471
int CSession::GetItem1718Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->shopItems) {
    if (item.identity[1] == id) {
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
ITEM_INFO CSession::FindCartItem(std::uint32_t id) const {
  for (auto item = this->cartItems.begin(); item != this->cartItems.end(); ++item) {
    if (item->identity[1] == id) {
      return *item;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0F40
// FUNCTION: fn_23476
ITEM_INFO CSession::GetItem1738At(int index) const {
  if (this->cartItems.empty()) {
    return ITEM_INFO(0);
  }

  std::list<ITEM_INFO>::const_iterator it = this->cartItems.begin();
  int position = 0;
  while (it != this->cartItems.end()) {
    if (position == index) {
      return *it;
    }
    ++position;
    ++it;
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DE0FE0
// FUNCTION: fn_23477
int CSession::GetItem1738Quantity(std::uint32_t id) const {
  for (const ITEM_INFO& item : this->cartItems) {
    if (item.identity[1] == id) {
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
  std::list<GuildInfo::Member> members;
  members = g_guildInfo.m_members;
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
  std::list<GuildInfo::Member> members;
  members = g_guildInfo.m_members;
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
  std::list<GuildInfo::Member> members;
  members = g_guildInfo.m_members;
  for (const GuildInfo::Member& member : members) {
    if (member.name == name) {
      return true;
    }
  }

  return std::strcmp(g_guildInfo.masterName.c_str(), name) == 0;
}

// OFFSET: 0x00DFA160
// FUNCTION: fn_23571
bool CSession::IsIndoorMapName(const char* name) const {
  std::string value(name);
  const std::size_t position = value.rfind("@");
  if (position != std::string::npos && position >= 4) {
    value.erase(position - 4, 3);
  }

  auto found = std::lower_bound(this->m_indoorRswNames.begin(), this->m_indoorRswNames.end(), value.c_str(),
                                [](const char* left, const char* right) { return std::strcmp(left, right) < 0; });
  return found != this->m_indoorRswNames.end() && std::strcmp(*found, value.c_str()) == 0;
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
    if (actor != nullptr && static_cast<EffectActor*>(actor)->HasId(160) == true) {
      actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
void CSession::AddPendingActorName(std::uint32_t id) {
  this->pendingActorNames.insert(id);
}

// OFFSET: 0x00E2F3B0
// FUNCTION: fn_24153
void CSession::EndSkillOption() {
  auto option = this->m_options.m_values.find(2);
  if (option != this->m_options.m_values.end() && static_cast<int>(option->second) != 0) {
    this->automaticSkillHotkey = 0;
    PACKET_CZ_USE_SKILL_END packet;
    packet.packetType = ID_CZ_USE_SKILL_END;
    packet.skillId = GC_ROLLINGCUTTER;
    GetRagConnection().Send(sizeof(packet), &packet);
    this->m_options.m_values[2] = 0;
  }
}

// OFFSET: 0x00E2F520
// FUNCTION: fn_24154
void CSession::ClearNormalItemSlots() {
  std::memset(this->m_normalItemSlots, 0, sizeof(this->m_normalItemSlots));
}

// OFFSET: 0x00E2F540
// FUNCTION: fn_24155
void CSession::ResetSwapEquippedItems() {
  for (int index = 0; index < 10; ++index) {
    this->swapEquippedItems[index].Reset();
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2F570
// FUNCTION: fn_24156
void CSession::ResetSwapCostumeItems() {
  for (int index = 0; index < 10; ++index) {
    this->swapCostumeItems[index].Reset();
  }

  RefreshItemWindows(&g_windowMgr);
}

// OFFSET: 0x00E2F5A0
// FUNCTION: fn_24157
bool CSession::IsPartyRecommendTarget(std::uint32_t aid) {
  if (g_session.m_level < 70 || g_session.IsBabyJob(g_session.GetDisplayJob()) == true || g_session.adoptionSkillAvailable == 0) {
    return false;
  }

  CGameActor* actor = static_cast<CGameActor*>(g_session.FindGameActor(aid));

  if (!actor) {
    return false;
  }

  const std::string name = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(aid);
  if (g_session.PartyByName(name.c_str()).valid == 0) {
    return false;
  }

  return 50 >= static_cast<std::int16_t>(actor->characterLevel) && g_session.IsBabyJob(aid) != true &&
         static_cast<unsigned>(aid - JT_NOVICE_H) > 20;
}

// OFFSET: 0x00E2F710
// FUNCTION: fn_24158
int CSession::GetBodyStyle(int job, int style, int renderJob) {
  if (style >= 2 || static_cast<unsigned>(renderJob - JT_RUNE_KNIGHT_2ND) <= JT_SECOND_JOB_END - JT_RUNE_KNIGHT_2ND - 1) {
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
int CSession::GetSessionIndex() const {
  return this->sessionIndex;
}

// OFFSET: 0x00DFF060
// FUNCTION: fn_23606
void CSession::RefreshMessengerGroupWindow() {
  if (UIWindow* window = g_windowMgr.messengerGroupWnd) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF080
// FUNCTION: fn_23607
void CSession::RefreshItem1740Window() {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.merchantItemWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF0A0
// FUNCTION: fn_23608
void CSession::RefreshItem1750Window(int type) {
  std::uint32_t window;
  if (type == 1) {
    window = g_windowMgr.buyingShopMirrorItemWnd;
  } else {
    window = type == 2 ? g_windowMgr.purchaseMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
  }

  if (window) {
    reinterpret_cast<UIWindow*>(window)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF120
// FUNCTION: fn_23610
void CSession::UpdateMonsterInfoEnabled(int enabled) {
  this->monsterInfoEnabled = g_serviceOptions.Get(enabled ? 45 : 46);
}

// OFFSET: 0x00DFF150
// FUNCTION: fn_23611
void __fastcall RefreshPartyWindows(CSession*) {
  if (UIWindow* window = g_windowMgr.messengerGroupWnd) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
  g_windowMgr.RebuildMiniPartyWindows();
}

// OFFSET: 0x00DFF180
// FUNCTION: fn_23612
void CSession::RefreshItem1738Window() {
  if (UIWindow* window = g_windowMgr.itemStoreWnd) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF1A0
// FUNCTION: fn_23613
void CSession::EraseMiniPartyRecord(int id) {
  for (auto it = this->miniPartyRecords.begin(); it != this->miniPartyRecords.end(); ++it) {
    if (id == it->id) {
      this->miniPartyRecords.erase(it);
      return;
    }
  }
}

// OFFSET: 0x00DFF250
// FUNCTION: fn_23615
void CSession::IgnoreText(std::string text) {
  std::string ignored;
}

// OFFSET: 0x00DFF2A0
// FUNCTION: fn_23616
void CSession::SaveMiniPartyInfo() {
  std::string text("");

  for (auto it = this->miniPartyRecords.begin(); it != this->miniPartyRecords.end(); ++it) {
    char line[512];
    std::memset(line, 0, sizeof(line));
    sprintf_s(line, "{%s,%d,%d,%d}\n", g_session.DecodePlayerName(), it->active, it->id, it->cell);
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
  for (ITEM_INFO& item : this->merchantPurchaseSavedItems) {
    if (position == index) {
      item.quantity = quantity;
      return;
    }
    ++position;
  }
}

// OFFSET: 0x00DFF4B0
// FUNCTION: fn_23618
void CSession::SetReplayPacketMode(int value) {
  this->replayPacketMode = value;
  if (UIWindow* window = g_windowMgr.equipWnd) {
    window->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF4E0
// FUNCTION: fn_23619
void CSession::UpdateFriendName(std::uint32_t characterId, const char* name) {
  for (auto it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    if (it->characterId == characterId) {
      if (std::strlen(name) == 0) {
        g_modeMgr.gameMode->SendMsg(MM_DELETE_FRIEND, it->accountId, it->characterId, 0, 0);
        this->m_friends.erase(it);
      } else {
        it->name = name;
      }
      break;
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MESSENGERGROUPWND)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00DFF830
// FUNCTION: fn_23621
void CSession::SetCostumeItem(unsigned int index, const ITEM_INFO& item) {
  if (index <= 9) {
    this->costumeItems[index] = item;
    EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RefreshWearable(reinterpret_cast<const int&>(item.identity[3]));
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
void CSession::SetEquippedArrowIndex(std::uint32_t value) {
  this->equippedArrowIndex = value;
  this->InvalidateWindowFrames();

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (!mode) {
    return;
  }

  std::vector<ActiveSkillEffect>::iterator it = std::find_if(
      g_activeSkillEffects.begin(), g_activeSkillEffects.end(),
      [](const ActiveSkillEffect& effect) { return effect.id == 0x2B7; });

  if (it == g_activeSkillEffects.end()) {
    if (value) {
      ActiveSkillEffect effect;
      effect.id = 0x2B7;
      g_activeSkillEffects.push_back(effect);
    }
  } else if (!value) {
    g_activeSkillEffects.erase(it);
  }

  if (mode->scene && mode->scene->player && mode->scene->player->skillEffect) {
    mode->scene->player->skillEffect->SendMsg(nullptr, AM_SETCOUNT, 0, 1, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFF9C0
// FUNCTION: fn_23625
void CSession::SetEquippedItem(unsigned int index, const ITEM_INFO& item) {
  if (index <= 9) {
    this->equippedItems[index] = item;
    EquipmentProperties::CEquipmentPropertiesMgr::GetInstance()->RefreshWearable(reinterpret_cast<const int&>(item.identity[3]));
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
          std::sprintf(message, LookupMessage(MSI_LOGIN_FRIEND), member.name.c_str());
        } else {
          std::sprintf(message, LookupMessage(MSI_LOGOUT_FRIEND), member.name.c_str());
        }
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF00, 0, 0, 0);
      }
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00DFFB70
// FUNCTION: fn_23627
void CSession::SetHomunSkillLevel(int skillId, int level) {
  skillId -= 8000;
  if (skillId > -1) {
    this->m_homunSkillLevels[skillId] = level;
  }
}

// OFFSET: 0x00DFFBA0
// FUNCTION: fn_23628
void CSession::UpdateInventoryItem(const ITEM_INFO& item, unsigned char refresh) {
  auto it = this->m_inventoryItems.begin();
  while (it != this->m_inventoryItems.end() && it->identity[1] != item.identity[1]) {
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
  skillId -= 8200;
  if (skillId > -1) {
    this->m_mercSkillLevels[skillId] = level;
  }
}

// OFFSET: 0x00DFFC40
// FUNCTION: fn_23630
void CSession::SetMerchantPurchaseSavedPrice(int index, std::uint32_t value) {
  int position = 0;
  for (ITEM_INFO& item : this->merchantPurchaseSavedItems) {
    if (position == index) {
      item.prices[0] = value;
      item.prices[1] = value;
      return;
    }
    ++position;
  }
}

// OFFSET: 0x00DFFC80
// FUNCTION: fn_23631
void CSession::AddMiniPartyRecord(int id, std::uint32_t value) {
  SessionRecord0734 record;
  record.active = 1;
  record.id = id;
  record.cell = value;
  this->miniPartyRecords.push_back(record);
}

// OFFSET: 0x00DFFCB0
// FUNCTION: fn_23632
void CSession::SetExchangeMoney(int value) {
  this->exchangeMoney = value;
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00DFFD20
// FUNCTION: fn_23634
void CSession::SetOtherExchangeMoney(int value) {
  this->otherExchangeMoney = value;
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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
  for (SessionRecord0734& record : this->miniPartyRecords) {
    if (id == record.id) {
      record.cell = value;
      return;
    }
  }
}

// OFFSET: 0x00E31B20
// FUNCTION: fn_24174
ITEM_INFO CSession::GetSwapEquippedItem(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (static_cast<unsigned int>(index) <= 9) {
    return this->swapEquippedItems[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E31BB0
// FUNCTION: fn_24175
ITEM_INFO CSession::GetSwapCostumeItem(int index) const {
  if (index == 15) {
    return LookupClientItemByIdentity(this->m_inventoryItems, this->swapArrowIndex);
  }

  if (static_cast<unsigned int>(index) <= 9) {
    return this->swapCostumeItems[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E31DE0
// FUNCTION: fn_24177
bool CSession::IsRequestReady(int requestId) {
  std::list<ShortcutCooldownRecord>::iterator it = this->m_requestTimers.begin();
  for (; it != this->m_requestTimers.end(); ++it) {
    if (requestId == it->id || it->id == 0x241) {
      break;
    }
  }
  if (it != this->m_requestTimers.end() && it->end - 100 > timeGetTime()) {
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

  if (g_windowMgr.basicInfoWnd) {
    if (this->maxHp < this->hp ||
        *reinterpret_cast<const int*>(&this->spAndHomunculusStats[4]) < *reinterpret_cast<const int*>(&this->spAndHomunculusStats[0])) {
      return;
    }

    reinterpret_cast<UIWindow*>(g_windowMgr.basicInfoWnd)->Invalidate();
    reinterpret_cast<UIWindow*>(g_windowMgr.basicInfoWnd)->InvalidateChildren();
  }

  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->NotifyItemBounds();
}

// OFFSET: 0x00DF9460
// FUNCTION: fn_23515
void CSession::RefreshHomunculusInfo() {
  if (g_modeMgr.GetGameMode() && g_windowMgr.homunInfoWnd) {
    g_windowMgr.homunInfoWnd->Invalidate();
    g_windowMgr.homunInfoWnd->InvalidateChildren();
  }
}

// OFFSET: 0x00DF94D0
// FUNCTION: fn_23517
void CSession::RefreshMercenaryInfo() {
  if (g_modeMgr.GetGameMode() && g_windowMgr.mercenaryInfoWnd) {
    g_windowMgr.mercenaryInfoWnd->Invalidate();
    g_windowMgr.mercenaryInfoWnd->InvalidateChildren();
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

  if (text[length - 1] == ' ') {
    return true;
  }

  return false;
}

// OFFSET: 0x00DF9AA0
// FUNCTION: fn_23561
bool CSession::AreInventoryCategoriesEmpty() const {
  for (int i = 0; i < 11; ++i) {
    if (!this->inventoryCategories[i].empty()) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00DFA3C0
// FUNCTION: fn_23573
bool CSession::HasItem1740Number(int value) const {
  std::list<ITEM_INFO>::const_iterator it = this->merchantShopItems.begin();
  for (; it != this->merchantShopItems.end(); ++it) {
    if (value == std::atoi(it->itemId.c_str())) {
      break;
    }
  }

  return it != this->merchantShopItems.end();
}

// OFFSET: 0x00DFA420
// FUNCTION: fn_23574
bool CSession::HasCharacterName(std::string name) {
  for (std::vector<std::string>::iterator it = this->characterNames.begin(); it != this->characterNames.end(); ++it) {
    if (name == *it) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFA4C0
// FUNCTION: fn_23575
bool CSession::IsKnownName(std::string name) const {
  if (name == this->DecodePlayerName()) {
    return true;
  }

  const char* text = name.c_str();
  for (const MessengerMember& member : this->m_friends) {
    if (member.name == text) {
      return true;
    }
  }

  GuildInfo* guildInfo = &g_guildInfo;
  if (guildInfo->GetMemberAccountId(name.c_str()) != -1) {
    return true;
  }

  if (name.find(s_messageEndTag) != std::string::npos) {
    return true;
  }

  return false;
}

// OFFSET: 0x00DFC0B0
// FUNCTION: fn_23593
void CSession::FlagInventoryItem(std::uint32_t id) {
  for (ITEM_INFO& item : this->m_inventoryItems) {
    if (item.identity[1] == id) {
      item.flags[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC0F0
// FUNCTION: fn_23594
void CSession::FlagMerchantShopItem(std::uint32_t id) {
  for (ITEM_INFO& item : this->merchantShopItems) {
    if (item.identity[1] == id) {
      item.flags[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC130
// FUNCTION: fn_23595
void CSession::FlagCartItem(std::uint32_t id) {
  for (ITEM_INFO& item : this->cartItems) {
    if (item.identity[1] == id) {
      item.flags[2] = 1;
      RefreshItemWindows(&g_windowMgr);
      return;
    }
  }
}

// OFFSET: 0x00DFC170
// FUNCTION: fn_23596
void CSession::CollectFilteredMerchantMyShopItems() {
  this->merchantMyShopItems.erase(this->merchantMyShopItems.begin(), this->merchantMyShopItems.end());

  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (item.identity[1] > 0) {
      const int itemId = std::atoi(item.itemId.c_str());
      if (std::find(this->inventoryFilterIds.begin(), this->inventoryFilterIds.end(), itemId) != this->inventoryFilterIds.end()) {
        this->merchantMyShopItems.push_back(item);
      }
    }
  }
}

// OFFSET: 0x00DFC330
// FUNCTION: fn_23597
void CSession::RefreshMerchantMyShopItems() {
  this->merchantMyShopItems.erase(this->merchantMyShopItems.begin(), this->merchantMyShopItems.end());

  const int count = static_cast<int>(this->m_inventoryItems.size());
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = this->GetInventoryItemAt(i);
    if (item.identity[1] > 0) {
      const int itemId = std::atoi(item.itemId.c_str());
      if (std::find(this->inventoryFilterIds.begin(), this->inventoryFilterIds.end(), itemId) != this->inventoryFilterIds.end()) {
        this->merchantMyShopItems.push_back(item);
      }
    }
  }

  for (auto it = this->merchantMyShopItems.begin(); it != this->merchantMyShopItems.end();) {
    const int itemId = std::atoi(it->itemId.c_str());
    auto match = this->merchantMirrorItems.begin();
    for (; match != this->merchantMirrorItems.end(); ++match) {
      if (std::atoi(match->itemId.c_str()) == itemId) {
        break;
      }
    }

    if (match == this->merchantMirrorItems.end()) {
      it = this->merchantMyShopItems.erase(it);
      continue;
    }

    const std::uint32_t value = match->prices[0];
    if (value == 0xFFFFFFFF || g_session.HasSwapEquipmentItem(it->identity[1]) == 1) {
      it = this->merchantMyShopItems.erase(it);
      continue;
    }

    it->prices[0] = value;
    it->prices[1] = value;
    ++it;
  }
}

std::uint32_t __fastcall ItemCardColor(const ITEM_INFO* item);

// OFFSET: 0x00DDD2C0
// FUNCTION: fn_23383
std::uint32_t CSession::GetItemNameColor(const ITEM_INFO& item) const {
  if (item.identified) {
    const std::uint32_t color = ItemCardColor(&item);
    if (color != 0) {
      return color;
    }
    if (item.bindOnEquip == 2) {
      return 0xFFFFFF00;
    }
    return 0xFFFFFFFF;
  }
  return 0xFFAAAAAA;
}

// OFFSET: 0x00E30720
// FUNCTION: fn_24166
int CSession::CountDistinctCostumeItems() const {
  int count = 0;
  std::vector<std::uint32_t> itemIds;
  for (int i = 0; i < 10; ++i) {
    const int itemId = this->costumeItems[i].identity[1];
    if (itemId != 0 && std::find(itemIds.begin(), itemIds.end(), itemId) == itemIds.end()) {
      itemIds.push_back(itemId);
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00E30830
// FUNCTION: fn_24167
int CSession::CountDistinctEquippedItems() const {
  int count = 0;
  std::vector<std::uint32_t> itemIds;
  for (int i = 0; i < 10; ++i) {
    const int itemId = this->equippedItems[i].identity[1];
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
  this->InitializeSkillTree();
  if (g_windowMgr.skillListWnd) {
    g_windowMgr.skillListWnd->SendMsg(nullptr, UM_REFRESH, 0, 0, 0, 0);
  }

  if (g_windowMgr.basicInfoWnd) {
    UIBasicInfoWnd* info = reinterpret_cast<UIBasicInfoWnd*>(g_windowMgr.basicInfoWnd);
    info->SetHeight(info->geometry.height);
  }
}

// OFFSET: 0x00DF9500
// FUNCTION: fn_23518
void CSession::InvalidateWindowFrames() {
  g_windowMgr.InvalidateFrames();
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
  if (it != this->m_shops.end()) {
    return it->second.flag;
  }

  return 0;
}

// OFFSET: 0x00DE3930
// FUNCTION: fn_23484
std::pair<int, int> CSession::GetShopEntry(unsigned int index, int key) {
  auto name = this->m_nameToId.find(std::string(this->m_navigationPlayerMap));
  if (name != this->m_nameToId.end()) {
    auto shop = this->m_shops.find(name->second);
    if (shop != this->m_shops.end() && shop->second.items.size() > index) {
      std::map<int, std::pair<int, int>>& entries = shop->second.items[index];
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
void CSession::HiFriends(char* text) {
  std::string greeting = "(Hi) ";
  if (text) {
    greeting += text;
  } else {
    greeting += "*^_^*";
  }

  for (auto it = this->m_friends.begin(); it != this->m_friends.end(); ++it) {
    g_windowMgr.chatInputText = greeting.c_str();
    g_modeMgr.gameMode->SendMsg(MM_WHISPERMSG, reinterpret_cast<int>(it->name.c_str()), 0, 0, 0);
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
    record.active = 1;
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
          record.cell = std::atoi(&data[i]);
          state = 1;
        }
      }
      break;
    }
    case '}':
      this->miniPartyRecords.push_back(record);
      break;
    case '{': {
      ++i;
      std::memset(name, 0, sizeof(name) - 1);
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
        record.active = std::atoi(&data[i]);
        state = 2;
      }
      break;
    }
    case '\n':
    case '\r':
      continue;
    }
  }

  delete[] data;
}

// OFFSET: 0x00DFD060
// FUNCTION: fn_23603
void CSession::LoadExternalSettings() {
  switch (g_serviceType) {
  case 0:
    switch (g_serviceMode) {
    case 1:
      {
        const char* fileName = std::strstr(g_launchServerName, "QmSakray") != nullptr ? "Lua Files\\service_korea\\ExternalSettings_kr_sak_qm" : "Lua Files\\service_korea\\ExternalSettings_kr_sak";
        this->m_hotkeyScript->Load(fileName, true, false);
      }
      break;
    case 2:
      this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr_indoor", true, false);
      break;
    default:
      if (g_serviceMode == 0 && std::strstr(g_launchServerName, "Dev") != nullptr) {
        this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr_qm", true, false);
      } else {
        this->m_hotkeyScript->Load("Lua Files\\service_korea\\ExternalSettings_kr", true, false);
      }
      break;
    }
    break;
  case 1:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_usa\\ExternalSettings_usa_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_usa\\ExternalSettings_usa", true, false);
      break;
    }
    break;
  case 2:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_japan\\ExternalSettings_jp_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_japan\\ExternalSettings_jp", true, false);
      break;
    }
    break;
  case 3:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_china\\ExternalSettings_ch_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_china\\ExternalSettings_ch", true, false);
      break;
    }
    break;
  case 4:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_taiwan\\ExternalSettings_tw_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_taiwan\\ExternalSettings_tw", true, false);
      break;
    }
    break;
  case 5:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_thailand\\ExternalSettings_th_sak", true, false);
      break;
    case 13:
      this->m_hotkeyScript->Load("Lua Files\\service_thailand\\ExternalSettings_th_stage", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_thailand\\ExternalSettings_th", true, false);
      break;
    }
    break;
  case 6:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_indonesia\\ExternalSettings_id_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_indonesia\\ExternalSettings_id", true, false);
      break;
    }
    break;
  case 7:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_philippine\\ExternalSettings_ph_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_philippine\\ExternalSettings_ph", true, false);
      break;
    }
    break;
  case 12:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_brazil\\ExternalSettings_br_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_brazil\\ExternalSettings_br", true, false);
      break;
    }
    break;
  case 18:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_france\\ExternalSettings_fr_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_france\\ExternalSettings_fr", true, false);
      break;
    }
    break;
  case 15:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_vietnam\\ExternalSettings_vn_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_vietnam\\ExternalSettings_vn", true, false);
      break;
    }
    break;
  case 14:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_ru\\ExternalSettings_ru_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_ru\\ExternalSettings_ru", true, false);
      break;
    }
    break;
  case 20:
    switch (g_serviceMode) {
    case 1:
      this->m_hotkeyScript->Load("Lua Files\\service_eu\\ExternalSettings_eu_sak", true, false);
      break;
    default:
      this->m_hotkeyScript->Load("Lua Files\\service_eu\\ExternalSettings_eu", true, false);
      break;
    }
    break;
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel", &*reinterpret_cast<int*>(this->maxBaseLevel));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel3rd", &this->m_maxLevel);
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelExtend2", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x00]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelUpperJob", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x04]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelHomun", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x08]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelNovice", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x0C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelSuperNovice", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x10]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelBase", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x14]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel2nd", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x18]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel3rd", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x1C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelExtend2", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x20]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelUpperJob", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x24]));

  if (*reinterpret_cast<int*>(this->maxBaseLevel) == -1 ||
      this->m_maxLevel == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x00]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x04]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x08]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x0C]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x10]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x14]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x18]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x1C]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x20]) == -1 ||
      *reinterpret_cast<int*>(&this->jobLevelLimits[0x24]) == -1) {
    const std::array<char, 27> source = {'\x1E', '\x9B', '\xE7', '\x02', '\xF7', '\x9D', '\x18', '\x1D', '\x2F', '\x8C', '\xE7', '\x03', '\xD6', '\xD8', '\x29', '\x11', '\x32', '\x87', '\xF2', '\x1E', '\xA5', '\xEB', '\x3C', '\x09', '\x2E', '\x8C', '\x95'};
    thread_local EncryptedName<sizeof(source), 0x655DBD856D95E95BULL> name(source);
    ShowErrorDialog(name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevel4th", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x74]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevel4th", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x78]));

  if (*reinterpret_cast<int*>(&this->jobLevelLimits[0x74]) == -1 || *reinterpret_cast<int*>(&this->jobLevelLimits[0x78]) == -1) {
    const std::array<char, 27> source = {'\xC2', '\x6B', '\x53', '\x54', '\x97', '\x4F', '\xEC', '\xE5', '\xF3', '\x7C', '\x53', '\x55', '\xB6', '\x0A', '\xDD', '\xE9', '\xEE', '\x77', '\x46', '\x48', '\xC5', '\x39', '\xC8', '\xF1', '\xF2', '\x7C', '\x21'};
    thread_local EncryptedName<sizeof(source), 0x9DA96FE53B211987ULL> name(source);
    ShowErrorDialog(name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "BaseLevelDoram", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x28]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelTable", "JobLevelDoram", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x2C]));

  if (*reinterpret_cast<int*>(&this->jobLevelLimits[0x2C]) == -1) {
    const std::array<char, 27> source = {'\x84', '\x29', '\xA1', '\x34', '\x8D', '\xA7', '\xC6', '\xDF', '\xB5', '\x3E', '\xA1', '\x35', '\xAC', '\xE2', '\xF7', '\xD3', '\xA8', '\x35', '\xB4', '\x28', '\xDF', '\xD1', '\xE2', '\xCB', '\xB4', '\x3E', '\xD3'};
    thread_local EncryptedName<sizeof(source), 0xA78387FF5BD35BC1ULL> name(source);
    ShowErrorDialog(name.Text(), 0);
  }

  this->m_hotkeyScript->Call(std::string("GetTableBoolValueForC", 21), "ss>b", "MakeableRace", "Doram", &*reinterpret_cast<int*>(this->raceCreationOptions));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Default99LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x30]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Default99LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x34]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Baby99LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x38]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Baby99LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x3C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "SecondHigh99LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x40]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "SecondHigh99LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x44]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Homun99LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x48]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Homun99LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x4C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "Level99AuraTable", "Boss99LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x50]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default150LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x54]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default150LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x58]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default160LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x5C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default160LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x60]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "HomunMaxLvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x64]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "HomunMaxLvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x68]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default185LvAura", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x6C]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "Default185LvAura_sub", &*reinterpret_cast<int*>(&this->jobLevelLimits[0x70]));
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "MaxLevelEffect4th", &this->m_maxLevelEffect4th);
  this->m_hotkeyScript->Call(std::string("GetTableIntValueForC", 20), "ss>d", "MaxLevelAuraTable", "MaxLevelEffect4th_sub", &this->m_maxLevelEffect4thSub);

  if (this->m_maxLevelEffect4th == -1 || this->m_maxLevelEffect4thSub == -1) {
    const std::array<char, 27> source = {'\x66', '\x23', '\xB5', '\x54', '\xA1', '\x55', '\x94', '\xF9', '\x57', '\x34', '\xB5', '\x55', '\x80', '\x10', '\xA5', '\xF5', '\x4A', '\x3F', '\xA0', '\x48', '\xF3', '\x23', '\xB0', '\xED', '\x56', '\x34', '\xC7'};
    thread_local EncryptedName<sizeof(source), 0x81D175D33BC75123ULL> name(source);
    ShowErrorDialog(name.Text(), 0);
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

  const std::array<char, 32> keySource = {'\xC4', '\x9E', '\x87', '\xA9', '\xA6', '\xF2', '\x9B', '\x4A', '\xCB', '\xB6', '\x93', '\xBC', '\xA7', '\xFA', '\x9D', '\x56', '\xCB', '\xA3', '\x80', '\xBA', '\xBF', '\xF2', '\x9B', '\x40', '\xFC', '\xBE', '\x8F', '\xB1', '\xB8', '\xFD', '\x8C', '\x2F'};
  thread_local EncryptedName<sizeof(keySource), 0x2FE993D1DDE1F197ULL> keyName(keySource);
  HKEY key;
  if (RegCreateKeyExA(HKEY_CURRENT_USER, keyName.Text(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, nullptr) == ERROR_SUCCESS) {
    const std::array<char, 8> pathSource = {'\xAF', '\xB6', '\x2A', '\xF9', '\xD4', '\xFB', '\xFF', '\xE7'};
    thread_local EncryptedName<sizeof(pathSource), 0xE7978FB5A94DD7FDULL> pathName(pathSource);
    std::string valueName = pathName.Text();
    if (g_serviceMode == 1) {
      const std::array<char, 11> sakraySource = {'\x38', '\xD8', '\xAC', '\x7D', '\x06', '\xF6', '\xAD', '\xD6', '\x1F', '\xD1', '\xC7'};
      thread_local EncryptedName<sizeof(sakraySource), 0xB7FD8F670FC7B96BULL> sakrayName(sakraySource);
      std::string sakrayValueName = sakrayName.Text();
    }

    RegSetValueExA(key, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(directory), static_cast<DWORD>(std::strlen(directory)));
  }

  this->m_averagePing = 0;
  *reinterpret_cast<int*>(&this->itemSendState[0x08]) = 0;
  *reinterpret_cast<int*>(this->showEffects) = 1;
  this->m_aid = 0;

  SYSTEMTIME systemTime;
  GetSystemTime(&systemTime);
  const int now =
    (((((static_cast<unsigned char>(systemTime.wYear) * 12 +systemTime.wMonth) * 31 + systemTime.wDay) * 24 + systemTime.wHour + 9) * 60 + systemTime.wMinute) * 60 + systemTime.wSecond) * 1000 +
    systemTime.wMilliseconds;
  *reinterpret_cast<int*>(this->clientTimeOffset) = now - timeGetTime();
  *reinterpret_cast<int*>(this->guildTargetCharacter) = 0;
  *reinterpret_cast<int*>(&this->whisperOptions[0x00]) = 0;
  *reinterpret_cast<int*>(&this->timeAttackState[0x04]) = 0;
  *reinterpret_cast<int*>(&this->timeAttackState[0x00]) = 0;
  *reinterpret_cast<int*>(&this->timeAttackState[0x0C]) = 0;
  *reinterpret_cast<int*>(&this->timeAttackState[0x08]) = 0;
  *reinterpret_cast<int*>(&this->interactionFlags[0x00]) = 0;
  *reinterpret_cast<int*>(&this->interactionFlags[0x04]) = 0;
  *reinterpret_cast<int*>(&this->interactionFlags[0x08]) = 0;
  this->partyBlocked = 0;
  *reinterpret_cast<int*>(this->timeAttackMode) = 0;
  this->m_hotkeyPage = 0;
  *reinterpret_cast<int*>(this->whisperRecordStorage) = 0;
  this->mapPropertyId = 0;
  g_mapPropertyMgr->Clear();
  g_activeSkillEffects.clear();

  *reinterpret_cast<int*>(&this->whisperOptions[0x04]) = 0;
  *reinterpret_cast<int*>(this->petNameModified) = 0;
  this->petJob = -1;
  this->petLevel = 1;
  this->petHunger = 100;
  *reinterpret_cast<int*>(this->petMaxHunger) = 100;
  this->m_petFriendly = 100;
  this->petEggIndex = -1;
  this->petPreviousHunger = 100;
  this->monsterTalkState[0] = 0;
  *reinterpret_cast<int*>(&this->monsterTalkState[0x04]) = -1;
  std::sprintf(this->petName, "");

  std::sprintf(reinterpret_cast<char*>(&this->spAndHomunculusStats[0x0C]), "");
  int* homunStats = reinterpret_cast<int*>(&this->spAndHomunculusStats[0x2C]);
  for (int index = 0; index < 8; ++index) {
    homunStats[index] = 0;
  }
  this->m_homunJob = -1;
  *reinterpret_cast<int*>(this->homunLevel) = 1;
  this->m_homunHp = 0;
  *reinterpret_cast<int*>(this->homunMaxHp) = 999999;
  this->m_homunSp = 0;
  this->homunMaxSp = 999999;
  this->m_homunExp = 0;
  this->m_homunMaxExp = 999999;
  this->homunHunger = 100;
  this->homunMaxHunger = 100;
  this->homunFriendly = 100;
  this->homunNameModified = 0;
  this->homunAlive = 0;
  this->m_traceAi = 0;
  this->m_homunActionState = 0;
  *reinterpret_cast<int*>(&this->companionTargetState[0x00]) = 0;
  *reinterpret_cast<int*>(&this->companionTargetState[0x04]) = 0;
  this->moveHomunEnemy = 0;
  this->selectHomunEnemy = 0;
  this->attackHomunEnemy = 0;
  this->homunEnemyGid = 0;
  this->homunEnemyX = 0;
  this->homunEnemyY = 0;

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
  *reinterpret_cast<int*>(this->mercenaryAttackRange) = 0;
  this->m_mercenaryExpiration = 0;
  this->m_mercenaryActionState = 0;
  this->autoReplyState[0] = 0;

  *reinterpret_cast<DWORD*>(&this->itemSendState[0x04]) = GetTickCount();
  *reinterpret_cast<int*>(&this->characterRenameData[0x01]) = 0;
  std::sprintf(reinterpret_cast<char*>(&this->characterRenameData[0x05]), "");
  reinterpret_cast<bool&>(this->replaySessionFlag) = false;
  this->memorialWindowOpen = 0;
  this->memorialWindowX = -1;
  this->memorialWindowY = -1;
  this->memorialDungeonName = "";
  this->memorialQueuePosition = 0;
  this->memorialCreationTime = 0;
  this->memorialExpirationTime = 0;
  this->memorialDungeonState = 0;
  *reinterpret_cast<int*>(this->equipmentViewState) = 0;
  this->m_simplifiedSkillList = 0;
  this->m_showSkillDescription = 1;
  this->characterSlotState = 0;
  this->m_normalSlotCount = 0;
  this->m_premiumSlotCount = 0;
  this->m_billingSlotCount = 0;
  this->m_producibleSlotCount = 0;
  this->blockedCharacterCount = 0;
  this->m_validSlotCount = 0;
  this->m_noSubChatWnds.clear();
  this->m_subChatWnds.clear();
  this->m_hotkeyScript = nullptr;
  this->elementalGid = -1;
  this->LoadProhibitionLog();

  this->storeSearchResults.clear();
  this->storeSearchCount = 0;
  this->storeSearchPageFlags = 0;
  this->storeSearchX = -1;
  this->storeSearchY = -1;
  this->storeSearchOwnerAid = 0xFFFFFFFF;
  this->ClearPairValues(1, 1, 1);

  this->m_lockItemDrop = false;
  for (std::list<ITEM_INFO>& items : this->inventoryCategories) {
    items.erase(items.begin(), items.end());
  }

  this->replayPacketMode = 0;
  *reinterpret_cast<int*>(&this->sellSoundState[0x03]) = 0;
  this->m_skipPriceWarning = true;
  *reinterpret_cast<int*>(&this->itemSelectionState[0x00]) = 0;
  *reinterpret_cast<int*>(&this->itemSelectionState[0x04]) = 0;
  *reinterpret_cast<int*>(&this->itemSelectionState[0x08]) = 0;
  this->mainShortcutRow = 0;
  this->alternateShortcutRow = 0;
  for (ITEM_INFO& item : this->selectionItems) {
    item.Reset();
  }

  this->achievementResult = 0;
  *reinterpret_cast<int*>(&this->itemDropState[0x02]) = 0;
  *reinterpret_cast<int*>(&this->itemDropState[0x06]) = 0;
  *reinterpret_cast<int*>(&this->itemDropState[0x0A]) = 0;
  *reinterpret_cast<int*>(&this->itemDropState[0x0E]) = 1;
  this->m_muteSellSound = true;
  this->ClearAchievements();
  this->playerAppearanceState = 0;
  this->m_bodyCostume = 0;
  this->m_lastIntervalTime = timeGetTime();

  this->automaticSkillHotkey = 0;
  this->m_currentHotkey = 0;
  this->petGid = 0xFFFFFFFF;
  *reinterpret_cast<int*>(&this->spAndHomunculusStats[0x08]) = -1;
  *reinterpret_cast<int*>(&this->companionTargetState[0x08]) = -1;
  this->m_guildMemberListSort = true;
  this->m_blockPrivateTab = false;
  this->swapEquipmentFlag[0] = 0;
  this->m_itemDefaultSort = false;
  this->m_itemAscendingOrder = false;
  this->m_itemDescendingOrder = false;
  this->m_showMenuIcon = true;
  this->m_homunculusAutoFeed = false;
  *reinterpret_cast<int*>(&this->homunculusFeedOptions[0x03]) = -1;
  *reinterpret_cast<int*>(&this->homunculusFeedOptions[0x07]) = 50;
  this->homunculusFeedOptions[0x0B] = 0;
  this->m_showRankingSchedule = -1;
  *reinterpret_cast<int*>(this->feedOptions) = 0;
}

// OFFSET: 0x00DF7C40
// FUNCTION: fn_23508
void CSession::ResetCharacterState() {
  *reinterpret_cast<float*>(&this->m_navigationPlayerMap[0x18]) = SquareRoot(50.0f);

  const int randomValue = std::max(0, std::rand() % 1024) + 1;

  this->m_zeny = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x04]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x08]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x20]) = 0;
  this->m_maxWeight = 0;
  this->m_weight = 0;

  if (this->maxHp != randomValue) {
    this->maxHp = randomValue;
    this->RefreshHpSpWindows();
  }

  if (this->hp != randomValue) {
    this->hp = randomValue;
    this->RefreshHpSpWindows();
  }

  this->m_zeny = randomValue;
  this->m_level = randomValue;
  this->m_jobLevel = randomValue;

  std::memcpy(this->m_playerNameKey, "잠이들었던거야", sizeof("잠이들었던거야"));

  this->intelligenceBonus = 0;
  this->dexterityBonus = 0;
  this->luckBonus = 0;
  this->strengthBonus = 0;
  this->agilityBonus = 0;
  this->vitalityBonus = 0;

  *reinterpret_cast<int*>(&this->combatStats[0x00]) = 2;
  *reinterpret_cast<int*>(&this->combatStats[0x04]) = 2;
  *reinterpret_cast<int*>(&this->combatStats[0x14]) = 2;
  *reinterpret_cast<int*>(&this->combatStats[0x08]) = 2;
  *reinterpret_cast<int*>(&this->combatStats[0x0C]) = 2;
  *reinterpret_cast<int*>(&this->combatStats[0x10]) = 2;

  *reinterpret_cast<int*>(&this->playerExperienceState[0x28]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x30]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x34]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x38]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x28]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x2C]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x20]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x3C]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x40]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x44]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x4C]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x48]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x18]) = 0;
  *reinterpret_cast<int*>(&this->combatStats[0x24]) = 0;

  *reinterpret_cast<int*>(&this->playerExperienceState[0x04]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x08]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x1C]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x20]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x0C]) = 999999;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x10]) = 0;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x14]) = 999999;
  *reinterpret_cast<int*>(&this->playerExperienceState[0x18]) = 0;

  this->cartCount = 0;
  this->cartMaxCount = 0;
  this->cartWeight = 0;
  this->cartMaxWeight = 0;
  *reinterpret_cast<int*>(&this->storeSlotCounts[0x00]) = 13;
  *reinterpret_cast<int*>(&this->storeSlotCounts[0x04]) = 5;
  this->playerTeam[0] = 0;
  this->m_score1 = -1;
  this->m_score2 = -1;
  this->chatFont[0] = 0;
  this->storeItemLimit = 20;
  this->selectedCharacterSlot = 0;

  this->equippedArrowIndex = 0;
  this->partyExpShare = 0;
  this->partyItemCollect = 0;
  this->partyItemDistribute = 0;
  this->monsterInfoEnabled = 1;
  this->quitBlocked = 0;
  this->battleChat = 0;
  this->quitAllowed = 0;
  this->sessionFlag = 0;
  *reinterpret_cast<int*>(&g_session.mapTransitionFlag[0x00]) = 0;
  *reinterpret_cast<std::uint16_t*>(&this->playerStatusFlags[0x08]) = 0;
  this->adoptionSkillAvailable = 0;
  this->loginOptions[0] = 1;
  this->guildLeader = 0;
  this->showTimer = 0;
  this->mouseAndChannelOptions[0] = 1;

  MEMORYSTATUS memoryStatus = {};
  memoryStatus.dwLength = sizeof(memoryStatus);
  GlobalMemoryStatus(&memoryStatus);
  this->lowMemory = memoryStatus.dwTotalPhys < 0x9B72EC0 ? 1 : 0;

  std::memset(&this->characterSelectionData[0x40], 0, 0x660);
  this->replayPacketMode = 0;

  std::map<int, int>& traitStats = this->m_traits.stats;
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
    this->equippedItems[index].Reset();
  }
}

// OFFSET: 0x00DF8940
// FUNCTION: fn_23512
void CSession::ClearItemContainers() {
  this->purchaseItems.erase(this->purchaseItems.begin(), this->purchaseItems.end());
  this->storageItems.erase(this->storageItems.begin(), this->storageItems.end());
  this->shopItems.erase(this->shopItems.begin(), this->shopItems.end());

  this->exchangeMoney = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_exchangeItems[index].quantity = 0;
  }

  this->otherExchangeMoney = 0;

  for (int index = 0; index < 10; ++index) {
    this->m_itemSlots[index].quantity = 0;
  }

  this->cartItems.erase(this->cartItems.begin(), this->cartItems.end());

  g_session.m_skillBundle.skills.clear();
  this->m_friends.erase(this->m_friends.begin(), this->m_friends.end());

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode != nullptr) {
    for (std::list<MessengerMember>::iterator it = this->m_partyMembers.begin(); it != this->m_partyMembers.end(); ++it) {
      CRenderObject* actor = mode->scene->GetGameActorByAidIncludeMe(it->accountId);
      if (actor != nullptr && static_cast<EffectActor*>(actor)->HasId(160) == true) {
        actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
    }
  }

  this->m_partyMembers.clear();
  this->partyName = "";
  this->partyLeader = 0;
  this->ClearItemQuantities();
}

// OFFSET: 0x00DF3160
// FUNCTION: fn_23503
void CSession::ClearStoreSearchResults() {
  this->storeSearchResults.clear();
  this->storeSearchCount = 0;
  this->storeSearchPageFlags = 0;
  this->storeSearchX = -1;
  this->storeSearchY = -1;
  this->storeSearchOwnerAid = 0xFFFFFFFF;
}

// OFFSET: 0x00D8D9E0
// FUNCTION: fn_22269
void CSession::ClearAchievements() {
  this->achievements.clear();
  this->achievementLevel = 0;
  this->achievementCompleted = 0;
  this->achievementTotal = 0;
  this->jobTitles.clear();
  this->achievementScore = 0;
  this->achievementIds.clear();
  this->jobTitle = 0;
  this->trackedAchievements.clear();
}

namespace {
struct ParameterNameLess {
  // INLINED
  bool operator()(const std::pair<std::string, std::vector<std::string>>& entry, const std::string& name) const {
    return std::strcmp(entry.first.c_str(), name.c_str()) < 0;
  }
};
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
  auto it = std::lower_bound(this->m_fogParameters.begin(), this->m_fogParameters.end(), key, ParameterNameLess());
  if (it != this->m_fogParameters.end() && std::strcmp(it->first.c_str(), name.c_str()) == 0) {
    *found = 1;
    return &it->second;
  }

  return &s_defaultParameters;
}

// OFFSET: 0x00DE3410
// FUNCTION: fn_23479
const std::vector<std::string>* CSession::GetMapViewParameters(const char* rswName, int* found) {
  std::vector<std::pair<std::string, std::vector<std::string>>>& parameters = this->m_mapViewParameters;
  *found = 0;
  // GLOBAL: gl_24993
  static std::vector<std::string> s_defaultParameters(9);

  std::string name = rswName;
  const std::size_t position = name.rfind("@");
  if (position != std::string::npos && position >= 4) {
    name.erase(position - 4, 3);
  }

  const char* key = name.c_str();
  auto it = std::lower_bound(parameters.begin(), parameters.end(), key, ParameterNameLess());
  if (it != parameters.end() && std::strcmp(it->first.c_str(), name.c_str()) == 0) {
    *found = 1;
    return &it->second;
  }

  return &s_defaultParameters;
}

// OFFSET: 0x00DF8150
// FUNCTION: fn_23509
bool CSession::LoadMapViewParameters() {
  this->m_mapViewParameters.clear();

  CFile file;
  if (!file.Open("ViewPointTable.txt", 0, true, false)) {
    return false;
  }

  const char* cursor = static_cast<const char*>(file.GetData());
  const char* end = cursor + file.GetSize();
  std::vector<std::string> values;
  std::string name;

  for (const char* start = cursor; cursor < end; ++cursor) {
    const char value = *cursor;
    if (value == '\n' || value == '\r') {
      start = cursor + 1;
      continue;
    }

    if (value != '#') {
      continue;
    }

    if (name.empty()) {
      name.assign(start, cursor - start);
    } else if (values.size() < 8) {
      values.push_back(std::string(start, cursor));
    } else {
      values.push_back(std::string(start, cursor));
      this->m_mapViewParameters.push_back(std::make_pair(name, values));
      name.clear();
      values.clear();
    }

    start = cursor + 1;
  }

  std::sort(this->m_mapViewParameters.begin(), this->m_mapViewParameters.end(), SessionParameterNameLess());
  file.Close();
  return true;
}

// OFFSET: 0x00DF8670
// FUNCTION: fn_23510
void CSession::InitWeaponHitWaveNames() {
  std::vector<std::string>& weaponWaves = reinterpret_cast<std::vector<std::string>&>(this->weaponSoundTables[0x18]);
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
  std::vector<const char*>& names = this->m_jobKeyNames;
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
  this->m_commandIds.push_back({"/help", 14});
  this->m_commandIds.push_back({"/h", 14});
  this->m_commandIds.push_back({"/killall", 5});
  this->m_commandIds.push_back({"/wjqthrwk", 6});
  this->m_commandIds.push_back({"/snrn", 6});
  this->m_commandIds.push_back({"/who", 6});
  this->m_commandIds.push_back({"/w", 6});
  this->m_commandIds.push_back({"/dmadkr", 7});
  this->m_commandIds.push_back({"/bgm", 7});
  this->m_commandIds.push_back({"/effect", 8});
  this->m_commandIds.push_back({"/dlvprxm", 8});
  this->m_commandIds.push_back({"/sound", 11});
  this->m_commandIds.push_back({"/tingonly", 195});
  this->m_commandIds.push_back({"/gyrhkdma", 11});
  this->m_commandIds.push_back({"/skip", 10});
  this->m_commandIds.push_back({"/ex", 15});
  this->m_commandIds.push_back({"/inall", 17});
  this->m_commandIds.push_back({"/exall", 16});
  this->m_commandIds.push_back({"/sit", 22});
  this->m_commandIds.push_back({"/dkswrl", 22});
  this->m_commandIds.push_back({"/stand", 22});
  this->m_commandIds.push_back({"/tjrl", 22});
  this->m_commandIds.push_back({"/coxldqkd", 23});
  this->m_commandIds.push_back({"/chat", 23});
  this->m_commandIds.push_back({"/xkfxhl", 27});
  this->m_commandIds.push_back({"/leave", 27});
  this->m_commandIds.push_back({"/stat", 2});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SURPRISE), 29});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_QUESTION), 30});
  this->m_commandIds.push_back({"/delight", 31});
  this->m_commandIds.push_back({"/ghk", 36});
  this->m_commandIds.push_back({"/fret", 35});
  this->m_commandIds.push_back({"/Wkwmd", 35});
  this->m_commandIds.push_back({"/q", 24});
  this->m_commandIds.push_back({"/rlQma", 31});
  this->m_commandIds.push_back({"/heart", 32});
  this->m_commandIds.push_back({"/gkxm", 32});
  this->m_commandIds.push_back({"/sweat", 33});
  this->m_commandIds.push_back({"/Eka", 33});
  this->m_commandIds.push_back({"/wnajr", 40});
  this->m_commandIds.push_back({"/qh", 41});
  this->m_commandIds.push_back({"/bo", 41});
  this->m_commandIds.push_back({"/rkdnl", 39});
  this->m_commandIds.push_back({"/gawi", 39});
  this->m_commandIds.push_back({"/bawi", 40});
  this->m_commandIds.push_back({"/qkdnl", 40});
  this->m_commandIds.push_back({"/whkvy", 9});
  this->m_commandIds.push_back({"/wkdth", 9});
  this->m_commandIds.push_back({"/where", 9});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_THINK), 38});
  this->m_commandIds.push_back({"/money", 37});
  this->m_commandIds.push_back({"/ehs", 37});
  this->m_commandIds.push_back({"/anger", 36});
  this->m_commandIds.push_back({"/aha", 34});
  this->m_commandIds.push_back({"/dkgk", 34});
  this->m_commandIds.push_back({"/tip", 42});
  this->m_commandIds.push_back({"/rldjr", 43});
  this->m_commandIds.push_back({"/memo", 43});
  this->m_commandIds.push_back({"/snap", 46});
  this->m_commandIds.push_back({"/tmsoq", 46});
  this->m_commandIds.push_back({"/skillsnap", 45});
  this->m_commandIds.push_back({"/tmzlftmsoq", 45});
  this->m_commandIds.push_back({"/itemsnap", 47});
  this->m_commandIds.push_back({"/dkdlxpatmsoq", 47});
  this->m_commandIds.push_back({"/rkfanfl", 51});
  this->m_commandIds.push_back({"/savechat", 51});
  this->m_commandIds.push_back({"/miss", 53});
  if (g_serviceMode != 3) {
    this->m_commandIds.push_back({"/noshift", 105});
    this->m_commandIds.push_back({"/noctrl", 109});
    this->m_commandIds.push_back({"/ns", 105});
    this->m_commandIds.push_back({"/nc", 109});
  }
  this->m_commandIds.push_back({"/window", 141});
  this->m_commandIds.push_back({"/wi", 141});
  this->m_commandIds.push_back({"/aura", 106});
  this->m_commandIds.push_back({"/aura2", 107});
  this->m_commandIds.push_back({"/doridori", 110});
  this->m_commandIds.push_back({"/bingbing", 111});
  this->m_commandIds.push_back({"/bangbang", 112});
  this->m_commandIds.push_back({"/skillfail", 113});
  this->m_commandIds.push_back({"/sf", 113});
  this->m_commandIds.push_back({"/loginout", 165});
  this->m_commandIds.push_back({"/li", 165});
  this->m_commandIds.push_back({"/shopping", 167});
  this->m_commandIds.push_back({"/sh", 167});
  this->m_commandIds.push_back({"/quickspell", 120});
  this->m_commandIds.push_back({"/q1", 120});
  this->m_commandIds.push_back({"/quickspell2", 121});
  this->m_commandIds.push_back({"/q2", 121});
  this->m_commandIds.push_back({"/q3", 122});
  this->m_commandIds.push_back({"/notalkmsg", 114});
  this->m_commandIds.push_back({"/nm", 114});
  this->m_commandIds.push_back({"/notalkmsg2", 148});
  this->m_commandIds.push_back({"/nm2", 148});
  this->m_commandIds.push_back({"/notrade", 136});
  this->m_commandIds.push_back({"/nt", 136});
  this->m_commandIds.push_back({"/set1", 115});
  this->m_commandIds.push_back({"/set2", 138});
  this->m_commandIds.push_back({"/set3", 166});
  this->m_commandIds.push_back({"/camera", 54});
  this->m_commandIds.push_back({"/tp", 59});
  this->m_commandIds.push_back({"/fog", 60});
  this->m_commandIds.push_back({"/ㅗ", 14});
  this->m_commandIds.push_back({"/채팅방", 23});
  this->m_commandIds.push_back({"/촘ㅅ", 23});
  this->m_commandIds.push_back({"/접속자", 6});
  this->m_commandIds.push_back({"/누구", 6});
  this->m_commandIds.push_back({"/ㅈ", 6});
  this->m_commandIds.push_back({"/좨", 6});
  this->m_commandIds.push_back({"/음악", 7});
  this->m_commandIds.push_back({"/ㅠ흐", 7});
  this->m_commandIds.push_back({"/이펙트", 8});
  this->m_commandIds.push_back({"/효과음", 11});
  this->m_commandIds.push_back({"/나ㅑㅔ", 10});
  this->m_commandIds.push_back({"/차단", 15});
  this->m_commandIds.push_back({"/해제전부", 17});
  this->m_commandIds.push_back({"/차단전부", 16});
  this->m_commandIds.push_back({"/앉기", 22});
  this->m_commandIds.push_back({"/냣", 22});
  this->m_commandIds.push_back({"/서기", 22});
  this->m_commandIds.push_back({"/ㄴㅅ뭉", 22});
  this->m_commandIds.push_back({"/ㅂ", 24});
  this->m_commandIds.push_back({"/탈퇴", 27});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_DELIGHT), 31});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_THROB), 32});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SWEAT), 33});
  this->m_commandIds.push_back({"/주먹", 40});
  this->m_commandIds.push_back({"/보", 41});
  this->m_commandIds.push_back({"/가위", 39});
  this->m_commandIds.push_back({"/바위", 40});
  this->m_commandIds.push_back({"/좌표", 9});
  this->m_commandIds.push_back({"/장소", 9});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_MONEY), 37});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_ANGER), 36});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_FRET), 35});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_AHA), 34});
  this->m_commandIds.push_back({"/샤ㅔ", 42});
  this->m_commandIds.push_back({"/기억", 43});
  this->m_commandIds.push_back({"/누메", 46});
  this->m_commandIds.push_back({"/스냅", 46});
  this->m_commandIds.push_back({"/나ㅑㅣㅣ누메", 45});
  this->m_commandIds.push_back({"/스킬스냅", 45});
  this->m_commandIds.push_back({"/ㅑㅅ드누메", 47});
  this->m_commandIds.push_back({"/아이템스냅", 47});
  this->m_commandIds.push_back({"/갈무리", 51});
  this->m_commandIds.push_back({"/autosavechat", 104});
  this->m_commandIds.push_back({"/objlight", 65});
  this->m_commandIds.push_back({"/loading", 145});
  this->m_commandIds.push_back({"/minigame", 173});
  this->m_commandIds.push_back({"/battlemode", 135});
  this->m_commandIds.push_back({"/bm", 135});
  this->m_commandIds.push_back({"/battlechat", 186});
  this->m_commandIds.push_back({"/전장", 186});
  this->m_commandIds.push_back({"/자동응답", 153});
  this->m_commandIds.push_back({"/automsg", 153});
  this->m_commandIds.push_back({"/am", 153});
  this->m_commandIds.push_back({"/traceai", 162});
  this->m_commandIds.push_back({"/호문지능", 164});
  this->m_commandIds.push_back({"/hoai", 164});
  this->m_commandIds.push_back({"/용병지능", 174});
  this->m_commandIds.push_back({"/merai", 174});
  this->m_commandIds.push_back({"/상태정보", 201});
  this->m_commandIds.push_back({"/stateinfo", 201});
  this->m_commandIds.push_back({"/monsterhp", 213});
  this->m_commandIds.push_back({"/ㅡㅐㅜㄴㅅㄷ고ㅔ", 213});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_BIGTHROB), 79});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_THANKS), 66});
  this->m_commandIds.push_back({"/ㄳ", 66});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_KEK), 67});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SORRY), 68});
  this->m_commandIds.push_back({"/sorry", 68});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SMILE), 69});
  this->m_commandIds.push_back({"/하하", 69});
  this->m_commandIds.push_back({"/ㅇ", 69});
  this->m_commandIds.push_back({"/smile", 69});
  this->m_commandIds.push_back({"/ㄱ", 31});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_PROFUSELY_SWEAT), 70});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SCRATCH), 71});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_BEST), 72});
  this->m_commandIds.push_back({"/원츄", 72});
  this->m_commandIds.push_back({"/ㅊ", 72});
  this->m_commandIds.push_back({"/emotion", 86});
  this->m_commandIds.push_back({"/lightmap", 119});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_STARE_ABOUT), 73});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HUK), 74});
  this->m_commandIds.push_back({"/ㅎ", 74});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_O), 75});
  this->m_commandIds.push_back({"/o", 75});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_X), 76});
  this->m_commandIds.push_back({"/x", 76});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HELP), 77});
  this->m_commandIds.push_back({"/ㅊㅊ", 77});
  this->m_commandIds.push_back({"/help", 77});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_GO), 78});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_CRY), 80});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_KIK), 81});
  this->m_commandIds.push_back({"/킥", 81});
  this->m_commandIds.push_back({"/ㅋ", 81});
  this->m_commandIds.push_back({"/케케", 81});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_CHUP), 82});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_CHUPCHUP), 83});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HNG), 84});
  this->m_commandIds.push_back({"/끄덕", 85});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_OK), 85});
  this->m_commandIds.push_back({"/이모션", 86});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_STARE), 123});
  this->m_commandIds.push_back({"/ㅉ", 123});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HUNGRY), 124});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_COOL), 125});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_MERONG), 126});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SHY), 127});
  this->m_commandIds.push_back({"/부", 127});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_GOODBOY), 128});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SPTIME), 129});
  this->m_commandIds.push_back({"/엠", 129});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SEXY), 130});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_COMEON), 131});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SLEEPY), 132});
  this->m_commandIds.push_back({"/ㅈ", 132});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_CONGRATULATION), 133});
  this->m_commandIds.push_back({"/ㅊㅋ", 133});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HPTIME), 134});
  this->m_commandIds.push_back({"/피", 134});
  this->m_commandIds.push_back({"/e1", 123});
  this->m_commandIds.push_back({"/e2", 124});
  this->m_commandIds.push_back({"/e3", 125});
  this->m_commandIds.push_back({"/e4", 126});
  this->m_commandIds.push_back({"/e5", 127});
  this->m_commandIds.push_back({"/e6", 128});
  this->m_commandIds.push_back({"/e7", 129});
  this->m_commandIds.push_back({"/e8", 130});
  this->m_commandIds.push_back({"/e9", 131});
  this->m_commandIds.push_back({"/e10", 132});
  this->m_commandIds.push_back({"/e11", 133});
  this->m_commandIds.push_back({"/e12", 134});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_SPARK), 154});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_CONFUSE), 155});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_OHNO), 156});
  this->m_commandIds.push_back({"/한숨", 156});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_HUM), 157});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_BLABLA), 158});
  this->m_commandIds.push_back({"/시끌시끌", 158});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_OTL), 159});
  this->m_commandIds.push_back({LookupMessage(MSI_EMOTION_DICE), 160});
  this->m_commandIds.push_back({"/dice", 160});
  this->m_commandIds.push_back({"/e19", 160});
  this->m_commandIds.push_back({"/e13", 154});
  this->m_commandIds.push_back({"/e14", 155});
  this->m_commandIds.push_back({"/e15", 156});
  this->m_commandIds.push_back({"/e16", 157});
  this->m_commandIds.push_back({"/e17", 158});
  this->m_commandIds.push_back({"/e18", 159});
  this->m_commandIds.push_back({"/otl", 159});
  this->m_commandIds.push_back({"/러브", 171});
  this->m_commandIds.push_back({"/e20", 171});
  this->m_commandIds.push_back({"/e27", 187});
  this->m_commandIds.push_back({"/e28", 188});
  this->m_commandIds.push_back({"/e29", 189});
  this->m_commandIds.push_back({"/e30", 190});
  this->m_commandIds.push_back({"/e31", 191});
  this->m_commandIds.push_back({"/e32", 192});
  this->m_commandIds.push_back({"/e33", 193});
  this->m_commandIds.push_back({"/흠", 187});
  this->m_commandIds.push_back({"/멍", 188});
  this->m_commandIds.push_back({"/웁", 189});
  this->m_commandIds.push_back({"/웩", 190});
  this->m_commandIds.push_back({"/탈력", 191});
  this->m_commandIds.push_back({"/공황", 192});
  this->m_commandIds.push_back({"/위스퍼", 193});
  this->m_commandIds.push_back({"/hum", 187});
  this->m_commandIds.push_back({"/abs", 188});
  this->m_commandIds.push_back({"/oops", 189});
  this->m_commandIds.push_back({"/spit", 190});
  this->m_commandIds.push_back({"/ene", 191});
  this->m_commandIds.push_back({"/panic", 192});
  this->m_commandIds.push_back({"/whisp", 193});
  this->m_commandIds.push_back({"/hw", 163});
  this->m_commandIds.push_back({"/ㅗㅈ", 163});
  this->m_commandIds.push_back({"/blacksmith", 146});
  this->m_commandIds.push_back({"/alchemist", 147});
  this->m_commandIds.push_back({"/taekwon", 152});
  this->m_commandIds.push_back({"/lv", 149});
  this->m_commandIds.push_back({"/sp", 150});
  this->m_commandIds.push_back({"/pk", 161});
  this->m_commandIds.push_back({"/cn", 181});
  this->m_commandIds.push_back({"/초대거부", 183});
  this->m_commandIds.push_back({"/초대수락", 182});
  this->m_commandIds.push_back({"/refuse", 183});
  this->m_commandIds.push_back({"/accept", 182});
  this->m_commandIds.push_back({"/장비공개", 185});
  this->m_commandIds.push_back({"/wkdqlrhdro", 185});
  this->m_commandIds.push_back({"/eqopen", 185});
  this->m_commandIds.push_back({"/show_addressbar", 205});
  this->m_commandIds.push_back({"/buildinfo", 207});
  this->m_commandIds.push_back({LookupMessage(MSI_GUILD_MEMBERLIST_SORT), 223});
  this->m_commandIds.push_back({"/viewpointvalue", 227});
  this->m_commandIds.push_back({"/mable", 240});
  this->m_commandIds.push_back({LookupMessage(MSI_CHECK_LEVEL_BEFORE_REGISTER_AGENCY), 241});
  this->m_commandIds.push_back({LookupMessage(MSI_RUSSIA_PERSONALINFO_EXP), 243});
  std::stable_sort(this->m_commandIds.begin(), this->m_commandIds.end(),
                   [](const std::pair<const char*, int>& left, const std::pair<const char*, int>& right) {
                     return std::strcmp(left.first, right.first) < 0;
                   });
  std::vector<std::pair<const char*, int>>& chatCommands =
    reinterpret_cast<std::vector<std::pair<const char*, int>>&>(this->commandTables[0x00]);
  chatCommands.push_back({"/taekwon", 152});
  chatCommands.push_back({"/hi", 151});
  chatCommands.push_back({"/blacksmith", 146});
  chatCommands.push_back({"/alchemist", 147});
  chatCommands.push_back({"/help", 14});
  chatCommands.push_back({"/sit", 22});
  chatCommands.push_back({"/stand", 22});
  chatCommands.push_back({"/q", 24});
  chatCommands.push_back({"/leave", 27});
  chatCommands.push_back({"/where", 9});
  chatCommands.push_back({"/memo", 43});
  chatCommands.push_back({"/chat", 23});
  chatCommands.push_back({"/savechat", 51});
  chatCommands.push_back({"/doridori", 110});
  chatCommands.push_back({"/bangbang", 112});
  chatCommands.push_back({"/bingbing", 111});
  chatCommands.push_back({"/ex", 15});
  chatCommands.push_back({"/inall", 17});
  chatCommands.push_back({"/exall", 16});
  std::vector<std::pair<const char*, int>>& windowCommands =
    reinterpret_cast<std::vector<std::pair<const char*, int>>&>(this->commandTables[0x0C]);
  if (g_serviceType == 12) {
    windowCommands.push_back({"Sala de Chat", 67});
    windowCommands.push_back({"Atributos", 65});
    windowCommands.push_back({"Detalhes", 68});
    windowCommands.push_back({"Configura\xE7\xE3o", 79});
    windowCommands.push_back({"Invent?io", 69});
    windowCommands.push_back({"Carrinho", 87});
    windowCommands.push_back({"Habilidades", 83});
    windowCommands.push_back({"Equipamentos", 81});
    windowCommands.push_back({"Inf. B?icas", 86});
    windowCommands.push_back({"Cl?", 71});
    windowCommands.push_back({"Conf. Grupo", 80});
    windowCommands.push_back({"Conf. Contatos", 73});
    windowCommands.push_back({"Grupo", 90});
    windowCommands.push_back({"Contatos", 72});
    windowCommands.push_back({"Atalhos", 77});
    windowCommands.push_back({"Lista Emoticons", 76});
    windowCommands.push_back({"Bicho de Est.", 74});
  } else {
    windowCommands.push_back({"Chat", 67});
    windowCommands.push_back({"Status", 65});
    windowCommands.push_back({"DetailLevel", 68});
    windowCommands.push_back({"Option", 79});
    windowCommands.push_back({"Items", 69});
    windowCommands.push_back({"Merchant", 87});
    windowCommands.push_back({"Skill", 83});
    windowCommands.push_back({"Equipment", 81});
    windowCommands.push_back({"BasicInfo", 86});
    windowCommands.push_back({"Guild", 71});
    windowCommands.push_back({"PartySetting", 80});
    windowCommands.push_back({"FriendOption", 73});
    windowCommands.push_back({"Messenger", 90});
    windowCommands.push_back({"WhisperList", 72});
    windowCommands.push_back({"Emotion", 77});
    windowCommands.push_back({"EmotionList", 76});
    windowCommands.push_back({"PetInfo", 74});
  }
  std::vector<const char*>& shortcutCommands = reinterpret_cast<std::vector<const char*>&>(this->commandTables[0x18]);
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

// OFFSET: 0x00DC2710
// FUNCTION: fn_22757
SessionTraits::SessionTraits() {
}

// OFFSET: 0x00DC2B20
// FUNCTION: fn_22763
CSession::CSession() {
  this->m_priorityClass = 32;
  *reinterpret_cast<std::uint16_t*>(&this->feedOptions[4]) = 0x100;
  this->sessionIndex = -1;
  this->skyMapNames.clear();

  CItemInfoMgr::Create();
  CSpecialPopupMgr::Create();
  CRuneSystemMgr::Create();
  if (!CUserHotkeyMgr::Initialize()) {
    const std::array<char, 6> captionSource = {'\x3A', '\x61', '\xAD', '\xBE', '\xF7', '\x67'};
    thread_local EncryptedName<sizeof(captionSource), 0xED796785D1DF137FULL> caption(captionSource);
    const char* captionText = caption.Text();
    const std::array<char, 33> textSource = {'\xB2', '\x92', '\x4A', '\x6C', '\x65', '\x31', '\x20', '\x85', '\x9A', '\xA2', '\x40', '\x44', '\x70', '\x0B', '\x75', '\xCB', '\xB2', '\xB5', '\x5C', '\x68', '\x63', '\x1C', '\x06', '\x9F', '\x82', '\xB3', '\x58', '\x67', '\x74', '\x1C', '\x67', '\xD8', '\xF1'};
    thread_local EncryptedName<sizeof(textSource), 0xF14F79170939C7F1ULL> text(textSource);
    MessageBoxA(nullptr, text.Text(), captionText, MB_OK);
  }
  if (!CBarterMarketItemMgr::CreateInstance()) {
    const std::array<char, 6> captionSource = {'\xE2', '\x87', '\x0D', '\x56', '\x43', '\x4B'};
    thread_local EncryptedName<sizeof(captionSource), 0xBF2B4B31397FF5A7ULL> caption(captionSource);
    const char* captionText = caption.Text();
    const std::array<char, 47> textSource = {'\x40', '\x49', '\x5C', '\xCF', '\xDC', '\x33', '\x1E', '\xBC', '\x4A', '\x65', '\x4E', '\xC9', '\xC9', '\x38', '\x0F', '\x94', '\x68', '\x55', '\x44', '\xDE', '\xC9', '\x14', '\x09', '\xBC', '\x64', '\x6A', '\x48', '\xC9', '\x87', '\x67', '\x3E', '\xAB', '\x6C', '\x46', '\x5B', '\xDE', '\xF4', '\x33', '\x0E', '\xAD', '\x68', '\x49', '\x4C', '\xDE', '\x95', '\x74', '\x7D'};
    thread_local EncryptedName<sizeof(textSource), 0xD97D5DBDBB2F2709ULL> text(textSource);
    MessageBoxA(nullptr, text.Text(), captionText, MB_OK);
  }
  if (!CExpandedBarterMarketItemMgr::CreateInstance()) {
    const std::array<char, 6> captionSource = {'\xDE', '\x25', '\x71', '\xF6', '\x15', '\x05'};
    thread_local EncryptedName<sizeof(captionSource), 0x050505679903579BULL> caption(captionSource);
    const char* captionText = caption.Text();
    const std::array<char, 48> textSource = {'\xF4', '\xD3', '\x9E', '\x43', '\xDA', '\xD1', '\x8C', '\xE6', '\xFE', '\xFF', '\x8C', '\x45', '\xCF', '\xDA', '\x9D', '\xCE', '\xDC', '\xCF', '\x86', '\x52', '\xCF', '\xF6', '\x9B', '\xE6', '\xD0', '\x8F', '\xA0', '\x50', '\xC9', '\x85', '\xD5', '\xC0', '\xCF', '\xD8', '\x8C', '\x43', '\xDE', '\xF6', '\x81', '\xF0', '\xC9', '\xDC', '\x83', '\x54', '\xDE', '\x97', '\xC6', '\x83'};
    thread_local EncryptedName<sizeof(textSource), 0x83EFBFBB37EDBDBDULL> text(textSource);
    MessageBoxA(nullptr, text.Text(), captionText, MB_OK);
  }
  if (!CPetEvolutionMgr::Create()) {
    const std::array<char, 6> captionSource = {'\x88', '\x83', '\xEB', '\xDA', '\xFF', '\x5B'};
    thread_local EncryptedName<sizeof(captionSource), 0xAD075B8DB599F1CDULL> caption(captionSource);
    const char* captionText = caption.Text();
    const std::array<char, 27> textSource = {'\xA2', '\x2B', '\x9C', '\xA3', '\x90', '\x65', '\x56', '\xB1', '\x94', '\x0F', '\x90', '\xB8', '\xBB', '\x5E', '\x5E', '\xAF', '\xC1', '\x1D', '\x90', '\xBB', '\xB0', '\x33', '\x70', '\xB3', '\x88', '\x0F', '\xF9'};
    thread_local EncryptedName<sizeof(textSource), 0xDD3913D5D7F97BE1ULL> text(textSource);
    MessageBoxA(nullptr, text.Text(), captionText, MB_OK);
  }
  if (!CAchievementMgr::CreateInstance()) {
    const std::array<char, 6> captionSource = {'\xCA', '\xEB', '\x99', '\x62', '\xC7', '\x7F'};
    thread_local EncryptedName<sizeof(captionSource), 0x39FF7FB50DEB998FULL> caption(captionSource);
    const char* captionText = caption.Text();
    const std::array<char, 26> textSource = {'\x3C', '\x54', '\xEE', '\x79', '\x5E', '\x6A', '\xFD', '\x4A', '\x12', '\x70', '\xE3', '\x65', '\x7A', '\x68', '\xF9', '\x0F', '\x19', '\x7C', '\xE1', '\x74', '\x17', '\x66', '\xE5', '\x46', '\x0B', '\x15'};
    thread_local EncryptedName<sizeof(textSource), 0x2F8B0F37118D157FULL> text(textSource);
    MessageBoxA(nullptr, text.Text(), captionText, MB_OK);
  }

  this->m_newRobeDrawOnTop = false;
  this->m_localMapMove = 0;
  this->m_swapEquipmentMode = 0;
  this->raceCreationOptions[0] = 1;
  *reinterpret_cast<int*>(&this->raceCreationOptions[4]) = 0;
  CPrivateAirplaneMgr::Create();
  CCheckAttendanceMgr::Create();
  *reinterpret_cast<int*>(&this->equipmentViewState[0x28]) = 0;
  this->extraInventorySlots = 0;
  CMapInfoMgr::Create();
  this->madogearType = 0;
  *reinterpret_cast<int*>(&this->jobLevelLimits[0x74]) = 0;
  *reinterpret_cast<int*>(&this->jobLevelLimits[0x78]) = 0;
  CChangeMaterialMgr::CreateInstance();
  this->m_options.Clear();
  *reinterpret_cast<int*>(&this->webServiceLoginData[0x18]) = 0;
  this->m_damageSkinIndex = 0;
  this->m_damageSkinMode = 0;
  this->m_damageMillionUnit = 0;
  this->m_navigationPlayerMap[0] = '\0';
}

// OFFSET: 0x00DE7290
// FUNCTION: fn_23497
void CSession::LoadLuaDataFiles() {
  if (this->m_hotkeyScript == nullptr) {
    this->m_hotkeyScript = new LuaScript();
    this->m_hotkeyScript->Open();
  }

  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\ChangeDirectoryList", true, false);
  this->m_hotkeyScript->Load("Lua Files\\MsgString_KR", true, false);
  this->m_hotkeyScript->Load("Lua Files\\Common", true, false);
  this->m_hotkeyScript->Load("Lua Files\\wndInfo", true, false);
  this->m_hotkeyScript->Load("Lua Files\\HotKey", true, false);

  WIN32_FIND_DATAA findData;
  HANDLE find = FindFirstFileA("data", &findData);
  if (find == INVALID_HANDLE_VALUE) {
    CreateDirectoryA("data", nullptr);
  } else {
    FindClose(find);
  }

  this->m_hotkeyScript->Load("SaveData\\UserKeys", false, true);
  ResetHotkeySlots();
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\NPCIdentity", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\jobName_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\jobName", true, false);
  {
    const std::array<char, 27> source = {'\x39', '\xB8', '\x8C', '\x7D', '\x2D', '\x0C', '\x89', '\x50', '\x06', '\x91', '\xA9', '\x3C', '\x1F', '\x04', '\xAC', '\x5B', '\x13', '\xA2', '\xB1', '\x0D', '\x0E', '\x11', '\xAC', '\x5B', '\x13', '\xA2', '\xED'};
    thread_local EncryptedName<sizeof(source), 0x35E5656B5DEDCD75ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\AccessoryId", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\accName_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\accName", true, false);

  std::string chatWndInfo("ChatWndInfo_U");
  chatWndInfo = "SaveData\\ChatWndInfo_U";
  if (!this->m_hotkeyScript->Load(chatWndInfo.c_str(), false, true) || !this->m_hotkeyScript->HasTable("ChatSubWnd_1")) {
    this->m_hotkeyScript->Load("Lua Files\\ChatWndInfo\\ChatWndInfo", true, false);
  }

  this->m_hotkeyScript->Load("Lua Files\\ChatWndInfo\\ChatWndInfo_F", true, false);
  this->m_canDragWnd = this->m_hotkeyScript->GlobalInteger("CanDragWnd") == 1;
  this->m_hotkeyScript->Load("Lua Files\\SkillInfoz\\jobinheritlist", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillInfoz\\SkillID", true, false);
  {
    const std::array<char, 35> source = {'\xB1', '\xCC', '\xDA', '\x01', '\xD9', '\x94', '\x2D', '\x2C', '\x8E', '\xE5', '\xE8', '\x4A', '\xF6', '\x91', '\x2D', '\x00', '\x93', '\xDF', '\xD4', '\x5B', '\xC3', '\xAE', '\x2A', '\x20', '\x91', '\xD5', '\xF2', '\x4F', '\xF9', '\x92', '\x0D', '\x20', '\x8E', '\xCD', '\xBB'};
    thread_local EncryptedName<sizeof(source), 0x4941FD9F21BBB9FDULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  {
    const std::array<char, 35> source = {'\x29', '\x6C', '\x24', '\x29', '\x6B', '\x4A', '\x77', '\xEC', '\x16', '\x45', '\x16', '\x62', '\x44', '\x4F', '\x77', '\xC0', '\x0B', '\x7F', '\x2A', '\x73', '\x71', '\x70', '\x70', '\xE0', '\x09', '\x75', '\x01', '\x6C', '\x5E', '\x40', '\x69', '\xE0', '\x15', '\x6D', '\x45'};
    thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\SkillInfoz\\SkillDelayList", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillInfoz\\SkillInfo_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillInfoz\\skilltreeview", true, false);
  this->m_hotkeyScript->Load("Lua Files\\StateIcon\\EFSTIDs", true, false);
  {
    const std::array<char, 34> source = {'\xF5', '\x70', '\x16', '\x6F', '\x4D', '\x74', '\xE3', '\x10', '\xCA', '\x59', '\x24', '\x3B', '\x6A', '\x69', '\xEA', '\x3C', '\xDA', '\x6A', '\x19', '\x13', '\x58', '\x69', '\xEE', '\x01', '\xDC', '\x4C', '\x14', '\x20', '\x65', '\x54', '\xE1', '\x13', '\xD6', '\x05'};
    thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\StateIcon\\StateIconInfo_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\StateIcon\\StateIconImgInfo", true, false);
  this->m_hotkeyScript->Load("System\\LuaFiles514\\OptionInfo", false, true);
  this->m_hotkeyScript->Load("Lua Files\\OptionInfo\\CmdInfo", true, false);
  this->m_hotkeyScript->Load("Lua Files\\OptionInfo\\OptionInfo_F", true, false);
  this->m_hotkeyScript->Load("SaveData\\OptionInfo", false, true);
  this->ApplySaveDataOptions();
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\SpriteRobeId", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\SpriteRobeName_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\SpriteRobeName", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\2DLayerDir_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\BigLayerDir_male", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\BigLayerDir_female", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\SmallLayerDir_male", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\SmallLayerDir_female", true, false);
  if (this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\_New_2DLayerDir_F", true, false)) {
    this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\_New_BigLayerDir_male", true, false);
    this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\_New_BigLayerDir_female", true, false);
    this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\_New_SmallLayerDir_male", true, false);
    this->m_hotkeyScript->Load("Lua Files\\SprEditInfo\\_New_SmallLayerDir_female", true, false);
    this->m_newRobeDrawOnTop = true;
  } else {
    MessageBoxA(nullptr, "NO!! 2D_LAYER_NEW_TABLE", "알림", MB_OK);
  }

  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\PCJobName", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\PCJobNameGender_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\PCJobNameGender", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\NpcLocationRadius", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\NpcLocationRadius_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\EffectTool\\effectmsgid", true, false);
  this->m_hotkeyScript->Load("Lua Files\\EffectTool\\effectfunc", true, false);
  this->m_hotkeyScript->Load("Lua Files\\EffectTool\\EffectToolUtil_ver_2", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillEffectInfo\\EffectID", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillEffectInfo\\ActorState", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillEffectInfo\\SkillEffectInfoList", true, false);
  this->m_hotkeyScript->Load("Lua Files\\SkillEffectInfo\\SkillEffectInfo_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\KafraMoveMapServiceList", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\KafraMoveMapServiceList_F", true, false);
  this->LoadNavigationScripts();
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\HelpMsgStr", true, false);
  {
    const std::array<char, 36> source = {'\x1D', '\xFE', '\xAC', '\x21', '\xFD', '\x54', '\x2D', '\xB4', '\x22', '\xD7', '\x88', '\x6F', '\xCF', '\x4F', '\x38', '\x80', '\x24', '\xEE', '\xB8', '\x64', '\xE7', '\x78', '\x2F', '\xA5', '\x23', '\xF2', '\x9C', '\x74', '\xDE', '\x48', '\x24', '\x9D', '\x38', '\xF8', '\xB9', '\x01'};
    thread_local EncryptedName<sizeof(source), 0xD1413DBB01CD8B51ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\WeaponTable", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\WeaponTable_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\JobIdentity", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\ShadowTable", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\ShadowTable_F", true, false);
  g_worldViewMgr.Load(*this->m_hotkeyScript);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\EnumVAR", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\AddRandomOptionNameTable", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\AddRandomOption_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DressRoom\\Dress_f", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DressRoom\\jobDressList", true, false);
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\TitleTable", true, false);
  if (!this->m_hotkeyScript->Load("Lua Files\\HatEffectInfo\\HatEffectIDs", true, false)) {
    MessageBoxA(hWnd, "Fail to load HatEffectInfo\\HatEffectIDs.lua", "Error", MB_OK);
  }
  if (!this->m_hotkeyScript->Load("Lua Files\\HatEffectInfo\\EffectHatItemTable", true, false)) {
    MessageBoxA(hWnd, "Fail to load HatEffectInfo\\EffectHatItemTable.lua", "Error", MB_OK);
  }
  if (!this->m_hotkeyScript->Load("Lua Files\\HatEffectInfo\\HatEffectInfo", true, false)) {
    MessageBoxA(hWnd, "Fail to load HatEffectInfo\\HatEffectInfo.lua", "Error", MB_OK);
  }
  if (!this->m_hotkeyScript->Load("Lua Files\\HatEffectInfo\\FootPrintEffectInfo", true, false)) {
    MessageBoxA(hWnd, "Fail to load HatEffectInfo\\FootPrintEffectInfo.lua", "Error", MB_OK);
  }
  if (!this->m_hotkeyScript->Load("Lua Files\\HatEffectInfo\\HatEffect_F", true, false)) {
    MessageBoxA(hWnd, "Fail to load HatEffectInfo\\HatEffect_F.lua", "Error", MB_OK);
  }

  this->m_hotkeyScript->Load("Lua Files\\SignBoardList_F", true, false);
  {
    const std::array<char, 24> source = {'\x25', '\x6A', '\x16', '\x75', '\xDB', '\xFC', '\x17', '\x06', '\x1A', '\x43', '\x24', '\x3C', '\xFA', '\xFB', '\x39', '\x0C', '\x08', '\x6D', '\x13', '\x19', '\xF4', '\xE6', '\x0F', '\x63'};
    thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\StylingShop\\StylingShopInfo_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\StylingShop\\StylingShopInfo", true, false);
  this->m_hotkeyScript->Load("Lua Files\\EffectTool\\ForceRenderEffect", true, false);
  this->m_hotkeyScript->Load("Lua Files\\OffsetItemPos\\OffsetItemPos_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\OffsetItemPos\\OffsetItemPos", true, false);
  {
    const std::array<char, 37> source = {'\x95', '\x8C', '\x80', '\xC3', '\x2B', '\x7C', '\xC1', '\x4E', '\xAA', '\xA5', '\x85', '\x82', '\x19', '\x74', '\xC4', '\x45', '\xBF', '\x96', '\xBD', '\xAF', '\x0C', '\x65', '\xC4', '\x45', '\xBC', '\xBD', '\x85', '\x96', '\x06', '\x51', '\xE9', '\x4A', '\xB2', '\xBB', '\x8E', '\x9B', '\x6D'};
    thread_local EncryptedName<sizeof(source), 0x2BAD156DE3E1F9D9ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  {
    const std::array<char, 36> source = {'\xC3', '\xC8', '\x8E', '\xA7', '\x3B', '\xAC', '\xC1', '\x10', '\xFC', '\xE1', '\x8B', '\xE6', '\x09', '\xA4', '\xC4', '\x1B', '\xE9', '\xD2', '\xB3', '\xCB', '\x1C', '\xB5', '\xC4', '\x1B', '\xEA', '\xE8', '\x9F', '\xE0', '\x0F', '\xA4', '\xC9', '\x10', '\xCD', '\xD2', '\x97', '\x87'};
    thread_local EncryptedName<sizeof(source), 0x75ADC57D87EFBD8FULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\transparentItem\\transparentItem", true, false);
  this->m_hotkeyScript->Load("Lua Files\\transparentItem\\transparentItem_f", true, false);
  this->LoadRobeAlpha();
  this->LoadExternalSettings();
  this->m_hotkeyScript->Load("Lua Files\\DataInfo\\TB_Layer_Priority", true, false);
  this->m_hotkeyScript->Load("Lua Files\\datainfo\\tb_cashshop_banner", true, false);
  this->m_hotkeyScript->Load("Lua Files\\datainfo\\tb_stoveshop_banner", true, false);
  {
    const std::array<char, 26> source = {'\xA3', '\x04', '\xBE', '\x33', '\x2B', '\x26', '\xAB', '\x88', '\x9C', '\x2D', '\x9E', '\x74', '\x04', '\x3B', '\x9B', '\xAC', '\x88', '\x18', '\xAB', '\x50', '\x02', '\x21', '\xA1', '\x84', '\x88', '\x71'};
    thread_local EncryptedName<sizeof(source), 0xEDC74F6D13DF71EFULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  {
    const std::array<char, 28> source = {'\xD9', '\xFA', '\x80', '\x2D', '\x1B', '\x60', '\x0D', '\x94', '\xE6', '\xD3', '\xA0', '\x6A', '\x34', '\x7D', '\x3D', '\xB0', '\xF2', '\xE6', '\x95', '\x4E', '\x32', '\x67', '\x07', '\x98', '\xF2', '\xD0', '\xA7', '\x0D'};
    thread_local EncryptedName<sizeof(source), 0xF161095D0DE18F95ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  this->m_hotkeyScript->Load("Lua Files\\datainfo\\tb_checkAttendance_banner", true, false);
  {
    const std::array<char, 36> source = {'\xCB', '\x66', '\x0A', '\x5F', '\x7F', '\x60', '\x1D', '\x6E', '\xF4', '\x4F', '\x2F', '\x1E', '\x54', '\x68', '\x16', '\x6E', '\xD4', '\x78', '\x02', '\x11', '\x65', '\x4D', '\x10', '\x66', '\xE6', '\x74', '\x0E', '\x2C', '\x52', '\x60', '\x1F', '\x47', '\xEE', '\x60', '\x1F', '\x7F'};
    thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  {
    const std::array<char, 36> source = {'\xE9', '\xE8', '\x0C', '\xD9', '\x25', '\x4E', '\x1B', '\x66', '\xD6', '\xC1', '\x29', '\x98', '\x0E', '\x46', '\x10', '\x66', '\xF6', '\xF6', '\x04', '\x97', '\x3F', '\x63', '\x16', '\x6E', '\xC4', '\xFA', '\x08', '\xAA', '\x08', '\x4E', '\x19', '\x4A', '\xCB', '\xFB', '\x02', '\xF9'};
    thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  {
    const std::array<char, 38> source = {'\x21', '\xCE', '\xF4', '\x0D', '\xD5', '\x92', '\x7B', '\xA0', '\x1E', '\xE7', '\xD1', '\x4C', '\xFE', '\x9A', '\x70', '\xA0', '\x3E', '\xD0', '\xFC', '\x43', '\xCF', '\xBF', '\x76', '\xA8', '\x0C', '\xDC', '\xF0', '\x7E', '\xF8', '\x92', '\x79', '\x8C', '\x03', '\xDD', '\xFA', '\x72', '\xD5', '\xFB'};
    thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> name(source);
    this->m_hotkeyScript->Load(name.Text(), true, false);
  }
  g_cashEmotionMgr->LoadScripts();
  this->m_hotkeyScript->Load("Lua Files\\AdventureGuide\\AvailableJobList_F", true, false);
  this->m_hotkeyScript->Load("Lua Files\\AdventureGuide\\AvailableJobList", true, false);
}
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
  bodyJobNames[401] = []() -> EncryptedName<6, 0xB39FA7917BCFBB8DULL>& {
    constexpr std::array<char, 6> source = CRYPT_NAME("DRUID", 0xB39FA7917BCFBB8DULL);
    thread_local EncryptedName<sizeof(source), 0xB39FA7917BCFBB8DULL> name(source);
    return name;
  }().Text();
  bodyJobNames[402] = []() -> EncryptedName<6, 0x8D1D0FF1AF2B13F7ULL>& {
    constexpr std::array<char, 6> source = CRYPT_NAME("DRUID", 0x8D1D0FF1AF2B13F7ULL);
    thread_local EncryptedName<sizeof(source), 0x8D1D0FF1AF2B13F7ULL> name(source);
    return name;
  }().Text();
  bodyJobNames[403] = []() -> EncryptedName<7, 0x4F637DC51307B759ULL>& {
    constexpr std::array<char, 7> source = CRYPT_NAME("KARNOS", 0x4F637DC51307B759ULL);
    thread_local EncryptedName<sizeof(source), 0x4F637DC51307B759ULL> name(source);
    return name;
  }().Text();
  bodyJobNames[404] = []() -> EncryptedName<7, 0x2DCB7567394DC1ABULL>& {
    constexpr std::array<char, 7> source = CRYPT_NAME("KARNOS", 0x2DCB7567394DC1ABULL);
    thread_local EncryptedName<sizeof(source), 0x2DCB7567394DC1ABULL> name(source);
    return name;
  }().Text();
  bodyJobNames[405] = []() -> EncryptedName<7, 0x6BA1C1132F033B75ULL>& {
    constexpr std::array<char, 7> source = CRYPT_NAME("ALITEA", 0x6BA1C1132F033B75ULL);
    thread_local EncryptedName<sizeof(source), 0x6BA1C1132F033B75ULL> name(source);
    return name;
  }().Text();
  bodyJobNames[406] = []() -> EncryptedName<9, 0xA12DAB392545D72FULL>& {
    constexpr std::array<char, 9> source = CRYPT_NAME("WEREWOLF", 0xA12DAB392545D72FULL);
    thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
    return name;
  }().Text();
  bodyJobNames[407] = []() -> EncryptedName<11, 0x33133B977BCD03CDULL>& {
    constexpr std::array<char, 11> source = CRYPT_NAME("WERERAPTOR", 0x33133B977BCD03CDULL);
    thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
    return name;
  }().Text();
  bodyJobNames[408] = []() -> EncryptedName<13, 0x2FEB8739E5975BD1ULL>& {
    constexpr std::array<char, 13> source = CRYPT_NAME("DRUID_RIDING", 0x2FEB8739E5975BD1ULL);
    thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
    return name;
  }().Text();
  bodyJobNames[409] = []() -> EncryptedName<14, 0x39A39F87DD5F535BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("KARNOS_RIDING", 0x39A39F87DD5F535BULL);
    thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
    return name;
  }().Text();
  bodyJobNames[410] = []() -> EncryptedName<14, 0x6FE153231D879777ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("ALITEA_RIDING", 0x6FE153231D879777ULL);
    thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
    return name;
  }().Text();
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
  shieldJobNames.resize(28053, []() -> EncryptedName<14, 0x234977DDB54BC71DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x234977DDB54BC71DULL);
    thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
    return name;
  }().Text());
  shieldJobNames[0] = []() -> EncryptedName<14, 0xEF6D1D893F2B2DC5ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xEF6D1D893F2B2DC5ULL);
    thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[1] = []() -> EncryptedName<10, 0xE55723712993051DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xE55723712993051DULL);
    thread_local EncryptedName<sizeof(source), 0xE55723712993051DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[2] = []() -> EncryptedName<14, 0x793F370985ED674FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x793F370985ED674FULL);
    thread_local EncryptedName<sizeof(source), 0x793F370985ED674FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[3] = []() -> EncryptedName<10, 0xCB1D253B7FE1F563ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0xCB1D253B7FE1F563ULL);
    thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[4] = []() -> EncryptedName<14, 0x7115D9F973F913C9ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x7115D9F973F913C9ULL);
    thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[5] = []() -> EncryptedName<10, 0x7321C3814F8799E1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0x7321C3814F8799E1ULL);
    thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[6] = []() -> EncryptedName<10, 0x7941D76335CFA14FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0x7941D76335CFA14FULL);
    thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[7] = []() -> EncryptedName<10, 0x0F15B16B5BAFD57FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("기사\\기사", 0x0F15B16B5BAFD57FULL);
    thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[8] = []() -> EncryptedName<18, 0x7DE349D717D7F123ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x7DE349D717D7F123ULL);
    thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[9] = []() -> EncryptedName<14, 0x9FED7DA189490D5DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x9FED7DA189490D5DULL);
    thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[10] = []() -> EncryptedName<14, 0x452FEF0D7FC95565ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0x452FEF0D7FC95565ULL);
    thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[11] = []() -> EncryptedName<10, 0xF3CB6F7711A71B6DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xF3CB6F7711A71B6DULL);
    thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[12] = []() -> EncryptedName<14, 0x839B27DF8F5967B3ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x839B27DF8F5967B3ULL);
    thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[13] = []() -> EncryptedName<28, 0x0F41EBBDA32D1B17ULL>& {
    constexpr std::array<char, 28> source = CRYPT_NAME("페코페코_기사\\페코페코_기사", 0x0F41EBBDA32D1B17ULL);
    thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[14] = []() -> EncryptedName<22, 0x2D51E3B7236175EDULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("크루세이더\\크루세이더", 0x2D51E3B7236175EDULL);
    thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> name(source);
    return name;
  }().Text();
  shieldJobNames[15] = []() -> EncryptedName<10, 0x176F138D5DD7F365ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x176F138D5DD7F365ULL);
    thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[16] = []() -> EncryptedName<14, 0xFBD599E117C547C1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0xFBD599E117C547C1ULL);
    thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[17] = []() -> EncryptedName<10, 0x192D654D29337BE5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x192D654D29337BE5ULL);
    thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[18] = []() -> EncryptedName<18, 0xB3E307A5F753D5E3ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0xB3E307A5F753D5E3ULL);
    thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[19] = []() -> EncryptedName<10, 0x79E915E9AF6F55DDULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x79E915E9AF6F55DDULL);
    thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
    return name;
  }().Text();
  shieldJobNames[20] = []() -> EncryptedName<10, 0xB7AFBD2FD9FD7501ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0xB7AFBD2FD9FD7501ULL);
    thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[21] = []() -> EncryptedName<34, 0x6BDF334F2BFDAD51ULL>& {
    constexpr std::array<char, 34> source = CRYPT_NAME("신페코크루세이더\\신페코크루세이더", 0x6BDF334F2BFDAD51ULL);
    thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[23] = []() -> EncryptedName<14, 0x674D9F3975573D41ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x674D9F3975573D41ULL);
    thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[22] = []() -> EncryptedName<10, 0x0B23D10F2F49D72BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x0B23D10F2F49D72BULL);
    thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[26] = []() -> EncryptedName<10, 0x136F2F9915D1DBCFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x136F2F9915D1DBCFULL);
    thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
    return name;
  }().Text();
  shieldJobNames[27] = []() -> EncryptedName<10, 0x114901C7E37F6D49ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x114901C7E37F6D49ULL);
    thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[28] = []() -> EncryptedName<10, 0x7D5D0F110B6B8371ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x7D5D0F110B6B8371ULL);
    thread_local EncryptedName<sizeof(source), 0x7D5D0F110B6B8371ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[29] = []() -> EncryptedName<10, 0xADC51BF9C98B413DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0xADC51BF9C98B413DULL);
    thread_local EncryptedName<sizeof(source), 0xADC51BF9C98B413DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[30] = []() -> EncryptedName<10, 0xDF8117FF49CFF551ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0xDF8117FF49CFF551ULL);
    thread_local EncryptedName<sizeof(source), 0xDF8117FF49CFF551ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[24] = []() -> EncryptedName<10, 0x87E3D1C50D572DEFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0x87E3D1C50D572DEFULL);
    thread_local EncryptedName<sizeof(source), 0x87E3D1C50D572DEFULL> name(source);
    return name;
  }().Text();
  shieldJobNames[25] = []() -> EncryptedName<10, 0x51BB39B763BBBD8DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("닌자\\닌자", 0x51BB39B763BBBD8DULL);
    thread_local EncryptedName<sizeof(source), 0x51BB39B763BBBD8DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[164] = []() -> EncryptedName<10, 0xDB27CB557D038759ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("닌자\\닌자", 0xDB27CB557D038759ULL);
    thread_local EncryptedName<sizeof(source), 0xDB27CB557D038759ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[167] = []() -> EncryptedName<14, 0x3FA3AFA7035DE3FFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x3FA3AFA7035DE3FFULL);
    thread_local EncryptedName<sizeof(source), 0x3FA3AFA7035DE3FFULL> name(source);
    return name;
  }().Text();
  shieldJobNames[165] = []() -> EncryptedName<10, 0x0399459DAF454F0FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0x0399459DAF454F0FULL);
    thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[166] = []() -> EncryptedName<10, 0xD5A78DB38D8DAFABULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xD5A78DB38D8DAFABULL);
    thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
    return name;
  }().Text();
  shieldJobNames[168] = []() -> EncryptedName<14, 0xA597A7E7492B39D5ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0xA597A7E7492B39D5ULL);
    thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[169] = []() -> EncryptedName<10, 0xFFABE3D5D1BBEF73ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0xFFABE3D5D1BBEF73ULL);
    thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[170] = []() -> EncryptedName<18, 0xC70319B5EB61598FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0xC70319B5EB61598FULL);
    thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[171] = []() -> EncryptedName<18, 0x0B8745519B1F95D7ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x0B8745519B1F95D7ULL);
    thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[172] = []() -> EncryptedName<10, 0x8783C33941E15117ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x8783C33941E15117ULL);
    thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[173] = []() -> EncryptedName<18, 0x271B45771F83DD9FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("태권소년\\태권소년", 0x271B45771F83DD9FULL);
    thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[174] = []() -> EncryptedName<14, 0xC1BB8111EDD183E1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xC1BB8111EDD183E1ULL);
    thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[175] = []() -> EncryptedName<10, 0x1767DDE18D779D8BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x1767DDE18D779D8BULL);
    thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[176] = []() -> EncryptedName<14, 0x01CB4F1DF9D3F931ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x01CB4F1DF9D3F931ULL);
    thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[177] = []() -> EncryptedName<10, 0xC5B1BBCD9BC9C997ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0xC5B1BBCD9BC9C997ULL);
    thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[178] = []() -> EncryptedName<14, 0x792FFD1325418FB5ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x792FFD1325418FB5ULL);
    thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[179] = []() -> EncryptedName<18, 0x49D33529F1556333ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x49D33529F1556333ULL);
    thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[180] = []() -> EncryptedName<14, 0xE33F2FF779878559ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0xE33F2FF779878559ULL);
    thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[181] = []() -> EncryptedName<14, 0xF15F8785CDE369BFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0xF15F8785CDE369BFULL);
    thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
    return name;
  }().Text();
  shieldJobNames[182] = []() -> EncryptedName<14, 0x81F58DED5D470D01ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x81F58DED5D470D01ULL);
    thread_local EncryptedName<sizeof(source), 0x81F58DED5D470D01ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[183] = []() -> EncryptedName<14, 0x454D31872BB3EDCBULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x454D31872BB3EDCBULL);
    thread_local EncryptedName<sizeof(source), 0x454D31872BB3EDCBULL> name(source);
    return name;
  }().Text();
  shieldJobNames[184] = []() -> EncryptedName<14, 0x41CFCB831F6B6B91ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x41CFCB831F6B6B91ULL);
    thread_local EncryptedName<sizeof(source), 0x41CFCB831F6B6B91ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[185] = []() -> EncryptedName<14, 0x5DE7712FAB613F75ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x5DE7712FAB613F75ULL);
    thread_local EncryptedName<sizeof(source), 0x5DE7712FAB613F75ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[186] = []() -> EncryptedName<14, 0x0F97E779A933D917ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x0F97E779A933D917ULL);
    thread_local EncryptedName<sizeof(source), 0x0F97E779A933D917ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[187] = []() -> EncryptedName<18, 0x3789496BE3E1D141ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x3789496BE3E1D141ULL);
    thread_local EncryptedName<sizeof(source), 0x3789496BE3E1D141ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[188] = []() -> EncryptedName<14, 0x4331A17F33095FEBULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0x4331A17F33095FEBULL);
    thread_local EncryptedName<sizeof(source), 0x4331A17F33095FEBULL> name(source);
    return name;
  }().Text();
  shieldJobNames[189] = []() -> EncryptedName<10, 0xD1056FF94D4D2773ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0xD1056FF94D4D2773ULL);
    thread_local EncryptedName<sizeof(source), 0xD1056FF94D4D2773ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[190] = []() -> EncryptedName<14, 0xB127A19D775BF751ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xB127A19D775BF751ULL);
    thread_local EncryptedName<sizeof(source), 0xB127A19D775BF751ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[191] = []() -> EncryptedName<10, 0x0F61499B4973EB5DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0x0F61499B4973EB5DULL);
    thread_local EncryptedName<sizeof(source), 0x0F61499B4973EB5DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[192] = []() -> EncryptedName<10, 0xB7A96F8701BB4B6BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xB7A96F8701BB4B6BULL);
    thread_local EncryptedName<sizeof(source), 0xB7A96F8701BB4B6BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[193] = []() -> EncryptedName<10, 0xA31F69E7EFED01C9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xA31F69E7EFED01C9ULL);
    thread_local EncryptedName<sizeof(source), 0xA31F69E7EFED01C9ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[194] = []() -> EncryptedName<10, 0xBFF5C517A1A77F59ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xBFF5C517A1A77F59ULL);
    thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[195] = []() -> EncryptedName<14, 0x659975C121557D3DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x659975C121557D3DULL);
    thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[196] = []() -> EncryptedName<14, 0x85E1F5A7A5755B7FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x85E1F5A7A5755B7FULL);
    thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
    return name;
  }().Text();
  shieldJobNames[197] = []() -> EncryptedName<10, 0xAD6153019F93A527ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0xAD6153019F93A527ULL);
    thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[198] = []() -> EncryptedName<10, 0xADADEB4DF7813B55ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0xADADEB4DF7813B55ULL);
    thread_local EncryptedName<sizeof(source), 0xADADEB4DF7813B55ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[199] = []() -> EncryptedName<10, 0xD9452D81E5CD99C1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0xD9452D81E5CD99C1ULL);
    thread_local EncryptedName<sizeof(source), 0xD9452D81E5CD99C1ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[200] = []() -> EncryptedName<10, 0xD7A749EB27D5BF6DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xD7A749EB27D5BF6DULL);
    thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[201] = []() -> EncryptedName<10, 0xB195637DAFCB0D59ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0xB195637DAFCB0D59ULL);
    thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[202] = []() -> EncryptedName<10, 0xB19D1FA9116F39D3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0xB19D1FA9116F39D3ULL);
    thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[203] = []() -> EncryptedName<10, 0x9569C9C7E70909AFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x9569C9C7E70909AFULL);
    thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> name(source);
    return name;
  }().Text();
  shieldJobNames[204] = []() -> EncryptedName<10, 0xF715514F5961B75BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xF715514F5961B75BULL);
    thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[205] = []() -> EncryptedName<18, 0x4191BB5531FD3999ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("태권소년\\태권소년", 0x4191BB5531FD3999ULL);
    thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[206] = []() -> EncryptedName<18, 0xBD2BA1C11B01E73BULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0xBD2BA1C11B01E73BULL);
    thread_local EncryptedName<sizeof(source), 0xBD2BA1C11B01E73BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[207] = []() -> EncryptedName<18, 0x4B755DB19953E311ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x4B755DB19953E311ULL);
    thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[208] = []() -> EncryptedName<14, 0xC9712D676B5B3943ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xC9712D676B5B3943ULL);
    thread_local EncryptedName<sizeof(source), 0xC9712D676B5B3943ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[209] = []() -> EncryptedName<10, 0xED15938D333D2DB9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xED15938D333D2DB9ULL);
    thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[210] = []() -> EncryptedName<14, 0xD98F5345098D630DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0xD98F5345098D630DULL);
    thread_local EncryptedName<sizeof(source), 0xD98F5345098D630DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[211] = []() -> EncryptedName<10, 0x9B0F234DEFDF7593ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x9B0F234DEFDF7593ULL);
    thread_local EncryptedName<sizeof(source), 0x9B0F234DEFDF7593ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[212] = []() -> EncryptedName<14, 0xFD69A9B3F311818DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0xFD69A9B3F311818DULL);
    thread_local EncryptedName<sizeof(source), 0xFD69A9B3F311818DULL> name(source);
    return name;
  }().Text();
  shieldJobNames[213] = []() -> EncryptedName<10, 0x7F2DFFC1ABE91F2BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0x7F2DFFC1ABE91F2BULL);
    thread_local EncryptedName<sizeof(source), 0x7F2DFFC1ABE91F2BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[214] = []() -> EncryptedName<10, 0x1F1D51274373C1FDULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0x1F1D51274373C1FDULL);
    thread_local EncryptedName<sizeof(source), 0x1F1D51274373C1FDULL> name(source);
    return name;
  }().Text();
  shieldJobNames[215] = []() -> EncryptedName<14, 0x5DAD2D25D9259B93ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x5DAD2D25D9259B93ULL);
    thread_local EncryptedName<sizeof(source), 0x5DAD2D25D9259B93ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[216] = []() -> EncryptedName<10, 0x6BF729E30729CF29ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x6BF729E30729CF29ULL);
    thread_local EncryptedName<sizeof(source), 0x6BF729E30729CF29ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[217] = []() -> EncryptedName<14, 0x3D371F5B0F7F9327ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x3D371F5B0F7F9327ULL);
    thread_local EncryptedName<sizeof(source), 0x3D371F5B0F7F9327ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[218] = []() -> EncryptedName<10, 0x610F7DD1EFC1C91BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x610F7DD1EFC1C91BULL);
    thread_local EncryptedName<sizeof(source), 0x610F7DD1EFC1C91BULL> name(source);
    return name;
  }().Text();
  shieldJobNames[219] = []() -> EncryptedName<18, 0xC5995F4B9FE96B89ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0xC5995F4B9FE96B89ULL);
    thread_local EncryptedName<sizeof(source), 0xC5995F4B9FE96B89ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[220] = []() -> EncryptedName<10, 0x097389157345A787ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x097389157345A787ULL);
    thread_local EncryptedName<sizeof(source), 0x097389157345A787ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[221] = []() -> EncryptedName<10, 0x5F93A1C3A7AD7537ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x5F93A1C3A7AD7537ULL);
    thread_local EncryptedName<sizeof(source), 0x5F93A1C3A7AD7537ULL> name(source);
    return name;
  }().Text();
  shieldJobNames[222] = []() -> EncryptedName<14, 0x875FCDF1E7A1795FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x875FCDF1E7A1795FULL);
    thread_local EncryptedName<sizeof(source), 0x875FCDF1E7A1795FULL> name(source);
    return name;
  }().Text();
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
  shieldJobNames[77] = []() -> EncryptedName<14, 0x5FFD7FABB5D1EF57ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x5FFD7FABB5D1EF57ULL);
    thread_local EncryptedName<sizeof(source), 0x5FFD7FABB5D1EF57ULL> name(source);
    return name;
  }().Text();
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
  std::vector<const char*>& femaleSuffixNames = reinterpret_cast<std::vector<const char*>&>(this->additionalSpriteNames[0x08]);
  femaleSuffixNames.resize(2, "f");
  femaleSuffixNames[0] = "f";
  femaleSuffixNames[1] = "";
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
  if (g_serviceType == 0) {
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
  } else {
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
  }
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

// OFFSET: 0x00DE7260
// FUNCTION: fn_23496
void CSession::ReloadLanguageTables() {
  ImportMessageTable();
  g_forbiddenWords.Load("manner.txt");
  this->LoadXmlTables();
  this->LoadMapNameTable();
  LoadItemNameTables();
  Sub7739F0();
}

// OFFSET: 0x00DF33B0
// FUNCTION: fn_23506
void CSession::LoadSessionTables() {
  CSession* session = this;
  ++g_sessionTableLoadCount;
  session->InitJobHitWaveNames();

  CCommonObject& storage = reinterpret_cast<CCommonObject&>(session->minimapTokenStorage[0x30]);
  {
    std::vector<char*> tokens;
    session->m_bgmFileNames.clear();
    session->m_bgmFileNames.reserve(512);
    storage.LoadTokens(tokens, "mp3NameTable.txt", 0);
    for (auto token = tokens.begin(); token != tokens.end(); token += 2) {
      session->m_bgmFileNames.push_back(std::make_pair(token[0], token[1]));
    }
    std::sort(session->m_bgmFileNames.begin(), session->m_bgmFileNames.end(),
              [](const std::pair<const char*, const char*>& left, const std::pair<const char*, const char*>& right) {
                return std::strcmp(left.first, right.first) < 0;
              });
  }

  storage.LoadTokens(session->m_indoorRswNames, "indoorRswTable.txt", 0);
  std::sort(session->m_indoorRswNames.begin(), session->m_indoorRswNames.end(),
            [](const char* left, const char* right) { return std::strcmp(left, right) < 0; });

  session->LoadMapViewParameters();
  session->InitSkillEffectIds();
  ImportMessageTable();
  g_forbiddenWords.Load("manner.txt");
  session->LoadXmlTables();
  session->LoadMapNameTable();
  LoadItemNameTables();
  Sub7739F0();
  g_simpleStringMgr->Init();

  constexpr std::array<char, 15> source = CRYPT_NAME("Ragnarok Setup", 0x9171997D6963C9F7ULL);
  thread_local EncryptedName<sizeof(source), 0x9171997D6963C9F7ULL> setupName(source);
  if (HWND window = FindWindowA(nullptr, setupName.Text())) {
    ShowErrorDialog(LookupMessage(MSI_TAREN_LOGINREFUSE_FAIL_PASSWORD), 0);
    SendMessageA(window, WM_CLOSE, 0, 0);
  }

  session->LoadFogParameters();
  session->LoadLuaDataFiles();
  session->InitWeaponHitWaveNames();
  session->InitResourceNameTables();
  session->InitJobSpriteNames();
  session->InitJobNameTable();
  session->InitNpcLocationRadius();
  session->InitCommandTables();
  session->ResetCharacterState();
  session->ClearItemContainers();
  session->m_inventoryItems.erase(session->m_inventoryItems.begin(), session->m_inventoryItems.end());
  session->m_pingSamples.erase(session->m_pingSamples.begin(), session->m_pingSamples.end());
  for (int index = 0; index < 10; ++index) {
    session->equippedItems[index].Reset();
  }

  {
    std::vector<char*> buyingStoreItems;
    session->inventoryFilterIds.clear();
    session->inventoryFilterIds.reserve(512);
    storage.LoadTokens(buyingStoreItems, "BuyingStoreItemList.txt", 0);
    for (char* token : buyingStoreItems) {
      session->inventoryFilterIds.push_back(std::atoi(token));
    }
  }

  session->mapPositionsScaled = 0;
  session->m_mapPosListCount = -1;
  session->LoadMapPosTable();
  session->LoadExceptionMinimapNameTable();
  session->InitPetActionTable();
}

template <>
EncryptedName<14, 0xB5F3F779237315F3ULL>::EncryptedName(const char (&source)[14]);
template <>
const char* EncryptedName<14, 0x6BC977B3B5993503ULL>::Text();
EncryptedName<14, 0x6BC977B3B5993503ULL>& PoringSuperNovice2SpriteName();
EncryptedName<14, 0x91493751EF33B5FDULL>& PoringSuperNovice2BabySpriteName();
template <>
const char* EncryptedName<18, 0x0DF5B33D9F21EB79ULL>::Text();
EncryptedName<18, 0x0DF5B33D9F21EB79ULL>& SheepPriestBabySpriteName();
template <>
const char* EncryptedName<14, 0x01E7898D9D235DDFULL>::Text();
EncryptedName<14, 0x01E7898D9D235DDFULL>& FoxWizardBabySpriteName();
template <>
const char* EncryptedName<14, 0x6961FF056999A303ULL>::Text();
EncryptedName<14, 0x6961FF056999A303ULL>& PigBlacksmithBabySpriteName();
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& PigMechanicSpriteName();
template <>
const char* EncryptedName<14, 0xC7FB6DF18577D34BULL>::Text();
EncryptedName<14, 0xC7FB6DF18577D34BULL>& PigMechanicBabySpriteName();
template <>
const char* EncryptedName<14, 0xBD3F430159B551FDULL>::Text();
EncryptedName<14, 0xBD3F430159B551FDULL>& OstrichRangerSpriteName();
template <>
const char* EncryptedName<14, 0x2B477F9B5B39B301ULL>::Text();
EncryptedName<14, 0x2B477F9B5B39B301ULL>& OstrichRangerBabySpriteName();
EncryptedName<10, 0x5B832361BDA77197ULL>& LionKnightSpriteName();
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& LionKnightBabySpriteName();
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& LionKnightHighSpriteName();
template <>
const char* EncryptedName<18, 0xE51B29334D353B81ULL>::Text();
EncryptedName<18, 0xE51B29334D353B81ULL>& LionRoyalGuardSpriteName();
EncryptedName<18, 0x8F3F2FA959977F9FULL>& LionRoyalGuardBabySpriteName();
template <>
const char* EncryptedName<18, 0x7FE563BD3FB7F925ULL>::Text();
EncryptedName<18, 0x7FE563BD3FB7F925ULL>& LionRuneKnightSpriteName();
template <>
const char* EncryptedName<18, 0x8B3FA35B5BDDDF89ULL>::Text();
EncryptedName<18, 0x8B3FA35B5BDDDF89ULL>& LionRuneKnightBabySpriteName();
template <>
const char* EncryptedName<22, 0x7DB1050779034DE7ULL>::Text();
EncryptedName<22, 0x7DB1050779034DE7ULL>& LionCrusaderSpriteName();
template <>
const char* EncryptedName<22, 0x5D6B3B1DEB2397B1ULL>::Text();
EncryptedName<22, 0x5D6B3B1DEB2397B1ULL>& LionCrusaderBabySpriteName();
template <>
const char* EncryptedName<22, 0x07112F85A39F4FABULL>::Text();
EncryptedName<22, 0x07112F85A39F4FABULL>& LionCrusaderHighSpriteName();
template <>
const char* EncryptedName<16, 0x45E3F103CBB5F157ULL>::Text();
EncryptedName<16, 0x45E3F103CBB5F157ULL>& KagerouSpriteName();
template <>
const char* EncryptedName<12, 0x936F3D2FFDFD87F5ULL>::Text();
EncryptedName<12, 0x936F3D2FFDFD87F5ULL>& OboroSpriteName();
template <>
const char* EncryptedName<16, 0x2D33791F67AD47DFULL>::Text();
EncryptedName<16, 0x2D33791F67AD47DFULL>& FrogKagerouSpriteName();
template <>
const char* EncryptedName<12, 0x6B3301E1A3079125ULL>::Text();
EncryptedName<12, 0x6B3301E1A3079125ULL>& FrogOboroSpriteName();
template <>
const char* EncryptedName<20, 0x2F2701A7E56D3D89ULL>::Text();
EncryptedName<20, 0x2F2701A7E56D3D89ULL>& RebellionSpriteName();
template <>
const char* EncryptedName<18, 0xC39FB559A17D3963ULL>::Text();
EncryptedName<18, 0xC39FB559A17D3963ULL>& DoramSummonerSpriteName();
template <>
const char* EncryptedName<23, 0x71B7691737C5A1AFULL>::Text();
EncryptedName<23, 0x71B7691737C5A1AFULL>& DoramCartSummonerSpriteName();
template <>
const char* EncryptedName<18, 0x899D11BB83695B0DULL>::Text();
EncryptedName<18, 0x899D11BB83695B0DULL>& DoramSummonerBabySpriteName();
template <>
const char* EncryptedName<23, 0x2B8725D7E95325F5ULL>::Text();
EncryptedName<23, 0x2B8725D7E95325F5ULL>& DoramCartSummonerBabySpriteName();
EncryptedName<10, 0x7989830DE73B9DBDULL>& NinjaBabySpriteName();
template <>
const char* EncryptedName<10, 0xBDADDD87C5A50B6BULL>::Text();
EncryptedName<10, 0xBDADDD87C5A50B6BULL>& FrogNinjaBabySpriteName();
template <>
const char* EncryptedName<16, 0xA7471985F93F9BDFULL>::Text();
EncryptedName<16, 0xA7471985F93F9BDFULL>& KagerouBabySpriteName();
template <>
const char* EncryptedName<12, 0x5FFD7FABB5D1EF57ULL>::Text();
EncryptedName<12, 0x5FFD7FABB5D1EF57ULL>& OboroBabySpriteName();
template <>
const char* EncryptedName<16, 0xD3E5516F89F161D1ULL>::Text();
EncryptedName<16, 0xD3E5516F89F161D1ULL>& FrogKagerouBabySpriteName();
template <>
const char* EncryptedName<12, 0xF535E561137F138DULL>::Text();
EncryptedName<12, 0xF535E561137F138DULL>& FrogOboroBabySpriteName();
template <>
const char* EncryptedName<18, 0xABED6971354375C3ULL>::Text();
EncryptedName<18, 0xABED6971354375C3ULL>& TaekwonBabySpriteName();
template <>
const char* EncryptedName<18, 0x01C9FBA7819FFBD7ULL>::Text();
EncryptedName<18, 0x01C9FBA7819FFBD7ULL>& PoringTaekwonBabySpriteName();
template <>
const char* EncryptedName<10, 0xFBAD074BDB2B77A1ULL>::Text();
EncryptedName<10, 0xFBAD074BDB2B77A1ULL>& StarGladiatorBabySpriteName();
template <>
const char* EncryptedName<10, 0x6BA557130BDFE997ULL>::Text();
EncryptedName<10, 0x6BA557130BDFE997ULL>& StarGladiator2BabySpriteName();
template <>
const char* EncryptedName<10, 0x050DF16FC75B25B9ULL>::Text();
EncryptedName<10, 0x050DF16FC75B25B9ULL>& PoringStarGladiatorBabySpriteName();
template <>
const char* EncryptedName<18, 0x6D05D5271B5D65C3ULL>::Text();
EncryptedName<18, 0x6D05D5271B5D65C3ULL>& SoulLinkerBabySpriteName();
template <>
const char* EncryptedName<18, 0x3D8387EF97A37DCFULL>::Text();
EncryptedName<18, 0x3D8387EF97A37DCFULL>& FrogSoulLinkerBabySpriteName();
template <>
const char* EncryptedName<10, 0x0789EBC9F5C1C59FULL>::Text();
template <>
const char* EncryptedName<10, 0x451FD72783630709ULL>::Text();
template <>
const char* EncryptedName<20, 0x3B6D9BFD27A7C7F7ULL>::Text();
template <>
const char* EncryptedName<20, 0xA15B555BE9CD2B81ULL>::Text();
template <>
const char* EncryptedName<10, 0x73C32959FDFFA51BULL>::Text();
template <>
const char* EncryptedName<18, 0xB3F1A10745C1F105ULL>::Text();
template <>
const char* EncryptedName<10, 0xD9115915B1919533ULL>::Text();
template <>
const char* EncryptedName<18, 0x7959FFBF652D07BFULL>::Text();
template <>
const char* EncryptedName<10, 0x99157F159BB99BD9ULL>::Text();
template <>
const char* EncryptedName<10, 0x31E3CF9D358D8705ULL>::Text();
template <>
const char* EncryptedName<10, 0xA55B7BEF3D7377E3ULL>::Text();
template <>
const char* EncryptedName<18, 0xF5496753BDE7C5BDULL>::Text();
template <>
const char* EncryptedName<10, 0x79A58B0F2173CF49ULL>::Text();
template <>
const char* EncryptedName<18, 0x6B3917C939150B9BULL>::Text();
template <>
const char* EncryptedName<18, 0x795787B7779D19DBULL>::Text();
template <>
const char* EncryptedName<23, 0xF9B1EF9F7961EFF5ULL>::Text();
template <>
const char* EncryptedName<25, 0x29AB0B59651BDFD5ULL>::Text();
template <>
const char* EncryptedName<22, 0xD97DC927AFE911F1ULL>::Text();
template <>
const char* EncryptedName<18, 0x47B7BF55EB471557ULL>::Text();
template <>
const char* EncryptedName<25, 0x8BA52B970DD531ADULL>::Text();
template <>
const char* EncryptedName<20, 0x2F2103792383718BULL>::Text();
template <>
const char* EncryptedName<27, 0xCFB9671DEF4BD9E7ULL>::Text();
template <>
const char* EncryptedName<32, 0x4349ADE96FEB27E5ULL>::Text();
template <>
const char* EncryptedName<39, 0x999F5191852D2F6BULL>::Text();
template <>
const char* EncryptedName<18, 0x35F59F6F3DD1C965ULL>::Text();
template <>
const char* EncryptedName<22, 0xBF0F412941076FADULL>::Text();
template <>
const char* EncryptedName<29, 0xD7BBEB2DDB295D11ULL>::Text();
template <>
const char* EncryptedName<12, 0xB59BADC5B365617DULL>::Text();
template <>
const char* EncryptedName<26, 0x8DFF0B4B67D9FDE1ULL>::Text();
template <>
const char* EncryptedName<16, 0x91E7A7DFA58F9B6BULL>::Text();
template <>
const char* EncryptedName<30, 0x77D925073D6F19B9ULL>::Text();
template <>
const char* EncryptedName<35, 0xAFF9332FD397533FULL>::Text();
template <>
const char* EncryptedName<26, 0xA1DFCDAD555BB16DULL>::Text();
template <>
const char* EncryptedName<33, 0x7DC10333F96DE3E7ULL>::Text();
template <>
const char* EncryptedName<26, 0x194F8339AF753FADULL>::Text();
template <>
const char* EncryptedName<33, 0x51372B6F4BA14B23ULL>::Text();
template <>
const char* EncryptedName<28, 0xE9FBAB53D5A1BB17ULL>::Text();
template <>
const char* EncryptedName<42, 0x1F79132B518FD389ULL>::Text();
template <>
const char* EncryptedName<44, 0xB70319D3A78D39CDULL>::Text();
template <>
const char* EncryptedName<30, 0xA5C5D1A3B9457B53ULL>::Text();
template <>
const char* EncryptedName<44, 0x8F61434F0F79DDD7ULL>::Text();
template <>
const char* EncryptedName<46, 0x4B670715D543C1C3ULL>::Text();
template <>
const char* EncryptedName<24, 0x61F59D5D41E76519ULL>::Text();
template <>
const char* EncryptedName<24, 0x4B1D078937DB892BULL>::Text();
template <>
const char* EncryptedName<31, 0x7155E5EF0DFB0D03ULL>::Text();
template <>
const char* EncryptedName<26, 0xD51953E763B94747ULL>::Text();
template <>
const char* EncryptedName<33, 0x17FB0F6BFF3BE93FULL>::Text();
template <>
const char* EncryptedName<18, 0x91DF1F5D3B014D45ULL>::Text();
template <>
const char* EncryptedName<25, 0x2529F16B0375530DULL>::Text();
template <>
const char* EncryptedName<18, 0x2F371DDDADDD53CFULL>::Text();
template <>
const char* EncryptedName<25, 0x0D7FA94DD32361A9ULL>::Text();
template <>
const char* EncryptedName<24, 0x3BBB5F0F8F216F31ULL>::Text();
EncryptedName<24, 0x3BBB5F0F8F216F31ULL>& NightWatchBodyPath2();
template <>
const char* EncryptedName<31, 0x2F3D75DFCF678FF3ULL>::Text();
EncryptedName<31, 0x2F3D75DFCF678FF3ULL>& NightWatchRidingBodyPath2();
template <>
const char* EncryptedName<26, 0xED730315539785C7ULL>::Text();
EncryptedName<26, 0xED730315539785C7ULL>& HyperNoviceBodyPath2();
template <>
const char* EncryptedName<33, 0x6F9D21B96D71C1FFULL>::Text();
EncryptedName<33, 0x6F9D21B96D71C1FFULL>& HyperNoviceRidingBodyPath2();
template <>
const char* EncryptedName<30, 0x39E7F13755252F8FULL>::Text();
EncryptedName<30, 0x39E7F13755252F8FULL>& SpiritHandlerBodyPath2();
template <>
const char* EncryptedName<37, 0xCBD7DDAFDF87F549ULL>::Text();
EncryptedName<37, 0xCBD7DDAFDF87F549ULL>& SpiritHandlerRidingBodyPath2();
template <>
const char* EncryptedName<12, 0xEB074B7FBDA3BFD5ULL>::Text();
EncryptedName<12, 0xEB074B7FBDA3BFD5ULL>& DruidBodyPath3();
template <>
const char* EncryptedName<12, 0x3D075BD5256F9B6FULL>::Text();
EncryptedName<12, 0x3D075BD5256F9B6FULL>& DruidBodyPath4();
template <>
const char* EncryptedName<14, 0x251761D7370BA5C3ULL>::Text();
EncryptedName<14, 0x251761D7370BA5C3ULL>& KarnosBodyPath3();
EncryptedName<14, 0x03014749B127914FULL>& KarnosBodyPath4();
EncryptedName<14, 0x85FB93CF2F494179ULL>& AliteaBodyPath2();

// OFFSET: 0x00E31E30
// FUNCTION: fn_24178
void CSession::InitJobSpriteNames() {
  std::vector<const char*>& jobSpriteNames = reinterpret_cast<std::vector<const char*>&>(this->m_jobSpriteNames);
  jobSpriteNames.resize(28053, []() -> EncryptedName<14, 0x114901C7E37F6D49ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x114901C7E37F6D49ULL);
    thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
    return name;
  }().Text());
  jobSpriteNames[0] = []() -> EncryptedName<14, 0xDF8117FF49CFF551ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xDF8117FF49CFF551ULL);
    thread_local EncryptedName<sizeof(source), 0xDF8117FF49CFF551ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[1] = []() -> EncryptedName<10, 0xADC51BF9C98B413DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xADC51BF9C98B413DULL);
    thread_local EncryptedName<sizeof(source), 0xADC51BF9C98B413DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[2] = []() -> EncryptedName<14, 0x51BB39B763BBBD8DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x51BB39B763BBBD8DULL);
    thread_local EncryptedName<sizeof(source), 0x51BB39B763BBBD8DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[3] = []() -> EncryptedName<10, 0x87E3D1C50D572DEFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x87E3D1C50D572DEFULL);
    thread_local EncryptedName<sizeof(source), 0x87E3D1C50D572DEFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[4] = []() -> EncryptedName<14, 0x3FA3AFA7035DE3FFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x3FA3AFA7035DE3FFULL);
    thread_local EncryptedName<sizeof(source), 0x3FA3AFA7035DE3FFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[5] = []() -> EncryptedName<10, 0xDB27CB557D038759ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0xDB27CB557D038759ULL);
    thread_local EncryptedName<sizeof(source), 0xDB27CB557D038759ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[6] = []() -> EncryptedName<10, 0xB3E307A5F753D5E3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0xB3E307A5F753D5E3ULL);
    thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[7] = []() -> EncryptedName<10, 0x192D654D29337BE5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("기사\\기사", 0x192D654D29337BE5ULL);
    thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[8] = []() -> EncryptedName<18, 0xB7AFBD2FD9FD7501ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0xB7AFBD2FD9FD7501ULL);
    thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[9] = []() -> EncryptedName<14, 0x79E915E9AF6F55DDULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x79E915E9AF6F55DDULL);
    thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[10] = []() -> EncryptedName<14, 0x674D9F3975573D41ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0x674D9F3975573D41ULL);
    thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[11] = []() -> EncryptedName<10, 0x6BDF334F2BFDAD51ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0x6BDF334F2BFDAD51ULL);
    thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[12] = []() -> EncryptedName<14, 0x136F2F9915D1DBCFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x136F2F9915D1DBCFULL);
    thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[13] = []() -> EncryptedName<28, 0x0B23D10F2F49D72BULL>& {
    constexpr std::array<char, 28> source = CRYPT_NAME("페코페코_기사\\페코페코_기사", 0x0B23D10F2F49D72BULL);
    thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[14] = []() -> EncryptedName<22, 0x1767DDE18D779D8BULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("크루세이더\\크루세이더", 0x1767DDE18D779D8BULL);
    thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[15] = []() -> EncryptedName<10, 0xC1BB8111EDD183E1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0xC1BB8111EDD183E1ULL);
    thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[16] = []() -> EncryptedName<14, 0xC5B1BBCD9BC9C997ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0xC5B1BBCD9BC9C997ULL);
    thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[17] = []() -> EncryptedName<10, 0x01CB4F1DF9D3F931ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x01CB4F1DF9D3F931ULL);
    thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[18] = []() -> EncryptedName<18, 0x49D33529F1556333ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x49D33529F1556333ULL);
    thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[19] = []() -> EncryptedName<10, 0x792FFD1325418FB5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x792FFD1325418FB5ULL);
    thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[20] = []() -> EncryptedName<10, 0xF15F8785CDE369BFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0xF15F8785CDE369BFULL);
    thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[21] = []() -> EncryptedName<34, 0xE33F2FF779878559ULL>& {
    constexpr std::array<char, 34> source = CRYPT_NAME("신페코크루세이더\\신페코크루세이더", 0xE33F2FF779878559ULL);
    thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[23] = []() -> EncryptedName<14, 0xD5A78DB38D8DAFABULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xD5A78DB38D8DAFABULL);
    thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[22] = []() -> EncryptedName<10, 0x0399459DAF454F0FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x0399459DAF454F0FULL);
    thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[26] = []() -> EncryptedName<10, 0xFFABE3D5D1BBEF73ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0xFFABE3D5D1BBEF73ULL);
    thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[27] = []() -> EncryptedName<10, 0xA597A7E7492B39D5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0xA597A7E7492B39D5ULL);
    thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[28] = []() -> EncryptedName<10, 0x0B8745519B1F95D7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x0B8745519B1F95D7ULL);
    thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[29] = []() -> EncryptedName<10, 0xC70319B5EB61598FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0xC70319B5EB61598FULL);
    thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[30] = []() -> EncryptedName<10, 0x271B45771F83DD9FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("결혼\\결혼", 0x271B45771F83DD9FULL);
    thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[24] = []() -> EncryptedName<10, 0x8783C33941E15117ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0x8783C33941E15117ULL);
    thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[25] = []() -> EncryptedName<10, 0x0F61499B4973EB5DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("닌자\\닌자", 0x0F61499B4973EB5DULL);
    thread_local EncryptedName<sizeof(source), 0x0F61499B4973EB5DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[164] = []() -> EncryptedName<10, 0xB127A19D775BF751ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("닌자\\닌자", 0xB127A19D775BF751ULL);
    thread_local EncryptedName<sizeof(source), 0xB127A19D775BF751ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[167] = []() -> EncryptedName<18, 0xA31F69E7EFED01C9ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울링커\\소울링커", 0xA31F69E7EFED01C9ULL);
    thread_local EncryptedName<sizeof(source), 0xA31F69E7EFED01C9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[165] = []() -> EncryptedName<10, 0xB7A96F8701BB4B6BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0xB7A96F8701BB4B6BULL);
    thread_local EncryptedName<sizeof(source), 0xB7A96F8701BB4B6BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[166] = []() -> EncryptedName<10, 0x659975C121557D3DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0x659975C121557D3DULL);
    thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[168] = []() -> EncryptedName<14, 0xBFF5C517A1A77F59ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0xBFF5C517A1A77F59ULL);
    thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[169] = []() -> EncryptedName<10, 0xAD6153019F93A527ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0xAD6153019F93A527ULL);
    thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[170] = []() -> EncryptedName<14, 0x85E1F5A7A5755B7FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제네릭\\제네릭", 0x85E1F5A7A5755B7FULL);
    thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[171] = []() -> EncryptedName<18, 0x454D31872BB3EDCBULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x454D31872BB3EDCBULL);
    thread_local EncryptedName<sizeof(source), 0x454D31872BB3EDCBULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[172] = []() -> EncryptedName<10, 0x81F58DED5D470D01ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x81F58DED5D470D01ULL);
    thread_local EncryptedName<sizeof(source), 0x81F58DED5D470D01ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[173] = []() -> EncryptedName<10, 0x5DE7712FAB613F75ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("권성\\권성", 0x5DE7712FAB613F75ULL);
    thread_local EncryptedName<sizeof(source), 0x5DE7712FAB613F75ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[174] = []() -> EncryptedName<14, 0x41CFCB831F6B6B91ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x41CFCB831F6B6B91ULL);
    thread_local EncryptedName<sizeof(source), 0x41CFCB831F6B6B91ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[175] = []() -> EncryptedName<10, 0x3789496BE3E1D141ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x3789496BE3E1D141ULL);
    thread_local EncryptedName<sizeof(source), 0x3789496BE3E1D141ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[176] = []() -> EncryptedName<14, 0x0F97E779A933D917ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x0F97E779A933D917ULL);
    thread_local EncryptedName<sizeof(source), 0x0F97E779A933D917ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[177] = []() -> EncryptedName<10, 0xD1056FF94D4D2773ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("슈라\\슈라", 0xD1056FF94D4D2773ULL);
    thread_local EncryptedName<sizeof(source), 0xD1056FF94D4D2773ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[178] = []() -> EncryptedName<14, 0x4331A17F33095FEBULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x4331A17F33095FEBULL);
    thread_local EncryptedName<sizeof(source), 0x4331A17F33095FEBULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[179] = []() -> EncryptedName<18, 0x4B755DB19953E311ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("아크비숍\\아크비숍", 0x4B755DB19953E311ULL);
    thread_local EncryptedName<sizeof(source), 0x4B755DB19953E311ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[180] = []() -> EncryptedName<14, 0xBD2BA1C11B01E73BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0xBD2BA1C11B01E73BULL);
    thread_local EncryptedName<sizeof(source), 0xBD2BA1C11B01E73BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[181] = []() -> EncryptedName<14, 0xED15938D333D2DB9ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0xED15938D333D2DB9ULL);
    thread_local EncryptedName<sizeof(source), 0xED15938D333D2DB9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[182] = []() -> EncryptedName<14, 0xC9712D676B5B3943ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("소서러\\소서러", 0xC9712D676B5B3943ULL);
    thread_local EncryptedName<sizeof(source), 0xC9712D676B5B3943ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[183] = []() -> EncryptedName<10, 0x9B0F234DEFDF7593ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("워록\\워록", 0x9B0F234DEFDF7593ULL);
    thread_local EncryptedName<sizeof(source), 0x9B0F234DEFDF7593ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[184] = []() -> EncryptedName<14, 0xD98F5345098D630DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0xD98F5345098D630DULL);
    thread_local EncryptedName<sizeof(source), 0xD98F5345098D630DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[185] = []() -> EncryptedName<14, 0x7F2DFFC1ABE91F2BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x7F2DFFC1ABE91F2BULL);
    thread_local EncryptedName<sizeof(source), 0x7F2DFFC1ABE91F2BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[186] = []() -> EncryptedName<14, 0xFD69A9B3F311818DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0xFD69A9B3F311818DULL);
    thread_local EncryptedName<sizeof(source), 0xFD69A9B3F311818DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[187] = []() -> EncryptedName<18, 0xD9452D81E5CD99C1ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0xD9452D81E5CD99C1ULL);
    thread_local EncryptedName<sizeof(source), 0xD9452D81E5CD99C1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[188] = []() -> EncryptedName<14, 0xADADEB4DF7813B55ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0xADADEB4DF7813B55ULL);
    thread_local EncryptedName<sizeof(source), 0xADADEB4DF7813B55ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[189] = []() -> EncryptedName<10, 0xB195637DAFCB0D59ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0xB195637DAFCB0D59ULL);
    thread_local EncryptedName<sizeof(source), 0xB195637DAFCB0D59ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[190] = []() -> EncryptedName<14, 0xD7A749EB27D5BF6DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xD7A749EB27D5BF6DULL);
    thread_local EncryptedName<sizeof(source), 0xD7A749EB27D5BF6DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[191] = []() -> EncryptedName<10, 0x9569C9C7E70909AFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0x9569C9C7E70909AFULL);
    thread_local EncryptedName<sizeof(source), 0x9569C9C7E70909AFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[192] = []() -> EncryptedName<10, 0xB19D1FA9116F39D3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xB19D1FA9116F39D3ULL);
    thread_local EncryptedName<sizeof(source), 0xB19D1FA9116F39D3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[193] = []() -> EncryptedName<10, 0x4191BB5531FD3999ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x4191BB5531FD3999ULL);
    thread_local EncryptedName<sizeof(source), 0x4191BB5531FD3999ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[194] = []() -> EncryptedName<10, 0xF715514F5961B75BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xF715514F5961B75BULL);
    thread_local EncryptedName<sizeof(source), 0xF715514F5961B75BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[195] = []() -> EncryptedName<14, 0x2F8B0F37118D157FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x2F8B0F37118D157FULL);
    thread_local EncryptedName<sizeof(source), 0x2F8B0F37118D157FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[196] = []() -> EncryptedName<14, 0xAD075B8DB599F1CDULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xAD075B8DB599F1CDULL);
    thread_local EncryptedName<sizeof(source), 0xAD075B8DB599F1CDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[197] = []() -> EncryptedName<10, 0x2FE993D1DDE1F197ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x2FE993D1DDE1F197ULL);
    thread_local EncryptedName<sizeof(source), 0x2FE993D1DDE1F197ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[198] = []() -> EncryptedName<18, 0x39FF7FB50DEB998FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("민스트럴\\민스트럴", 0x39FF7FB50DEB998FULL);
    thread_local EncryptedName<sizeof(source), 0x39FF7FB50DEB998FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[199] = []() -> EncryptedName<10, 0xB7FD8F670FC7B96BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0xB7FD8F670FC7B96BULL);
    thread_local EncryptedName<sizeof(source), 0xB7FD8F670FC7B96BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[200] = []() -> EncryptedName<10, 0xE7978FB5A94DD7FDULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xE7978FB5A94DD7FDULL);
    thread_local EncryptedName<sizeof(source), 0xE7978FB5A94DD7FDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[201] = []() -> EncryptedName<10, 0x4941FD9F21BBB9FDULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x4941FD9F21BBB9FDULL);
    thread_local EncryptedName<sizeof(source), 0x4941FD9F21BBB9FDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[202] = []() -> EncryptedName<10, 0x35E5656B5DEDCD75ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x35E5656B5DEDCD75ULL);
    thread_local EncryptedName<sizeof(source), 0x35E5656B5DEDCD75ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[203] = []() -> EncryptedName<10, 0xF14F79170939C7F1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0xF14F79170939C7F1ULL);
    thread_local EncryptedName<sizeof(source), 0xF14F79170939C7F1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[204] = []() -> EncryptedName<10, 0xA5FD29D7AB6DC59FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xA5FD29D7AB6DC59FULL);
    thread_local EncryptedName<sizeof(source), 0xA5FD29D7AB6DC59FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[205] = []() -> EncryptedName<18, 0xD97D5DBDBB2F2709ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("태권소년\\태권소년", 0xD97D5DBDBB2F2709ULL);
    thread_local EncryptedName<sizeof(source), 0xD97D5DBDBB2F2709ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[206] = []() -> EncryptedName<18, 0xED796785D1DF137FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0xED796785D1DF137FULL);
    thread_local EncryptedName<sizeof(source), 0xED796785D1DF137FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[207] = []() -> EncryptedName<18, 0x83EFBFBB37EDBDBDULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x83EFBFBB37EDBDBDULL);
    thread_local EncryptedName<sizeof(source), 0x83EFBFBB37EDBDBDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[208] = []() -> EncryptedName<14, 0xBF2B4B31397FF5A7ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xBF2B4B31397FF5A7ULL);
    thread_local EncryptedName<sizeof(source), 0xBF2B4B31397FF5A7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[209] = []() -> EncryptedName<10, 0xDD3913D5D7F97BE1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xDD3913D5D7F97BE1ULL);
    thread_local EncryptedName<sizeof(source), 0xDD3913D5D7F97BE1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[210] = []() -> EncryptedName<14, 0x050505679903579BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x050505679903579BULL);
    thread_local EncryptedName<sizeof(source), 0x050505679903579BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[211] = []() -> EncryptedName<10, 0x03772763F96D9DA5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x03772763F96D9DA5ULL);
    thread_local EncryptedName<sizeof(source), 0x03772763F96D9DA5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[212] = []() -> EncryptedName<14, 0x0B7109397F6B1387ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x0B7109397F6B1387ULL);
    thread_local EncryptedName<sizeof(source), 0x0B7109397F6B1387ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[213] = []() -> EncryptedName<10, 0x23BBD58F8B5FF5C9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0x23BBD58F8B5FF5C9ULL);
    thread_local EncryptedName<sizeof(source), 0x23BBD58F8B5FF5C9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[214] = []() -> EncryptedName<10, 0xC517FB932D95BB6DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xC517FB932D95BB6DULL);
    thread_local EncryptedName<sizeof(source), 0xC517FB932D95BB6DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[215] = []() -> EncryptedName<14, 0x9949BFD527532943ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0x9949BFD527532943ULL);
    thread_local EncryptedName<sizeof(source), 0x9949BFD527532943ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[216] = []() -> EncryptedName<10, 0xCD8F077123FB7D6FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0xCD8F077123FB7D6FULL);
    thread_local EncryptedName<sizeof(source), 0xCD8F077123FB7D6FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[217] = []() -> EncryptedName<14, 0x13E33FA1A591AB67ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x13E33FA1A591AB67ULL);
    thread_local EncryptedName<sizeof(source), 0x13E33FA1A591AB67ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[218] = []() -> EncryptedName<10, 0x17A9E5BF39AD4737ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x17A9E5BF39AD4737ULL);
    thread_local EncryptedName<sizeof(source), 0x17A9E5BF39AD4737ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[219] = []() -> EncryptedName<18, 0x758F1D0B4F7705B9ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x758F1D0B4F7705B9ULL);
    thread_local EncryptedName<sizeof(source), 0x758F1D0B4F7705B9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[220] = []() -> EncryptedName<10, 0x891B232D09451965ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x891B232D09451965ULL);
    thread_local EncryptedName<sizeof(source), 0x891B232D09451965ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[221] = []() -> EncryptedName<10, 0x637B959D55771F69ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x637B959D55771F69ULL);
    thread_local EncryptedName<sizeof(source), 0x637B959D55771F69ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[222] = []() -> EncryptedName<14, 0xD1413DBB01CD8B51ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xD1413DBB01CD8B51ULL);
    thread_local EncryptedName<sizeof(source), 0xD1413DBB01CD8B51ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[223] = []() -> EncryptedName<10, 0x75ADC57D87EFBD8FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("워록\\워록", 0x75ADC57D87EFBD8FULL);
    thread_local EncryptedName<sizeof(source), 0x75ADC57D87EFBD8FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[224] = []() -> EncryptedName<18, 0x2BAD156DE3E1F9D9ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("아크비숍\\아크비숍", 0x2BAD156DE3E1F9D9ULL);
    thread_local EncryptedName<sizeof(source), 0x2BAD156DE3E1F9D9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[225] = []() -> EncryptedName<14, 0xF161095D0DE18F95ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xF161095D0DE18F95ULL);
    thread_local EncryptedName<sizeof(source), 0xF161095D0DE18F95ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[226] = []() -> EncryptedName<14, 0xEDC74F6D13DF71EFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("소서러\\소서러", 0xEDC74F6D13DF71EFULL);
    thread_local EncryptedName<sizeof(source), 0xEDC74F6D13DF71EFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[227] = []() -> EncryptedName<18, 0xA12DAB392545D72FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("민스트럴\\민스트럴", 0xA12DAB392545D72FULL);
    thread_local EncryptedName<sizeof(source), 0xA12DAB392545D72FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[228] = []() -> EncryptedName<10, 0x6BA1C1132F033B75ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x6BA1C1132F033B75ULL);
    thread_local EncryptedName<sizeof(source), 0x6BA1C1132F033B75ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[229] = []() -> EncryptedName<10, 0x2FEB8739E5975BD1ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("슈라\\슈라", 0x2FEB8739E5975BD1ULL);
    thread_local EncryptedName<sizeof(source), 0x2FEB8739E5975BD1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[230] = []() -> EncryptedName<14, 0x33133B977BCD03CDULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제네릭\\제네릭", 0x33133B977BCD03CDULL);
    thread_local EncryptedName<sizeof(source), 0x33133B977BCD03CDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[232] = []() -> EncryptedName<10, 0x6FE153231D879777ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x6FE153231D879777ULL);
    thread_local EncryptedName<sizeof(source), 0x6FE153231D879777ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[233] = []() -> EncryptedName<14, 0x39A39F87DD5F535BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x39A39F87DD5F535BULL);
    thread_local EncryptedName<sizeof(source), 0x39A39F87DD5F535BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[234] = []() -> EncryptedName<10, 0xEF6D1D893F2B2DC5ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xEF6D1D893F2B2DC5ULL);
    thread_local EncryptedName<sizeof(source), 0xEF6D1D893F2B2DC5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[235] = []() -> EncryptedName<14, 0x234977DDB54BC71DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x234977DDB54BC71DULL);
    thread_local EncryptedName<sizeof(source), 0x234977DDB54BC71DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[236] = []() -> EncryptedName<10, 0xE52DF30DCBAF7125ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0xE52DF30DCBAF7125ULL);
    thread_local EncryptedName<sizeof(source), 0xE52DF30DCBAF7125ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[237] = []() -> EncryptedName<14, 0xCFF3899D95AF355FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0xCFF3899D95AF355FULL);
    thread_local EncryptedName<sizeof(source), 0xCFF3899D95AF355FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[238] = []() -> EncryptedName<10, 0x9171997D6963C9F7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0x9171997D6963C9F7ULL);
    thread_local EncryptedName<sizeof(source), 0x9171997D6963C9F7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[239] = []() -> EncryptedName<10, 0x4F99DF03C3EF31B7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0x4F99DF03C3EF31B7ULL);
    thread_local EncryptedName<sizeof(source), 0x4F99DF03C3EF31B7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[231] = []() -> EncryptedName<10, 0x8D1D0FF1AF2B13F7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0x8D1D0FF1AF2B13F7ULL);
    thread_local EncryptedName<sizeof(source), 0x8D1D0FF1AF2B13F7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[51] = []() -> EncryptedName<14, 0xB39FA7917BCFBB8DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xB39FA7917BCFBB8DULL);
    thread_local EncryptedName<sizeof(source), 0xB39FA7917BCFBB8DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[52] = []() -> EncryptedName<10, 0x2DCB7567394DC1ABULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0x2DCB7567394DC1ABULL);
    thread_local EncryptedName<sizeof(source), 0x2DCB7567394DC1ABULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[53] = []() -> EncryptedName<14, 0x4F637DC51307B759ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x4F637DC51307B759ULL);
    thread_local EncryptedName<sizeof(source), 0x4F637DC51307B759ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[54] = []() -> EncryptedName<10, 0x452FEF0D7FC95565ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x452FEF0D7FC95565ULL);
    thread_local EncryptedName<sizeof(source), 0x452FEF0D7FC95565ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[55] = []() -> EncryptedName<14, 0x9FED7DA189490D5DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x9FED7DA189490D5DULL);
    thread_local EncryptedName<sizeof(source), 0x9FED7DA189490D5DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[56] = []() -> EncryptedName<10, 0x839B27DF8F5967B3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0x839B27DF8F5967B3ULL);
    thread_local EncryptedName<sizeof(source), 0x839B27DF8F5967B3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[57] = []() -> EncryptedName<10, 0xF3CB6F7711A71B6DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0xF3CB6F7711A71B6DULL);
    thread_local EncryptedName<sizeof(source), 0xF3CB6F7711A71B6DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[58] = []() -> EncryptedName<10, 0x2D51E3B7236175EDULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("기사\\기사", 0x2D51E3B7236175EDULL);
    thread_local EncryptedName<sizeof(source), 0x2D51E3B7236175EDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[59] = []() -> EncryptedName<18, 0x0F41EBBDA32D1B17ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x0F41EBBDA32D1B17ULL);
    thread_local EncryptedName<sizeof(source), 0x0F41EBBDA32D1B17ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[60] = []() -> EncryptedName<14, 0xFBD599E117C547C1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0xFBD599E117C547C1ULL);
    thread_local EncryptedName<sizeof(source), 0xFBD599E117C547C1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[61] = []() -> EncryptedName<14, 0x176F138D5DD7F365ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0x176F138D5DD7F365ULL);
    thread_local EncryptedName<sizeof(source), 0x176F138D5DD7F365ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[62] = []() -> EncryptedName<10, 0x793F370985ED674FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0x793F370985ED674FULL);
    thread_local EncryptedName<sizeof(source), 0x793F370985ED674FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[63] = []() -> EncryptedName<14, 0xE55723712993051DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xE55723712993051DULL);
    thread_local EncryptedName<sizeof(source), 0xE55723712993051DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[64] = []() -> EncryptedName<28, 0x7115D9F973F913C9ULL>& {
    constexpr std::array<char, 28> source = CRYPT_NAME("페코페코_기사\\페코페코_기사", 0x7115D9F973F913C9ULL);
    thread_local EncryptedName<sizeof(source), 0x7115D9F973F913C9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[65] = []() -> EncryptedName<22, 0xCB1D253B7FE1F563ULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("크루세이더\\크루세이더", 0xCB1D253B7FE1F563ULL);
    thread_local EncryptedName<sizeof(source), 0xCB1D253B7FE1F563ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[66] = []() -> EncryptedName<10, 0x7941D76335CFA14FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x7941D76335CFA14FULL);
    thread_local EncryptedName<sizeof(source), 0x7941D76335CFA14FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[67] = []() -> EncryptedName<14, 0x7321C3814F8799E1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x7321C3814F8799E1ULL);
    thread_local EncryptedName<sizeof(source), 0x7321C3814F8799E1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[68] = []() -> EncryptedName<10, 0x7DE349D717D7F123ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x7DE349D717D7F123ULL);
    thread_local EncryptedName<sizeof(source), 0x7DE349D717D7F123ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[69] = []() -> EncryptedName<18, 0x0F15B16B5BAFD57FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x0F15B16B5BAFD57FULL);
    thread_local EncryptedName<sizeof(source), 0x0F15B16B5BAFD57FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[70] = []() -> EncryptedName<10, 0x9B5B959B954D9333ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x9B5B959B954D9333ULL);
    thread_local EncryptedName<sizeof(source), 0x9B5B959B954D9333ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[71] = []() -> EncryptedName<10, 0x17FFC98B398FAF61ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x17FFC98B398FAF61ULL);
    thread_local EncryptedName<sizeof(source), 0x17FFC98B398FAF61ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[72] = []() -> EncryptedName<34, 0x078143534999254DULL>& {
    constexpr std::array<char, 34> source = CRYPT_NAME("신페코크루세이더\\신페코크루세이더", 0x078143534999254DULL);
    thread_local EncryptedName<sizeof(source), 0x078143534999254DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[73] = []() -> EncryptedName<14, 0xAFA5617F178B6185ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xAFA5617F178B6185ULL);
    thread_local EncryptedName<sizeof(source), 0xAFA5617F178B6185ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[74] = []() -> EncryptedName<10, 0xE50B2935073DD3E9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("검사\\검사", 0xE50B2935073DD3E9ULL);
    thread_local EncryptedName<sizeof(source), 0xE50B2935073DD3E9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[75] = []() -> EncryptedName<14, 0x3F5315DFE32B4FAFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("마법사\\마법사", 0x3F5315DFE32B4FAFULL);
    thread_local EncryptedName<sizeof(source), 0x3F5315DFE32B4FAFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[76] = []() -> EncryptedName<10, 0x33832D4D3FDFBDBFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("궁수\\궁수", 0x33832D4D3FDFBDBFULL);
    thread_local EncryptedName<sizeof(source), 0x33832D4D3FDFBDBFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[77] = []() -> EncryptedName<14, 0x7D750DADBDE9C3F1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0x7D750DADBDE9C3F1ULL);
    thread_local EncryptedName<sizeof(source), 0x7D750DADBDE9C3F1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[78] = []() -> EncryptedName<10, 0xED5143E7C1B39315ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("상인\\상인", 0xED5143E7C1B39315ULL);
    thread_local EncryptedName<sizeof(source), 0xED5143E7C1B39315ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[79] = []() -> EncryptedName<10, 0xFF7D83CB791339D9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("도둑\\도둑", 0xFF7D83CB791339D9ULL);
    thread_local EncryptedName<sizeof(source), 0xFF7D83CB791339D9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[80] = []() -> EncryptedName<10, 0x31956551B311FD95ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("기사\\기사", 0x31956551B311FD95ULL);
    thread_local EncryptedName<sizeof(source), 0x31956551B311FD95ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[81] = []() -> EncryptedName<18, 0x393D916561BDC391ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("프리스트\\프리스트", 0x393D916561BDC391ULL);
    thread_local EncryptedName<sizeof(source), 0x393D916561BDC391ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[82] = []() -> EncryptedName<14, 0x955D4DF94DE9BB07ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("위저드\\위저드", 0x955D4DF94DE9BB07ULL);
    thread_local EncryptedName<sizeof(source), 0x955D4DF94DE9BB07ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[83] = []() -> EncryptedName<14, 0x8741152D8FB34B4FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제철공\\제철공", 0x8741152D8FB34B4FULL);
    thread_local EncryptedName<sizeof(source), 0x8741152D8FB34B4FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[84] = []() -> EncryptedName<10, 0xB739073FE921AD6FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("헌터\\헌터", 0xB739073FE921AD6FULL);
    thread_local EncryptedName<sizeof(source), 0xB739073FE921AD6FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[85] = []() -> EncryptedName<14, 0xC399B7091589B50BULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("어세신\\어세신", 0xC399B7091589B50BULL);
    thread_local EncryptedName<sizeof(source), 0xC399B7091589B50BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[86] = []() -> EncryptedName<28, 0x7FC1015DFFCF6D19ULL>& {
    constexpr std::array<char, 28> source = CRYPT_NAME("페코페코_기사\\페코페코_기사", 0x7FC1015DFFCF6D19ULL);
    thread_local EncryptedName<sizeof(source), 0x7FC1015DFFCF6D19ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[87] = []() -> EncryptedName<22, 0x0753A7E1EFA9E139ULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("크루세이더\\크루세이더", 0x0753A7E1EFA9E139ULL);
    thread_local EncryptedName<sizeof(source), 0x0753A7E1EFA9E139ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[88] = []() -> EncryptedName<10, 0x7FCD77F9C7CBD14DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("몽크\\몽크", 0x7FCD77F9C7CBD14DULL);
    thread_local EncryptedName<sizeof(source), 0x7FCD77F9C7CBD14DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[89] = []() -> EncryptedName<14, 0xCB3FBD4183DB8323ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0xCB3FBD4183DB8323ULL);
    thread_local EncryptedName<sizeof(source), 0xCB3FBD4183DB8323ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[90] = []() -> EncryptedName<10, 0x3B915119EFD523EFULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x3B915119EFD523EFULL);
    thread_local EncryptedName<sizeof(source), 0x3B915119EFD523EFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[91] = []() -> EncryptedName<18, 0x5B8F49AF09390D1BULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("연금술사\\연금술사", 0x5B8F49AF09390D1BULL);
    thread_local EncryptedName<sizeof(source), 0x5B8F49AF09390D1BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[92] = []() -> EncryptedName<10, 0x3F557BE333F95F6DULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("바드\\바드", 0x3F557BE333F95F6DULL);
    thread_local EncryptedName<sizeof(source), 0x3F557BE333F95F6DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[93] = []() -> EncryptedName<10, 0x474DC581D5E98357ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("무희\\무희", 0x474DC581D5E98357ULL);
    thread_local EncryptedName<sizeof(source), 0x474DC581D5E98357ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[94] = []() -> EncryptedName<34, 0x01FF9313695DD9DFULL>& {
    constexpr std::array<char, 34> source = CRYPT_NAME("구페코크루세이더\\구페코크루세이더", 0x01FF9313695DD9DFULL);
    thread_local EncryptedName<sizeof(source), 0x01FF9313695DD9DFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[95] = []() -> EncryptedName<14, 0x1BF915E1F37DFDFDULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x1BF915E1F37DFDFDULL);
    thread_local EncryptedName<sizeof(source), 0x1BF915E1F37DFDFDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[96] = []() -> EncryptedName<18, 0x49C3759B77DBD3C1ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("태권소년\\태권소년", 0x49C3759B77DBD3C1ULL);
    thread_local EncryptedName<sizeof(source), 0x49C3759B77DBD3C1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[97] = []() -> EncryptedName<10, 0x6DED3DD1CF75AD9BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("권성\\권성", 0x6DED3DD1CF75AD9BULL);
    thread_local EncryptedName<sizeof(source), 0x6DED3DD1CF75AD9BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[98] = []() -> EncryptedName<10, 0x91E147EBBD47C39FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("권성\\권성", 0x91E147EBBD47C39FULL);
    thread_local EncryptedName<sizeof(source), 0x91E147EBBD47C39FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[99] = []() -> EncryptedName<18, 0xD1538513DF975701ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울링커\\소울링커", 0xD1538513DF975701ULL);
    thread_local EncryptedName<sizeof(source), 0xD1538513DF975701ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[100] = []() -> EncryptedName<14, 0xA1A55B4D1597EDD3ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("성직자\\성직자", 0xA1A55B4D1597EDD3ULL);
    thread_local EncryptedName<sizeof(source), 0xA1A55B4D1597EDD3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[101] = []() -> EncryptedName<10, 0x4F1791FB1503C713ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("기사\\기사", 0x4F1791FB1503C713ULL);
    thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[102] = []() -> EncryptedName<14, 0x7D993F650D2785FFULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("세이지\\세이지", 0x7D993F650D2785FFULL);
    thread_local EncryptedName<sizeof(source), 0x7D993F650D2785FFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[104] = []() -> EncryptedName<18, 0x61FF87ED7BF39BE1ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("룬나이트\\룬나이트", 0x61FF87ED7BF39BE1ULL);
    thread_local EncryptedName<sizeof(source), 0x61FF87ED7BF39BE1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[110] = []() -> EncryptedName<18, 0x151F77EBEBBB2147ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("룬나이트\\룬나이트", 0x151F77EBEBBB2147ULL);
    thread_local EncryptedName<sizeof(source), 0x151F77EBEBBB2147ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[130] = []() -> EncryptedName<36, 0x2D394DBD0F332733ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x2D394DBD0F332733ULL);
    thread_local EncryptedName<sizeof(source), 0x2D394DBD0F332733ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[131] = []() -> EncryptedName<36, 0x69D345DB950F336DULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x69D345DB950F336DULL);
    thread_local EncryptedName<sizeof(source), 0x69D345DB950F336DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[159] = []() -> EncryptedName<36, 0xC7912587FFF31B71ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0xC7912587FFF31B71ULL);
    thread_local EncryptedName<sizeof(source), 0xC7912587FFF31B71ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[138] = []() -> EncryptedName<36, 0x2D3B15774149D5DDULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x2D3B15774149D5DDULL);
    thread_local EncryptedName<sizeof(source), 0x2D3B15774149D5DDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[139] = []() -> EncryptedName<36, 0x9B17C9EFDBB39F2FULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x9B17C9EFDBB39F2FULL);
    thread_local EncryptedName<sizeof(source), 0x9B17C9EFDBB39F2FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[140] = []() -> EncryptedName<36, 0x0BEB63A5C905B3A7ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x0BEB63A5C905B3A7ULL);
    thread_local EncryptedName<sizeof(source), 0x0BEB63A5C905B3A7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[141] = []() -> EncryptedName<36, 0x8B3FEB5B8BD5C3C3ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x8B3FEB5B8BD5C3C3ULL);
    thread_local EncryptedName<sizeof(source), 0x8B3FEB5B8BD5C3C3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[142] = []() -> EncryptedName<36, 0x8F1B5BFD5F6175B5ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x8F1B5BFD5F6175B5ULL);
    thread_local EncryptedName<sizeof(source), 0x8F1B5BFD5F6175B5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[143] = []() -> EncryptedName<36, 0x078333BB9725B7C9ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x078333BB9725B7C9ULL);
    thread_local EncryptedName<sizeof(source), 0x078333BB9725B7C9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[144] = []() -> EncryptedName<36, 0xF5657D1F3DE5CDC7ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0xF5657D1F3DE5CDC7ULL);
    thread_local EncryptedName<sizeof(source), 0xF5657D1F3DE5CDC7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[145] = []() -> EncryptedName<36, 0x69FD1D9733D3CFC1ULL>& {
    constexpr std::array<char, 36> source = CRYPT_NAME("페코페코_룬나이트\\페코페코_룬나이트", 0x69FD1D9733D3CFC1ULL);
    thread_local EncryptedName<sizeof(source), 0x69FD1D9733D3CFC1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[107] = []() -> EncryptedName<18, 0xF7530137154D8FE5ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("아크비숍\\아크비숍", 0xF7530137154D8FE5ULL);
    thread_local EncryptedName<sizeof(source), 0xF7530137154D8FE5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[113] = []() -> EncryptedName<18, 0x8B65CD61012B5931ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("아크비숍\\아크비숍", 0x8B65CD61012B5931ULL);
    thread_local EncryptedName<sizeof(source), 0x8B65CD61012B5931ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[105] = []() -> EncryptedName<10, 0x17359BF953C367B3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("워록\\워록", 0x17359BF953C367B3ULL);
    thread_local EncryptedName<sizeof(source), 0x17359BF953C367B3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[111] = []() -> EncryptedName<10, 0x2FF3C5036D070127ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("워록\\워록", 0x2FF3C5036D070127ULL);
    thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[108] = []() -> EncryptedName<14, 0x93E7A3A1F587CDC5ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("미케닉\\미케닉", 0x93E7A3A1F587CDC5ULL);
    thread_local EncryptedName<sizeof(source), 0x93E7A3A1F587CDC5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[114] = []() -> EncryptedName<14, 0xC52FB5FBB94FF917ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("미케닉\\미케닉", 0xC52FB5FBB94FF917ULL);
    thread_local EncryptedName<sizeof(source), 0xC52FB5FBB94FF917ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[136] = []() -> EncryptedName<18, 0x812BE72BCD5707B7ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("마도기어\\마도기어", 0x812BE72BCD5707B7ULL);
    thread_local EncryptedName<sizeof(source), 0x812BE72BCD5707B7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[137] = []() -> EncryptedName<18, 0x59278F63D79F3DA1ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("마도기어\\마도기어", 0x59278F63D79F3DA1ULL);
    thread_local EncryptedName<sizeof(source), 0x59278F63D79F3DA1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[162] = []() -> EncryptedName<18, 0x1D6D75EDE7CD311FULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("마도기어\\마도기어", 0x1D6D75EDE7CD311FULL);
    thread_local EncryptedName<sizeof(source), 0x1D6D75EDE7CD311FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[106] = []() -> EncryptedName<14, 0x3375B1C7DBB5FD55ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("레인져\\레인져", 0x3375B1C7DBB5FD55ULL);
    thread_local EncryptedName<sizeof(source), 0x3375B1C7DBB5FD55ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[112] = []() -> EncryptedName<14, 0x0F0F8B6B8F9D19D1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("레인져\\레인져", 0x0F0F8B6B8F9D19D1ULL);
    thread_local EncryptedName<sizeof(source), 0x0F0F8B6B8F9D19D1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[134] = []() -> EncryptedName<22, 0xD98F03A52D35636DULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("레인져늑대\\레인져늑대", 0xD98F03A52D35636DULL);
    thread_local EncryptedName<sizeof(source), 0xD98F03A52D35636DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[135] = []() -> EncryptedName<22, 0x31D7C1EDA345E36DULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("레인져늑대\\레인져늑대", 0x31D7C1EDA345E36DULL);
    thread_local EncryptedName<sizeof(source), 0x31D7C1EDA345E36DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[161] = []() -> EncryptedName<22, 0xD3755DD95F47A909ULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("레인져늑대\\레인져늑대", 0xD3755DD95F47A909ULL);
    thread_local EncryptedName<sizeof(source), 0xD3755DD95F47A909ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[109] = []() -> EncryptedName<26, 0x490FFB43A12F0DB9ULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("길로틴크로스\\길로틴크로스", 0x490FFB43A12F0DB9ULL);
    thread_local EncryptedName<sizeof(source), 0x490FFB43A12F0DB9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[115] = []() -> EncryptedName<26, 0x3FAF85D995034FD7ULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("길로틴크로스\\길로틴크로스", 0x3FAF85D995034FD7ULL);
    thread_local EncryptedName<sizeof(source), 0x3FAF85D995034FD7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[116] = []() -> EncryptedName<18, 0x73D1674B61DF7B0BULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("로얄가드\\로얄가드", 0x73D1674B61DF7B0BULL);
    thread_local EncryptedName<sizeof(source), 0x73D1674B61DF7B0BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[123] = []() -> EncryptedName<18, 0x91651D81F559F971ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("로얄가드\\로얄가드", 0x91651D81F559F971ULL);
    thread_local EncryptedName<sizeof(source), 0x91651D81F559F971ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[132] = []() -> EncryptedName<30, 0x072D2D872D259D5BULL>& {
    constexpr std::array<char, 30> source = CRYPT_NAME("신페코로얄가드\\신페코로얄가드", 0x072D2D872D259D5BULL);
    thread_local EncryptedName<sizeof(source), 0x072D2D872D259D5BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[133] = []() -> EncryptedName<30, 0x875FCDF1E7A1795FULL>& {
    constexpr std::array<char, 30> source = CRYPT_NAME("신페코로얄가드\\신페코로얄가드", 0x875FCDF1E7A1795FULL);
    thread_local EncryptedName<sizeof(source), 0x875FCDF1E7A1795FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[160] = []() -> EncryptedName<30, 0xB96B77B7255F8569ULL>& {
    constexpr std::array<char, 30> source = CRYPT_NAME("신페코로얄가드\\신페코로얄가드", 0xB96B77B7255F8569ULL);
    thread_local EncryptedName<sizeof(source), 0xB96B77B7255F8569ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[120] = []() -> EncryptedName<10, 0x9BE3EF737387AD79ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("슈라\\슈라", 0x9BE3EF737387AD79ULL);
    thread_local EncryptedName<sizeof(source), 0x9BE3EF737387AD79ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[127] = []() -> EncryptedName<10, 0x4755C1355575FD37ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("슈라\\슈라", 0x4755C1355575FD37ULL);
    thread_local EncryptedName<sizeof(source), 0x4755C1355575FD37ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[117] = []() -> EncryptedName<14, 0x11CB35636377DD07ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("소서러\\소서러", 0x11CB35636377DD07ULL);
    thread_local EncryptedName<sizeof(source), 0x11CB35636377DD07ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[124] = []() -> EncryptedName<14, 0x79DF094DD9616575ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("소서러\\소서러", 0x79DF094DD9616575ULL);
    thread_local EncryptedName<sizeof(source), 0x79DF094DD9616575ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[122] = []() -> EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x9D8F2D37BFFD61B7ULL);
    thread_local EncryptedName<sizeof(source), 0x9D8F2D37BFFD61B7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[129] = []() -> EncryptedName<10, 0x5DAD2D25D9259B93ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0x5DAD2D25D9259B93ULL);
    thread_local EncryptedName<sizeof(source), 0x5DAD2D25D9259B93ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[121] = []() -> EncryptedName<14, 0x1F1D51274373C1FDULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제네릭\\제네릭", 0x1F1D51274373C1FDULL);
    thread_local EncryptedName<sizeof(source), 0x1F1D51274373C1FDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[128] = []() -> EncryptedName<14, 0x3D371F5B0F7F9327ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제네릭\\제네릭", 0x3D371F5B0F7F9327ULL);
    thread_local EncryptedName<sizeof(source), 0x3D371F5B0F7F9327ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[119] = []() -> EncryptedName<14, 0x6BF729E30729CF29ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("원더러\\원더러", 0x6BF729E30729CF29ULL);
    thread_local EncryptedName<sizeof(source), 0x6BF729E30729CF29ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[126] = []() -> EncryptedName<14, 0xC5995F4B9FE96B89ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("원더러\\원더러", 0xC5995F4B9FE96B89ULL);
    thread_local EncryptedName<sizeof(source), 0xC5995F4B9FE96B89ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[118] = []() -> EncryptedName<18, 0x610F7DD1EFC1C91BULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("민스트럴\\민스트럴", 0x610F7DD1EFC1C91BULL);
    thread_local EncryptedName<sizeof(source), 0x610F7DD1EFC1C91BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[125] = []() -> EncryptedName<18, 0x5F93A1C3A7AD7537ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("민스트럴\\민스트럴", 0x5F93A1C3A7AD7537ULL);
    thread_local EncryptedName<sizeof(source), 0x5F93A1C3A7AD7537ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[146] = []() -> EncryptedName<18, 0x097389157345A787ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("룬나이트\\룬나이트", 0x097389157345A787ULL);
    thread_local EncryptedName<sizeof(source), 0x097389157345A787ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[147] = []() -> EncryptedName<10, 0x732B390D63FD47D7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("워록\\워록", 0x732B390D63FD47D7ULL);
    thread_local EncryptedName<sizeof(source), 0x732B390D63FD47D7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[148] = []() -> EncryptedName<14, 0xB7A76BF9CD41F99FULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("레인져\\레인져", 0xB7A76BF9CD41F99FULL);
    thread_local EncryptedName<sizeof(source), 0xB7A76BF9CD41F99FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[149] = []() -> EncryptedName<18, 0xE3FB23537D2785BFULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("아크비숍\\아크비숍", 0xE3FB23537D2785BFULL);
    thread_local EncryptedName<sizeof(source), 0xE3FB23537D2785BFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[150] = []() -> EncryptedName<14, 0x1FDB81175D1557E3ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("미케닉\\미케닉", 0x1FDB81175D1557E3ULL);
    thread_local EncryptedName<sizeof(source), 0x1FDB81175D1557E3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[151] = []() -> EncryptedName<26, 0xB3CF9BEB7963DB89ULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("길로틴크로스\\길로틴크로스", 0xB3CF9BEB7963DB89ULL);
    thread_local EncryptedName<sizeof(source), 0xB3CF9BEB7963DB89ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[152] = []() -> EncryptedName<18, 0x8BBF792B47E72173ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("로얄가드\\로얄가드", 0x8BBF792B47E72173ULL);
    thread_local EncryptedName<sizeof(source), 0x8BBF792B47E72173ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[153] = []() -> EncryptedName<14, 0xDD69EDFFA18F537DULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("소서러\\소서러", 0xDD69EDFFA18F537DULL);
    thread_local EncryptedName<sizeof(source), 0xDD69EDFFA18F537DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[154] = []() -> EncryptedName<18, 0x71A509DB43792991ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("민스트럴\\민스트럴", 0x71A509DB43792991ULL);
    thread_local EncryptedName<sizeof(source), 0x71A509DB43792991ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[155] = []() -> EncryptedName<14, 0xFB2BC75769E777D3ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("원더러\\원더러", 0xFB2BC75769E777D3ULL);
    thread_local EncryptedName<sizeof(source), 0xFB2BC75769E777D3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[156] = []() -> EncryptedName<10, 0x038535BB092533B7ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("슈라\\슈라", 0x038535BB092533B7ULL);
    thread_local EncryptedName<sizeof(source), 0x038535BB092533B7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[157] = []() -> EncryptedName<14, 0x9B9D51AD3515F779ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("제네릭\\제네릭", 0x9B9D51AD3515F779ULL);
    thread_local EncryptedName<sizeof(source), 0x9B9D51AD3515F779ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[158] = []() -> EncryptedName<10, 0xB5D7B5AF4FEDFF57ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("로그\\로그", 0xB5D7B5AF4FEDFF57ULL);
    thread_local EncryptedName<sizeof(source), 0xB5D7B5AF4FEDFF57ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[240] = []() -> EncryptedName<14, 0x67DD5BAB1919D9D1ULL>& {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0x67DD5BAB1919D9D1ULL);
    thread_local EncryptedName<sizeof(source), 0x67DD5BAB1919D9D1ULL> name(source);
    return name;
  }().Text();
  {
    constexpr std::array<char, 14> source = CRYPT_NAME("초보자\\초보자", 0xB5F3F779237315F3ULL);
    thread_local EncryptedName<sizeof(source), 0xB5F3F779237315F3ULL> name(source);
    jobSpriteNames[241] = name.Text();
  }
  jobSpriteNames[242] = PoringSuperNovice2SpriteName().Text();
  jobSpriteNames[243] = PoringSuperNovice2BabySpriteName().Text();
  jobSpriteNames[244] = SheepPriestBabySpriteName().Text();
  jobSpriteNames[245] = FoxWizardBabySpriteName().Text();
  jobSpriteNames[246] = PigBlacksmithBabySpriteName().Text();
  jobSpriteNames[247] = PigMechanicSpriteName().Text();
  jobSpriteNames[255] = PigMechanicBabySpriteName().Text();
  jobSpriteNames[248] = OstrichRangerSpriteName().Text();
  jobSpriteNames[256] = OstrichRangerBabySpriteName().Text();
  jobSpriteNames[249] = LionKnightSpriteName().Text();
  jobSpriteNames[257] = LionKnightBabySpriteName().Text();
  jobSpriteNames[250] = LionKnightHighSpriteName().Text();
  jobSpriteNames[251] = LionRoyalGuardSpriteName().Text();
  jobSpriteNames[258] = LionRoyalGuardBabySpriteName().Text();
  jobSpriteNames[252] = LionRuneKnightSpriteName().Text();
  jobSpriteNames[259] = LionRuneKnightBabySpriteName().Text();
  jobSpriteNames[253] = LionCrusaderSpriteName().Text();
  jobSpriteNames[260] = LionCrusaderBabySpriteName().Text();
  jobSpriteNames[254] = LionCrusaderHighSpriteName().Text();
  jobSpriteNames[261] = KagerouSpriteName().Text();
  jobSpriteNames[262] = OboroSpriteName().Text();
  jobSpriteNames[263] = FrogKagerouSpriteName().Text();
  jobSpriteNames[264] = FrogOboroSpriteName().Text();
  jobSpriteNames[265] = RebellionSpriteName().Text();
  jobSpriteNames[268] = DoramSummonerSpriteName().Text();
  jobSpriteNames[269] = DoramCartSummonerSpriteName().Text();
  jobSpriteNames[270] = DoramSummonerBabySpriteName().Text();
  jobSpriteNames[271] = DoramCartSummonerBabySpriteName().Text();
  jobSpriteNames[272] = NinjaBabySpriteName().Text();
  jobSpriteNames[280] = FrogNinjaBabySpriteName().Text();
  jobSpriteNames[273] = KagerouBabySpriteName().Text();
  jobSpriteNames[274] = OboroBabySpriteName().Text();
  jobSpriteNames[281] = FrogKagerouBabySpriteName().Text();
  jobSpriteNames[282] = FrogOboroBabySpriteName().Text();
  jobSpriteNames[275] = TaekwonBabySpriteName().Text();
  jobSpriteNames[283] = PoringTaekwonBabySpriteName().Text();
  jobSpriteNames[276] = StarGladiatorBabySpriteName().Text();
  jobSpriteNames[288] = StarGladiator2BabySpriteName().Text();
  jobSpriteNames[284] = PoringStarGladiatorBabySpriteName().Text();
  jobSpriteNames[277] = SoulLinkerBabySpriteName().Text();
  jobSpriteNames[285] = FrogSoulLinkerBabySpriteName().Text();
  jobSpriteNames[278] = []() -> EncryptedName<10, 0x0789EBC9F5C1C59FULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0x0789EBC9F5C1C59FULL);
    thread_local EncryptedName<sizeof(source), 0x0789EBC9F5C1C59FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[286] = []() -> EncryptedName<10, 0x451FD72783630709ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("건너\\건너", 0x451FD72783630709ULL);
    thread_local EncryptedName<sizeof(source), 0x451FD72783630709ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[279] = []() -> EncryptedName<20, 0x3B6D9BFD27A7C7F7ULL>& {
    constexpr std::array<char, 20> source = CRYPT_NAME("rebellion\\rebellion", 0x3B6D9BFD27A7C7F7ULL);
    thread_local EncryptedName<sizeof(source), 0x3B6D9BFD27A7C7F7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[287] = []() -> EncryptedName<20, 0xA15B555BE9CD2B81ULL>& {
    constexpr std::array<char, 20> source = CRYPT_NAME("rebellion\\rebellion", 0xA15B555BE9CD2B81ULL);
    thread_local EncryptedName<sizeof(source), 0xA15B555BE9CD2B81ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[289] = []() -> EncryptedName<10, 0x73C32959FDFFA51BULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0x73C32959FDFFA51BULL);
    thread_local EncryptedName<sizeof(source), 0x73C32959FDFFA51BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[290] = []() -> EncryptedName<18, 0xB3F1A10745C1F105ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울리퍼\\소울리퍼", 0xB3F1A10745C1F105ULL);
    thread_local EncryptedName<sizeof(source), 0xB3F1A10745C1F105ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[291] = []() -> EncryptedName<10, 0xD9115915B1919533ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0xD9115915B1919533ULL);
    thread_local EncryptedName<sizeof(source), 0xD9115915B1919533ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[292] = []() -> EncryptedName<18, 0x7959FFBF652D07BFULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울리퍼\\소울리퍼", 0x7959FFBF652D07BFULL);
    thread_local EncryptedName<sizeof(source), 0x7959FFBF652D07BFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[293] = []() -> EncryptedName<10, 0x99157F159BB99BD9ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0x99157F159BB99BD9ULL);
    thread_local EncryptedName<sizeof(source), 0x99157F159BB99BD9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[294] = []() -> EncryptedName<10, 0x31E3CF9D358D8705ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0x31E3CF9D358D8705ULL);
    thread_local EncryptedName<sizeof(source), 0x31E3CF9D358D8705ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[295] = []() -> EncryptedName<10, 0xA55B7BEF3D7377E3ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0xA55B7BEF3D7377E3ULL);
    thread_local EncryptedName<sizeof(source), 0xA55B7BEF3D7377E3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[296] = []() -> EncryptedName<18, 0xF5496753BDE7C5BDULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울리퍼\\소울리퍼", 0xF5496753BDE7C5BDULL);
    thread_local EncryptedName<sizeof(source), 0xF5496753BDE7C5BDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[297] = []() -> EncryptedName<10, 0x79A58B0F2173CF49ULL>& {
    constexpr std::array<char, 10> source = CRYPT_NAME("성제\\성제", 0x79A58B0F2173CF49ULL);
    thread_local EncryptedName<sizeof(source), 0x79A58B0F2173CF49ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[298] = []() -> EncryptedName<18, 0x6B3917C939150B9BULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("소울리퍼\\소울리퍼", 0x6B3917C939150B9BULL);
    thread_local EncryptedName<sizeof(source), 0x6B3917C939150B9BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[307] = []() -> EncryptedName<18, 0x795787B7779D19DBULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("WINDHAWK\\WINDHAWK", 0x795787B7779D19DBULL);
    thread_local EncryptedName<sizeof(source), 0x795787B7779D19DBULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[328] = []() -> EncryptedName<23, 0xF9B1EF9F7961EFF5ULL>& {
    constexpr std::array<char, 23> source = CRYPT_NAME("WINDHAWK\\WOLF_WINDHAWK", 0xF9B1EF9F7961EFF5ULL);
    thread_local EncryptedName<sizeof(source), 0xF9B1EF9F7961EFF5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[320] = []() -> EncryptedName<25, 0x29AB0B59651BDFD5ULL>& {
    constexpr std::array<char, 25> source = CRYPT_NAME("WINDHAWK\\WINDHAWK_RIDING", 0x29AB0B59651BDFD5ULL);
    thread_local EncryptedName<sizeof(source), 0x29AB0B59651BDFD5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[313] = []() -> EncryptedName<22, 0xD97DC927AFE911F1ULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("TROUBADOUR\\TROUBADOUR", 0xD97DC927AFE911F1ULL);
    thread_local EncryptedName<sizeof(source), 0xD97DC927AFE911F1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[326] = []() -> EncryptedName<29, 0x07A7D947CF7323BFULL>& {
    constexpr std::array<char, 29> source = CRYPT_NAME("TROUBADOUR\\TROUBADOUR_RIDING", 0x07A7D947CF7323BFULL);
    thread_local EncryptedName<sizeof(source), 0x07A7D947CF7323BFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[314] = []() -> EncryptedName<18, 0x47B7BF55EB471557ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("TROUVERE\\TROUVERE", 0x47B7BF55EB471557ULL);
    thread_local EncryptedName<sizeof(source), 0x47B7BF55EB471557ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[327] = []() -> EncryptedName<25, 0x8BA52B970DD531ADULL>& {
    constexpr std::array<char, 25> source = CRYPT_NAME("TROUVERE\\TROUVERE_RIDING", 0x8BA52B970DD531ADULL);
    thread_local EncryptedName<sizeof(source), 0x8BA52B970DD531ADULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[305] = []() -> EncryptedName<20, 0x2F2103792383718BULL>& {
    constexpr std::array<char, 20> source = CRYPT_NAME("ARCH_MAGE\\ARCH_MAGE", 0x2F2103792383718BULL);
    thread_local EncryptedName<sizeof(source), 0x2F2103792383718BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[318] = []() -> EncryptedName<27, 0xCFB9671DEF4BD9E7ULL>& {
    constexpr std::array<char, 27> source = CRYPT_NAME("ARCH_MAGE\\ARCH_MAGE_RIDING", 0xCFB9671DEF4BD9E7ULL);
    thread_local EncryptedName<sizeof(source), 0xCFB9671DEF4BD9E7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[311] = []() -> EncryptedName<32, 0x4349ADE96FEB27E5ULL>& {
    constexpr std::array<char, 32> source = CRYPT_NAME("ELEMETAL_MASTER\\ELEMETAL_MASTER", 0x4349ADE96FEB27E5ULL);
    thread_local EncryptedName<sizeof(source), 0x4349ADE96FEB27E5ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[324] = []() -> EncryptedName<39, 0x999F5191852D2F6BULL>& {
    constexpr std::array<char, 39> source = CRYPT_NAME("ELEMETAL_MASTER\\ELEMETAL_MASTER_RIDING", 0x999F5191852D2F6BULL);
    thread_local EncryptedName<sizeof(source), 0x999F5191852D2F6BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[306] = []() -> EncryptedName<18, 0x35F59F6F3DD1C965ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("CARDINAL\\CARDINAL", 0x35F59F6F3DD1C965ULL);
    thread_local EncryptedName<sizeof(source), 0x35F59F6F3DD1C965ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[319] = []() -> EncryptedName<25, 0xC1FD95811D1F4FA1ULL>& {
    constexpr std::array<char, 25> source = CRYPT_NAME("CARDINAL\\CARDINAL_RIDING", 0xC1FD95811D1F4FA1ULL);
    thread_local EncryptedName<sizeof(source), 0xC1FD95811D1F4FA1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[312] = []() -> EncryptedName<22, 0xBF0F412941076FADULL>& {
    constexpr std::array<char, 22> source = CRYPT_NAME("INQUISITOR\\INQUISITOR", 0xBF0F412941076FADULL);
    thread_local EncryptedName<sizeof(source), 0xBF0F412941076FADULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[325] = []() -> EncryptedName<29, 0xD7BBEB2DDB295D11ULL>& {
    constexpr std::array<char, 29> source = CRYPT_NAME("INQUISITOR\\INQUISITOR_RIDING", 0xD7BBEB2DDB295D11ULL);
    thread_local EncryptedName<sizeof(source), 0xD7BBEB2DDB295D11ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[309] = []() -> EncryptedName<12, 0xB59BADC5B365617DULL>& {
    constexpr std::array<char, 12> source = CRYPT_NAME("BIOLO\\BIOLO", 0xB59BADC5B365617DULL);
    thread_local EncryptedName<sizeof(source), 0xB59BADC5B365617DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[322] = []() -> EncryptedName<26, 0x8DFF0B4B67D9FDE1ULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("BIOLO_RIDING\\BIOLO_RIDING", 0x8DFF0B4B67D9FDE1ULL);
    thread_local EncryptedName<sizeof(source), 0x8DFF0B4B67D9FDE1ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[303] = []() -> EncryptedName<16, 0x91E7A7DFA58F9B6BULL>& {
    constexpr std::array<char, 16> source = CRYPT_NAME("MEISTER\\MEISTER", 0x91E7A7DFA58F9B6BULL);
    thread_local EncryptedName<sizeof(source), 0x91E7A7DFA58F9B6BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[316] = []() -> EncryptedName<30, 0x77D925073D6F19B9ULL>& {
    constexpr std::array<char, 30> source = CRYPT_NAME("MEISTER_RIDING\\MEISTER_RIDING", 0x77D925073D6F19B9ULL);
    thread_local EncryptedName<sizeof(source), 0x77D925073D6F19B9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[329] = []() -> EncryptedName<35, 0xAFF9332FD397533FULL>& {
    constexpr std::array<char, 35> source = CRYPT_NAME("MEISTER_MADOGEAR\\MEISTER_MADOGEAR1", 0xAFF9332FD397533FULL);
    thread_local EncryptedName<sizeof(source), 0xAFF9332FD397533FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[304] = []() -> EncryptedName<26, 0xA1DFCDAD555BB16DULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("SHADOW_CROSS\\SHADOW_CROSS", 0xA1DFCDAD555BB16DULL);
    thread_local EncryptedName<sizeof(source), 0xA1DFCDAD555BB16DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[317] = []() -> EncryptedName<33, 0x7DC10333F96DE3E7ULL>& {
    constexpr std::array<char, 33> source = CRYPT_NAME("SHADOW_CROSS\\SHADOW_CROSS_RIDING", 0x7DC10333F96DE3E7ULL);
    thread_local EncryptedName<sizeof(source), 0x7DC10333F96DE3E7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[310] = []() -> EncryptedName<26, 0x194F8339AF753FADULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("ABYSS_CHASER\\ABYSS_CHASER", 0x194F8339AF753FADULL);
    thread_local EncryptedName<sizeof(source), 0x194F8339AF753FADULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[323] = []() -> EncryptedName<33, 0x51372B6F4BA14B23ULL>& {
    constexpr std::array<char, 33> source = CRYPT_NAME("ABYSS_CHASER\\ABYSS_CHASER_RIDING", 0x51372B6F4BA14B23ULL);
    thread_local EncryptedName<sizeof(source), 0x51372B6F4BA14B23ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[302] = []() -> EncryptedName<28, 0xE9FBAB53D5A1BB17ULL>& {
    constexpr std::array<char, 28> source = CRYPT_NAME("DRAGON_KNIGHT\\DRAGON_KNIGHT", 0xE9FBAB53D5A1BB17ULL);
    thread_local EncryptedName<sizeof(source), 0xE9FBAB53D5A1BB17ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[315] = []() -> EncryptedName<42, 0x1F79132B518FD389ULL>& {
    constexpr std::array<char, 42> source = CRYPT_NAME("DRAGON_KNIGHT_RIDING\\DRAGON_KNIGHT_RIDING", 0x1F79132B518FD389ULL);
    thread_local EncryptedName<sizeof(source), 0x1F79132B518FD389ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[330] = []() -> EncryptedName<44, 0xB70319D3A78D39CDULL>& {
    constexpr std::array<char, 44> source = CRYPT_NAME("DRAGON_KNIGHT_CHICKEN\\DRAGON_KNIGHT_CHICKEN", 0xB70319D3A78D39CDULL);
    thread_local EncryptedName<sizeof(source), 0xB70319D3A78D39CDULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[308] = []() -> EncryptedName<30, 0xA5C5D1A3B9457B53ULL>& {
    constexpr std::array<char, 30> source = CRYPT_NAME("IMPERIAL_GUARD\\IMPERIAL_GUARD", 0xA5C5D1A3B9457B53ULL);
    thread_local EncryptedName<sizeof(source), 0xA5C5D1A3B9457B53ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[321] = []() -> EncryptedName<44, 0x8F61434F0F79DDD7ULL>& {
    constexpr std::array<char, 44> source = CRYPT_NAME("IMPERIAL_GUARD_RIDING\\IMPERIAL_GUARD_RIDING", 0x8F61434F0F79DDD7ULL);
    thread_local EncryptedName<sizeof(source), 0x8F61434F0F79DDD7ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[331] = []() -> EncryptedName<46, 0x4B670715D543C1C3ULL>& {
    constexpr std::array<char, 46> source = CRYPT_NAME("IMPERIAL_GUARD_CHICKEN\\IMPERIAL_GUARD_CHICKEN", 0x4B670715D543C1C3ULL);
    thread_local EncryptedName<sizeof(source), 0x4B670715D543C1C3ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[352] = []() -> EncryptedName<24, 0x61F59D5D41E76519ULL>& {
    constexpr std::array<char, 24> source = CRYPT_NAME("SKY_EMPEROR\\SKY_EMPEROR", 0x61F59D5D41E76519ULL);
    thread_local EncryptedName<sizeof(source), 0x61F59D5D41E76519ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[366] = []() -> EncryptedName<24, 0x4B1D078937DB892BULL>& {
    constexpr std::array<char, 24> source = CRYPT_NAME("SKY_EMPEROR\\SKY_EMPEROR", 0x4B1D078937DB892BULL);
    thread_local EncryptedName<sizeof(source), 0x4B1D078937DB892BULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[359] = []() -> EncryptedName<31, 0x7155E5EF0DFB0D03ULL>& {
    constexpr std::array<char, 31> source = CRYPT_NAME("SKY_EMPEROR\\SKY_EMPEROR_RIDING", 0x7155E5EF0DFB0D03ULL);
    thread_local EncryptedName<sizeof(source), 0x7155E5EF0DFB0D03ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[353] = []() -> EncryptedName<26, 0xD51953E763B94747ULL>& {
    constexpr std::array<char, 26> source = CRYPT_NAME("SOUL_ASCETIC\\SOUL_ASCETIC", 0xD51953E763B94747ULL);
    thread_local EncryptedName<sizeof(source), 0xD51953E763B94747ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[360] = []() -> EncryptedName<33, 0x17FB0F6BFF3BE93FULL>& {
    constexpr std::array<char, 33> source = CRYPT_NAME("SOUL_ASCETIC\\SOUL_ASCETIC_RIDING", 0x17FB0F6BFF3BE93FULL);
    thread_local EncryptedName<sizeof(source), 0x17FB0F6BFF3BE93FULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[354] = []() -> EncryptedName<18, 0x91DF1F5D3B014D45ULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("SHINKIRO\\SHINKIRO", 0x91DF1F5D3B014D45ULL);
    thread_local EncryptedName<sizeof(source), 0x91DF1F5D3B014D45ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[361] = []() -> EncryptedName<25, 0x2529F16B0375530DULL>& {
    constexpr std::array<char, 25> source = CRYPT_NAME("SHINKIRO\\SHINKIRO_RIDING", 0x2529F16B0375530DULL);
    thread_local EncryptedName<sizeof(source), 0x2529F16B0375530DULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[355] = []() -> EncryptedName<18, 0x2F371DDDADDD53CFULL>& {
    constexpr std::array<char, 18> source = CRYPT_NAME("SHIRANUI\\SHIRANUI", 0x2F371DDDADDD53CFULL);
    thread_local EncryptedName<sizeof(source), 0x2F371DDDADDD53CFULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[362] = []() -> EncryptedName<25, 0x0D7FA94DD32361A9ULL>& {
    constexpr std::array<char, 25> source = CRYPT_NAME("SHIRANUI\\SHIRANUI_RIDING", 0x0D7FA94DD32361A9ULL);
    thread_local EncryptedName<sizeof(source), 0x0D7FA94DD32361A9ULL> name(source);
    return name;
  }().Text();
  jobSpriteNames[356] = NightWatchBodyPath2().Text();
  jobSpriteNames[363] = NightWatchRidingBodyPath2().Text();
  jobSpriteNames[357] = HyperNoviceBodyPath2().Text();
  jobSpriteNames[364] = HyperNoviceRidingBodyPath2().Text();
  jobSpriteNames[358] = SpiritHandlerBodyPath2().Text();
  jobSpriteNames[365] = SpiritHandlerRidingBodyPath2().Text();
  jobSpriteNames[401] = DruidBodyPath3().Text();
  jobSpriteNames[402] = DruidBodyPath4().Text();
  jobSpriteNames[403] = KarnosBodyPath3().Text();
  jobSpriteNames[404] = KarnosBodyPath4().Text();
  jobSpriteNames[405] = AliteaBodyPath2().Text();
}
