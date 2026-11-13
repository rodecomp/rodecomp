#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/World/World.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/Tokenizer.h"
#include "Contents/Messenger/ClientMessenger.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>

template __declspec(noinline) std::_Tree_find_result<std::map<std::int64_t, std::int64_t>::_Mybase::_Nodeptr> std::map<std::int64_t, std::int64_t>::_Mybase::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
template __declspec(noinline) std::pair<std::map<int, SessionRecord6144>::_Mybase::_Nodeptr, bool> std::map<int, SessionRecord6144>::_Mybase::_Emplace<std::pair<const int, SessionRecord6144>>(std::pair<const int, SessionRecord6144>&&);

// GLOBAL: gl_24650
char g_decodedPlayerName[64];

// OFFSET: 0x00E198F0
// FUNCTION: fn_23844
void InitMapTypeInfoTable() {
  CFile file;
  if (!file.Open("MapInfoTable.scp", 0, true, false)) {
    if (g_serviceType == 0) {
      ShowErrorDialog("Load MapInfoTable Failed!", 0);
    } else {
      g_mapPropertyMgr->Clear();
    }
  } else {
    const int size = file.GetSize();
    char* buffer = new char[size];
    file.Read(buffer, size);

    int index = 0;
    int mapId = 0;
    int propertyId = 0;

    do {
      char character = buffer[index++];

      while (index < size) {
        if (character == '/') {
          if (buffer[index] == '/') {
            do {
              character = buffer[index++];
            } while (character != '\n');
          } else if (buffer[index] == '*') {
            do {
              do {
                character = buffer[index++];
              } while (character != '*');
            } while (buffer[index] != '/');
          }
        } else if (character != ' ' && character != '\t') {
          if (character == '\r') {
            ++index;
          } else {
            char valueText[16] = {};
            int textIndex = 0;

            while (character != ' ' && character != '\t') {
              valueText[textIndex++] = character;
              character = buffer[index++];
              if (character == '\r') {
                index += 1;
                break;
              }
            }

            g_mapPropertyMgr->SetValue(mapId, propertyId, std::atoi(valueText));
            ++propertyId;
            if (propertyId >= 27) {
              ++mapId;
              propertyId = 0;
              break;
            }
          }
        }

        character = buffer[index++];
      }
    } while (mapId < 40 && index < size);

    delete[] buffer;
    file.Close();
  }
}

// OFFSET: 0x00E1DF20
// FUNCTION: fn_23886
int CSession::HasSkillEffect(std::uint32_t id) const {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == id) {
      return 1;
    }
  }

  return 0;
}

// OFFSET: 0x00E096F0
// FUNCTION: fn_23782
CSkillInfo CSession::LookupHomunSkill(std::uint32_t id) const {
  if (this->m_homunSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  for (auto it = this->m_homunSkills.begin(); it != this->m_homunSkills.end(); ++it) {
    if (it->id == id) {
      return *it;
    }
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E09A10
// FUNCTION: fn_23787
CSkillInfo CSession::LookupMercSkill(std::uint32_t id) const {
  if (this->m_mercSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  for (auto it = this->m_mercSkills.begin(); it != this->m_mercSkills.end(); ++it) {
    if (it->id == id) {
      return *it;
    }
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E095B0
// FUNCTION: fn_23780
CSkillInfo CSession::LookupAdditionalSkill(std::uint32_t id) const {
  if (this->m_additionalSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  for (auto it = this->m_additionalSkills.begin(); it != this->m_additionalSkills.end(); ++it) {
    if (it->id == id) {
      return *it;
    }
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E088A0
// FUNCTION: fn_23767
CSkillInfo CSession::LookupSkill(std::uint32_t id) {
  CSkillInfo skill(0);
  if (id - 8000u <= 60) {
    skill = g_session.LookupHomunSkill(id);
  } else if (id - 8200u <= 41 || id - 8400u <= 57) {
    skill = g_session.LookupMercSkill(id);
  } else if (id - 10000u <= 19) {
    skill = g_session.LookupAdditionalSkill(id);
  } else {
    skill = g_session.m_skillBundle.Lookup(id);
  }

  return skill;
}

// OFFSET: 0x00E25240
// FUNCTION: fn_23904
void CSession::NotifySkillWindows() {
  if (g_windowMgr.skillListWnd) {
    g_windowMgr.skillListWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (g_windowMgr.shortCutWnd) {
    g_windowMgr.shortCutWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_QUIKSLOTWND)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E08530
// FUNCTION: fn_23762
void CSession::RemoveSkill(std::uint32_t id) {
  if (CSkillInfo* skill = this->m_skillBundle.FindGrouped(id)) {
    const std::pair<bool, int> previous = this->m_skillBundle.PreviousLevel(static_cast<int>(id));
    if (previous.first == true) {
      skill->level = previous.second;
    }

    this->NotifySkillWindows();
    return;
  }

  this->m_skillBundle.Remove(id);
  if (this->m_skillBundle.RemoveExtra(id) == true) {
    this->NotifySkillWindows();
  }
}

// OFFSET: 0x00E07540
// FUNCTION: fn_23746
bool CSession::UpdateSkill(const CSkillInfo& value, unsigned char notify, unsigned char updateType) {
  {
    CSkillInfo snapshot(value);
    const auto update = [](CSkillInfo& target, const CSkillInfo& source, bool copyType, bool copyLearnedLevel) {
      target.level = source.level;
      std::memcpy(&target.spCost, &source.spCost, sizeof(target.spCost));
      std::memcpy(&target.range, &source.range, sizeof(target.range));
      std::memcpy(&target.upgradable, &source.upgradable, sizeof(target.upgradable));

      if (copyType) {
        std::memcpy(&target.type, &source.type, sizeof(target.type));
      }

      if (copyLearnedLevel) {
        target.learnedLevel = source.learnedLevel;
      }

      target.apCost = source.apCost;
    };

    CSkillInfo* grouped = this->m_skillBundle.FindGrouped(snapshot.id);
    if (grouped) {
      update(*grouped, snapshot, true, true);
    } else if (snapshot.available) {
      if (CSkillInfo* extra = this->m_skillBundle.FindExtra(snapshot.id)) {
        update(*extra, snapshot, true, false);
      } else {
        snapshot.treePosition = UINT32_MAX;
        snapshot.canPlanLevel = 0;
        this->m_skillBundle.AddExtra(snapshot);
      }
    }

    if (grouped || snapshot.available) {
      if (CSkillInfo::UsesLevel(*this->m_hotkeyScript, static_cast<int>(snapshot.id)) && snapshot.level != 0) {
        const int saved = this->UseLevel(static_cast<int>(snapshot.id));
        if (saved <= 0 || saved > snapshot.level) {
          this->m_skillBundle.SetSkillValue(static_cast<int>(snapshot.id), snapshot.level);
        }
      }
    }
  }

  if (CSkillInfo* existing = this->m_skillBundle.Find(value.id)) {
    existing->level = value.level;
    std::memcpy(&existing->spCost, &value.spCost, sizeof(existing->spCost));
    std::memcpy(&existing->range, &value.range, sizeof(existing->range));
    std::memcpy(&existing->upgradable, &value.upgradable, sizeof(existing->upgradable));

    if (updateType == 1) {
      std::memcpy(&existing->type, &value.type, sizeof(existing->type));
    }

    existing->learnedLevel = value.learnedLevel;
    existing->apCost = value.apCost;
    if (notify == 1) {
      this->NotifySkillWindows();
    }

    return false;
  }

  this->m_skillBundle.Add(value);
  if (notify != 0) {
    this->NotifySkillWindows();
  }

  return true;
}

// OFFSET: 0x00E07290
// FUNCTION: fn_23740
bool CSession::UpdateAdditionalSkill(const CSkillInfo& value, int) {
  for (CSkillInfo& skill : this->m_additionalSkills) {
    if (value.id == skill.id) {
      skill.level = value.level;
      std::memcpy(&skill.spCost, &value.spCost, sizeof(skill.spCost));
      std::memcpy(&skill.range, &value.range, sizeof(skill.range));
      std::memcpy(&skill.upgradable, &value.upgradable, sizeof(skill.upgradable));
      skill.apCost = value.apCost;

      return false;
    }
  }

  this->m_additionalSkills.push_back(value);
  return true;
}

// OFFSET: 0x00E072F0
// FUNCTION: fn_23741
bool CSession::UpdateHomunSkill(const CSkillInfo& value, unsigned char notify) {
  for (CSkillInfo& skill : this->m_homunSkills) {
    if (value.id == skill.id) {
      skill.level = value.level;
      skill.spCost = value.spCost;
      skill.range = value.range;
      skill.upgradable = value.upgradable;
      skill.type = value.type;
      skill.apCost = value.apCost;
      if (notify) {
        this->NotifyHomunSkillWindows();
      }

      return false;
    }
  }

  this->m_homunSkills.push_back(value);
  if (notify) {
    this->NotifyHomunSkillWindows();
  }

  return true;
}

// OFFSET: 0x00E07380
// FUNCTION: fn_23742
bool CSession::UpdateMercSkill(const CSkillInfo& value, unsigned char notify) {
  for (CSkillInfo& skill : this->m_mercSkills) {
    if (value.id == skill.id) {
      skill.level = value.level;
      skill.spCost = value.spCost;
      skill.range = value.range;
      skill.upgradable = value.upgradable;
      skill.type = value.type;
      skill.apCost = value.apCost;
      if (notify) {
        this->NotifyMercSkillWindows();
      }

      return false;
    }
  }

  this->m_mercSkills.push_back(value);
  if (notify) {
    this->NotifyMercSkillWindows();
  }

  return true;
}

// OFFSET: 0x00E07410
// FUNCTION: fn_23743
void CSession::AddAchievementId(int id) {
  if (std::find(this->achievementIds.begin(), this->achievementIds.end(), id) == this->achievementIds.end()) {
    this->achievementIds.push_back(id);
  }
}

// OFFSET: 0x00E07470
// FUNCTION: fn_23744
bool CSession::EquipAlternateCostumeItem(const ITEM_INFO& item) {
  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->alternateCostumeItems[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.expireTime = item.expireTime;
      equipped.bindOnEquip = item.bindOnEquip;
      equipped.sprite = item.sprite;
    }
  }

  return true;
}

// OFFSET: 0x00E07930
// FUNCTION: fn_23747
void CSession::AddStackedItem(ITEM_INFO item) {
  int index;
  for (index = 0; index < 5; ++index) {
    if (this->selectionItems[index].quantity != 0 && this->selectionItems[index].identity[1] == item.identity[1]) {
      break;
    }
  }

  if (index < 5) {
    this->selectionItems[index].quantity += item.quantity;
  } else {
    for (index = 0; index < 5; ++index) {
      if (this->selectionItems[index].quantity == 0) {
        this->selectionItems[index] = item;
        break;
      }
    }
  }

  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.rodexComposeWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E07A10
// FUNCTION: fn_23748
void CSession::AddItemToFreeSlot(ITEM_INFO item) {
  for (int index = 0; index < 5; ++index) {
    if (this->selectionItems[index].quantity == 0) {
      this->selectionItems[index] = item;
      break;
    }
  }
}

// OFFSET: 0x00E081E0
// FUNCTION: fn_23752
bool CSession::IsListedId(int id, int variant) const {
  if (variant == 0) {
    if (id == 19 || id == 4020 || id == 4042 || id == 4068 || id == 4075 || id == 4104 || id == 4263) {
      return true;
    }
  } else if (id == 20 || id == 4021 || id == 4043 || id == 4069 || id == 4076 || id == 4105 || id == 4264) {
    return true;
  }

  return false;
}

// OFFSET: 0x00E08290
// FUNCTION: fn_23754
void CSession::ClearItemQuantities() {
  for (int index = 0; index < 5; ++index) {
    this->selectionItems[index].quantity = 0;
  }

  this->achievementResult = 0;
}

// OFFSET: 0x00E082D0
// FUNCTION: fn_23755
void CSession::ResetAlternateCostumeItems() {
  for (int index = 0; index < 10; ++index) {
    this->alternateCostumeItems[index].Reset();
  }
}

// OFFSET: 0x00E08300
// FUNCTION: fn_23756
void CSession::ClearQuickSlots() {
  this->quickSlot.enabled = 0;
  this->quickSlot.itemId = 0;
  this->quickSlot.value = 0;
  this->alternateQuickSlot.enabled = 0;
  this->alternateQuickSlot.itemId = 0;
  this->alternateQuickSlot.value = 0;
}

// OFFSET: 0x00E08330
// FUNCTION: fn_23757
void CSession::ClearShortcutSlots() {
  for (int index = 0; index < 36; ++index) {
    this->m_shortcutSlots[index].enabled = 0;
    this->m_shortcutSlots[index].itemId = 0;
    this->m_shortcutSlots[index].value = 0;
  }
}

// OFFSET: 0x00E08360
// FUNCTION: fn_23758
void CSession::ClearSecondaryShortcutSlots() {
  std::memset(this->m_secondaryShortcutSlots, 0, sizeof(this->m_secondaryShortcutSlots));
}

// OFFSET: 0x00E08380
// FUNCTION: fn_23759
void CSession::ClearHotkeySkillLevels() {
  for (int index = 0; index < 36; ++index) {
    this->hotkeySkillLevels[index] = 0;
  }
}

// OFFSET: 0x00E083D0
// FUNCTION: fn_23760
void CSession::EraseSummonHp(std::int64_t key) {
  auto first = this->summonHp.find(key);
  if (this->summonHp.end() != first) {
    this->summonHp.erase(first);
  }

  auto second = this->summonMaxHp.find(key);
  if (this->summonMaxHp.end() != second) {
    this->summonMaxHp.erase(second);
  }
}

// OFFSET: 0x00E08D50
// FUNCTION: fn_23772
int CSession::CountProhibitionLog(std::uint32_t aid) {
  int matches = 0;
  const int count = static_cast<int>(this->m_prohibitionLog.size());
  const CRenderObject* actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid);
  if (!actor) {
    return -1;
  }

  const std::uint32_t accountId = actor->render.accountId;
  this->m_prohibitionTarget = accountId;
  for (int index = 0; index < count; ++index) {
    const ProhibitionLogEntry& entry = this->m_prohibitionLog[index];
    if (std::strcmp(entry.ownerName, this->m_serverName.c_str()) == 0 && entry.accountId == accountId) {
      ++matches;
    }
  }

  return matches;
}

// OFFSET: 0x00E085A0
// FUNCTION: fn_23763
void CSession::DeleteProhibitionLog(std::uint32_t aid, std::uint32_t minutes) {
  const char* message;
  auto found = this->m_prohibitionLog.end();
  for (auto entry = this->m_prohibitionLog.begin(); entry != this->m_prohibitionLog.end(); ++entry) {
    if (entry->aid == aid) {
      found = entry;
    }
  }

  if (found == this->m_prohibitionLog.end()) {
    message = "채팅 로그 삭제 실패 : 채팅 금지를 준 기록이 없습니다.";
  } else if (found->minutes > minutes) {
    message = "채팅 로그 삭제 안함 : 마이너스 된 시간이 기존의 채팅 금지 시간보다 짧습니다.";
  } else {
    this->m_prohibitionLog.erase(found);
    message = "채팅 로그 삭제 성공";
  }

  g_windowMgr.Confirm(message, 0, true, false, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00E08690
// FUNCTION: fn_23764
std::int64_t CSession::FindSummonHp(std::int64_t key) const {
  auto found = this->summonHp.find(key);
  if (this->summonHp.end() != found) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00E086F0
// FUNCTION: fn_23765
std::int64_t CSession::FindSummonMaxHp(std::int64_t key) const {
  auto found = this->summonMaxHp.find(key);
  if (this->summonMaxHp.end() != found) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00E08750
// FUNCTION: fn_23766
SessionRecord6144 CSession::FindAchievementRecord(int key) const {
  auto found = this->achievements.find(key);
  if (found != this->achievements.end()) {
    return found->second;
  }

  return {};
}

// OFFSET: 0x00E06FD0
// FUNCTION: fn_23738
void CSession::UpdateAchievementRecord(const AchievementRecordPacket& packet) {
  auto found = this->achievements.find(packet.id);
  if (found != this->achievements.end()) {
    SessionRecord6144& record = found->second;
    reinterpret_cast<unsigned char*>(&record.completionFlags)[1] = packet.completed;
    std::memcpy(record.objectives, packet.objectives, sizeof(record.objectives));
    record.completedAt = packet.completedAt;
    record.rewarded = packet.rewarded;
    record.SplitDate(packet.completedAt, reinterpret_cast<std::uint16_t&>(record.year), reinterpret_cast<std::uint16_t&>(record.month), reinterpret_cast<std::uint16_t&>(record.day));
    return;
  }

  SessionRecord6144 record;
  reinterpret_cast<unsigned char*>(&record.completionFlags)[0] = 1;
  reinterpret_cast<unsigned char*>(&record.completionFlags)[1] = packet.completed;
  std::memcpy(record.objectives, packet.objectives, sizeof(record.objectives));
  record.completedAt = packet.completedAt;
  record.rewarded = packet.rewarded;
  record.SplitDate(packet.completedAt, reinterpret_cast<std::uint16_t&>(record.year), reinterpret_cast<std::uint16_t&>(record.month), reinterpret_cast<std::uint16_t&>(record.day));
  this->achievements.insert({packet.id, record});
}

// OFFSET: 0x00E08A00
// FUNCTION: fn_23769
int CSession::GetBeginEffectId(int skillId) const {
  int effectId = -1;
  bool hasEffect = false;
  if (g_session.m_hotkeyScript->Call("HaveSkillEffectInfo", "d>b", skillId, &hasEffect) && hasEffect) {
    g_session.m_hotkeyScript->Call("GetBeginEffectID", "d>d", skillId, &effectId);
  }

  return effectId;
}

// OFFSET: 0x00E08C70
// FUNCTION: fn_23771
const char* CSession::DecodePlayerName() const {
  std::memcpy(g_decodedPlayerName, this->m_playerNameKey, sizeof(g_decodedPlayerName));
  for (unsigned int index = 0; index < sizeof(g_decodedPlayerName); ++index) {
    g_decodedPlayerName[index] ^= g_playerNameKey[index];
  }

  return g_decodedPlayerName;
}

// OFFSET: 0x00E08EE0
// FUNCTION: fn_23773
ITEM_INFO CSession::GetCostumeItemAt(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (index >= 0 && index < 10) {
    return this->costumeItems[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E08F70
// FUNCTION: fn_23774
int CSession::CountSelectionItems() const {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (this->selectionItems[index].quantity != 0) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00E09080
// FUNCTION: fn_23775
__declspec(noinline) void SessionRecord6144::SplitDate(__time32_t time, std::uint16_t& year, std::uint16_t& month, std::uint16_t& day) const {
  time = time > 0 ? time : 0;

  const std::tm local = *_localtime32(&time);
  year = static_cast<std::uint16_t>(local.tm_year + 1900);
  month = static_cast<std::uint16_t>(local.tm_mon + 1);
  day = static_cast<std::uint16_t>(local.tm_mday);
}

// OFFSET: 0x00E091F0
// FUNCTION: fn_23776
int CSession::GetDefaultOptionValue(const char* name, int fallback) const {
  int value = -1;
  this->m_hotkeyScript->Call("l_GetDefaultOptionValue", "s>d", name, &value);
  return value == -1 ? fallback : value;
}

// OFFSET: 0x00E094B0
// FUNCTION: fn_23777
int CSession::FindActorSkillAnimationValue(int key) const {
  for (const std::pair<int, int>& entry : this->keyValues) {
    if (key == entry.first) {
      return entry.second;
    }
  }

  return 0;
}

// OFFSET: 0x00E09510
// FUNCTION: fn_23779
CSkillInfo CSession::AdditionalSkillAt(int index) const {
  if (this->m_additionalSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  int position = 0;
  for (auto it = this->m_additionalSkills.begin(); it != this->m_additionalSkills.end(); ++it) {
    if (position == index) {
      return *it;
    }

    ++position;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E09650
// FUNCTION: fn_23781
CSkillInfo CSession::HomunSkillAt(int index) const {
  if (this->m_homunSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  int position = 0;
  for (auto it = this->m_homunSkills.begin(); it != this->m_homunSkills.end(); ++it) {
    if (position == index) {
      return *it;
    }

    ++position;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E251C0
// FUNCTION: fn_23902
__declspec(noinline) void CSession::NotifyHomunSkillWindows() {
  if (g_windowMgr.homunculusSkillWnd) {
    g_windowMgr.homunculusSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (g_windowMgr.shortCutWnd) {
    g_windowMgr.shortCutWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E25200
// FUNCTION: fn_23903
__declspec(noinline) void CSession::NotifyMercSkillWindows() {
  if (g_windowMgr.mercenarySkillWnd) {
    g_windowMgr.mercenarySkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (g_windowMgr.shortCutWnd) {
    g_windowMgr.shortCutWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E08260
// FUNCTION: fn_23753
void CSession::ResetCostumeItems() {
  for (int index = 0; index < 10; ++index) {
    this->costumeItems[index].Reset();
  }

  RequestBasicInfoRefresh();
}

// OFFSET: 0x00E08B40
// FUNCTION: fn_23770
float GetSkillFactor(int job) {
  if ((job < JT_NOVICE_B || job > JT_THIEF_B) && job != JT_SUPERNOVICE_B && job != JT_SUPERNOVICE2_B &&
      static_cast<unsigned>(job - JT_KNIGHT_B) > JT_CHICKEN2_B - JT_KNIGHT_B && static_cast<unsigned>(job - JT_RUNE_KNIGHT_B) > JT_MADOGEAR_B - JT_RUNE_KNIGHT_B &&
      static_cast<unsigned>(job - JT_PORING_NOVICE_B) > JT_DOG_CHASER_B - JT_PORING_NOVICE_B && (job < JT_SHEEP_PRIEST_B || job > JT_PIG_BLACKSMITH_B) &&
      job != JT_PORING_SNOVICE2_B && static_cast<unsigned>(job - JT_PIG_MECHANIC_B) > JT_LION_CRUSADER_B - JT_PIG_MECHANIC_B && job != JT_DO_SUMMONER_B &&
      static_cast<unsigned>(job - JT_NINJA_B) > JT_STAR2_B - JT_NINJA_B && job != JT_STAR_EMPEROR_B && job != JT_SOUL_REAPER_B && job != JT_STAR2_EMPEROR_B &&
      job != JT_HAETAE_STAR_EMPEROR_B && job != JT_HAETAE_SOUL_REAPER_B && job != JT_DRUID_B && job != JT_KARNOS_B) {
    return 1.0f;
  }

  if (g_session.IsDoramJob(job) == true) {
    return 0.8f;
  }

  if ((job >= JT_NOVICE_B && job <= JT_THIEF_B) || job == JT_SUPERNOVICE_B || job == JT_SUPERNOVICE2_B) {
    return 0.75f;
  }

  return 0.82f;
}

// OFFSET: 0x00E084B0
// FUNCTION: fn_23761
void CSession::ClearAdditionalShortcutSkills() {
  for (int slot = 0; slot < 36; ++slot) {
    const NormalItemSlot& value = this->m_shortcutSlots[slot];
    if (value.itemId != 0 && value.enabled != 0 && value.itemId >= 10000 && value.itemId < 10020) {
      this->AssignShortcut(1, 0, 0, slot, 0);
    }
  }

  for (int slot = 0; slot < 36; ++slot) {
    const NormalItemSlot& value = this->m_secondaryShortcutSlots[slot];
    if (value.itemId != 0 && value.enabled != 0 && value.itemId >= 10000 && value.itemId < 10020) {
      this->AssignShortcut(1, 0, 0, slot, 1);
    }
  }
}

// OFFSET: 0x00E089A0
// FUNCTION: fn_23768
int CSession::SkillLevel(int id) const {
  if (id - 8000u <= 60u) {
    return g_session.HomunSkillLevel(id);
  }

  if (id - 8200u <= 41u || id - 8400u <= 57u) {
    return g_session.MercSkillLevel(id);
  }

  return g_session.UseLevel(id);
}

// OFFSET: 0x00E094E0
// FUNCTION: fn_23778
CRenderObject* CSession::FindGameActor(std::uint32_t aid) const {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode == nullptr) {
    return nullptr;
  }

  return mode->scene->GetGameActorByAID(aid);
}

// OFFSET: 0x00E07AA0
// FUNCTION: fn_23749
void CSession::AddProhibitionLog(std::uint32_t aid, std::uint32_t minutes) {
  const CRenderObject* actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid);
  if (!actor) {
    return;
  }

  std::uint32_t accountId = actor->render.accountId;
  const std::string actorName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(aid);

  ProhibitionLogEntry entry;
  std::strcpy(entry.name, actorName.c_str());
  std::strcpy(entry.ownerName, this->m_serverName.c_str());
  entry.minutes = minutes;
  entry.aid = aid;
  entry.accountId = accountId;

  SYSTEMTIME localTime;
  GetLocalTime(&localTime);
  entry.date = ((localTime.wYear << 8 | localTime.wMonth) << 8) | localTime.wDay;
  entry.time = ((localTime.wHour << 8 | localTime.wMinute) << 8) | localTime.wSecond;
  this->m_prohibitionLog.push_back(entry);

  CFile file;
  char path[256];
  GetCurrentDirectoryA(sizeof(path), path);
  char* data = std::strstr(path, "\\data");
  int count = 0;
  int size = 0;
  if (data) {
    std::memcpy(path, path, data - path);
  }
  std::strcat(path, "\\ProhibitionLog.gd");

  char* buffer;
  if (file.Open(path, 0, true, false)) {
    size = file.GetSize() - 4;
    buffer = new char[size];
    file.Read(&count, 4);
    file.Read(buffer, size);
    file.Close();
  }

  ++count;
  file.Open(path, 0x1001, true, false);
  file.Write(&count, 4);
  if (size) {
    file.Write(buffer, size);
  }

  int length = static_cast<int>(std::strlen(entry.name)) + 1;
  file.Write(&length, 4);
  file.Write(entry.name, length);
  length = static_cast<int>(std::strlen(entry.ownerName)) + 1;
  file.Write(&length, 4);
  file.Write(entry.ownerName, length);
  file.Write(&aid, 4);
  file.Write(&accountId, 4);
  file.Write(&entry.minutes, 4);
  file.Write(&entry.date, 4);
  file.Write(&entry.time, 4);
  file.Close();
  if (size) {
    delete[] buffer;
  }
}

// OFFSET: 0x00E07FB0
// FUNCTION: fn_23750
int CSession::ProcessEmotionShortcut(const char* text) {
  if (!text) {
    return 0;
  }

  CTokenizer tokenizer;
  tokenizer.Parse(std::string(text));
  if (tokenizer.tokens.size() == 0) {
    return 0;
  }

  LowercaseText(tokenizer[0]);
  CCashEmotionMgr* emotions = g_cashEmotionMgr;
  return emotions->PlaySearchedEmotion(tokenizer[0]);
}

// OFFSET: 0x00E08190
// FUNCTION: fn_23751
void CSession::RestoreSelectionItems() {
  bool restored = false;
  for (int index = 0; index < 5; ++index) {
    if (this->selectionItems[index].quantity != 0) {
      restored = true;
      g_session.AddInventoryItem(this->selectionItems[index], 0);
    }
  }

  if (restored) {
    RefreshItemWindows(&g_windowMgr);
  }
}

// OFFSET: 0x00E252B0
// FUNCTION: fn_23905
void CSession::ReturnStackedItem(int index, int count) {
  for (int slot = 0; slot < 5; ++slot) {
    const int quantity = this->selectionItems[slot].quantity;
    if (quantity != 0 && this->selectionItems[slot].identity[1] == index) {
      this->selectionItems[slot].quantity = count;
      g_session.AddInventoryItem(this->selectionItems[slot], 0);
      this->selectionItems[slot].quantity = quantity - count;
      RefreshItemWindows(&g_windowMgr);
      break;
    }
  }

  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.rodexComposeWnd)) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

namespace EquipmentProperties {
class CEquipmentPropertiesMgr {
public:
  virtual ~CEquipmentPropertiesMgr();
  unsigned char padding_0004[0x1F8];

  CEquipmentPropertiesMgr();
  __declspec(noinline) void EquipWearable(const ITEM_INFO* item, bool notify);
};
}

// OFFSET: 0x00E07150
// FUNCTION: fn_23739
bool CSession::EquipCostumeItem(const ITEM_INFO& item, const char* name) {
  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  const int order[3] = {slots.values[2], slots.values[1], slots.values[0]};

  for (int index = 0; index < 3; ++index) {
    const int slot = order[index];
    if (slot != -1) {
      ITEM_INFO& equipped = this->costumeItems[slot];
      equipped = item;
      equipped.quantity = 1;
      equipped.expireTime = item.expireTime;
      equipped.bindOnEquip = item.bindOnEquip;
      equipped.sprite = item.sprite;
    }
  }

  RequestBasicInfoRefresh();
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->EquipWearable(&item, *name == '\0');
  this->InvalidateWindowFrames();
  return true;
}

// OFFSET: 0x00E09900
// FUNCTION: fn_23785
ITEM_INFO CSession::GetSelectionItemAt(int index) const {
  return this->selectionItems[index];
}

// OFFSET: 0x00E09970
// FUNCTION: fn_23786
CSkillInfo CSession::MercSkillAt(int index) const {
  if (this->m_mercSkills.size() == 0) {
    return CSkillInfo(-1);
  }

  int position = 0;
  for (auto it = this->m_mercSkills.begin(); it != this->m_mercSkills.end(); ++it) {
    if (position == index) {
      return *it;
    }

    ++position;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E0AAD0
// FUNCTION: fn_23802
ITEM_INFO CSession::GetAlternateCostumeItemAt(int index) const {
  if (index == 15) {
    return ITEM_INFO(0);
  }

  if (index >= 0 && index < 10) {
    return this->alternateCostumeItems[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E13000
// FUNCTION: fn_23822
void CSession::FindLatestCompletedAchievements(int* second, int* first) {
  int bestValue = -1;
  int secondValue;
  for (auto it = this->achievements.begin(); it != this->achievements.end(); ++it) {
    if (reinterpret_cast<const unsigned char*>(&it->second.completionFlags)[1]) {
      const int value = it->second.completedAt;
      if (bestValue < value) {
        secondValue = bestValue;
        bestValue = value;
        *second = *first;
        *first = it->first;
      } else if (secondValue < value) {
        secondValue = value;
        *second = it->first;
      }
    }
  }
}

// OFFSET: 0x00E19580
// FUNCTION: fn_23842
void CSession::LoadProhibitionLog() {
  CFile file;
  char path[256];
  GetCurrentDirectoryA(sizeof(path), path);
  char* data = std::strstr(path, "\\data");
  if (data) {
    std::memcpy(path, path, data - path);
  }
  std::strcat(path, "\\ProhibitionLog.gd");

  if (file.Open(path, 0, true, false)) {
    int count = 0;
    file.Read(&count, 4);
    for (int index = 0; index < count; ++index) {
      int length;
      ProhibitionLogEntry entry;
      file.Read(&length, 4);
      file.Read(entry.name, length);
      file.Read(&length, 4);
      file.Read(entry.ownerName, length);
      file.Read(&entry.aid, 4);
      file.Read(&entry.accountId, 4);
      file.Read(&entry.minutes, 4);
      file.Read(&entry.date, 4);
      file.Read(&entry.time, 4);
      this->m_prohibitionLog.push_back(entry);
    }
    file.Close();
  }
}

// OFFSET: 0x00E19B50
// FUNCTION: fn_23845
void CSession::ClearPairValues(unsigned char clearThird, unsigned char clearSecond, unsigned char clearFirst) {
  if (clearFirst == 1) {
    this->m_pairValues[0] = -1;
  }
  if (clearSecond == 1) {
    this->m_pairValues[1] = -1;
  }
  if (clearThird == 1) {
    this->m_pairValues[2] = -1;
  }
}

// OFFSET: 0x00E19B90
// FUNCTION: fn_23846
void CSession::ResetPetEggAndJob() {
  g_session.petEggIndex = -1;
  g_session.petJob = -1;
}
