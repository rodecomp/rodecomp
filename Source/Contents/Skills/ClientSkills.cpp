#include "Contents/Skills/ClientSkills.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/ClientState.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "Contents/World/WorldScene.h"
#include "Script/LuaScript.h"
#include "UI/Shortcuts/ShortcutBindings.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstring>

// GLOBAL: gl_24650
char g_decodedPlayerName[64];

namespace {
  // INLINED
  CSkillInfo Find(const std::list<CSkillInfo>& values, std::uint32_t id) {
    for (const CSkillInfo& value : values) {
      if (value.id == id) {
        return value;
      }
    }

    return CSkillInfo(-1);
  }
}

// OFFSET: 0x00E1DF20
// FUNCTION: fn_23886
bool ClientSkills::HasEffect(std::uint32_t id) const {
  for (const ActiveSkillEffect& effect : this->effects) {
    if (effect.id == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00E096F0
// FUNCTION: fn_23782
CSkillInfo ClientSkills::LookupFirst(std::uint32_t id) const {
  return Find(this->firstRange, id);
}

// OFFSET: 0x00E09A10
// FUNCTION: fn_23787
CSkillInfo ClientSkills::LookupOther(std::uint32_t id) const {
  return Find(this->otherRanges, id);
}

// OFFSET: 0x00E095B0
// FUNCTION: fn_23780
CSkillInfo ClientSkills::LookupAdditional(std::uint32_t id) const {
  return Find(this->additional, id);
}

// OFFSET: 0x00E088A0
// FUNCTION: fn_23767
CSkillInfo ClientSkills::Lookup(std::uint32_t id) const {
  if (id - 8000u <= 60) {
    return this->LookupFirst(id);
  }

  if (id - 8200u <= 41 || id - 8400u <= 57) {
    return this->LookupOther(id);
  }

  if (id - 10000u <= 19) {
    return this->LookupAdditional(id);
  }

  return this->bundle.Lookup(id, this->classMode, this->HasEffect(1717), this->HasEffect(1716));
}

// OFFSET: 0x00E25240
// FUNCTION: fn_23904
void SkillWindows::Notify() {
  if (this->first) {
    this->first->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (this->second) {
    this->second->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (UIWindow* window = this->windows.LookupWindow(159)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E08530
// FUNCTION: fn_23762
void ClientSkills::Remove(std::uint32_t id, SkillWindows& windows) {
  if (CSkillInfo* skill = this->bundle.FindGrouped(id)) {
    int previous;
    if (this->bundle.PreviousLevel(static_cast<int>(id), previous)) {
      skill->level = previous;
    }

    windows.Notify();
    return;
  }

  this->bundle.Remove(id);
  if (this->bundle.RemoveExtra(id, windows.windows)) {
    windows.Notify();
  }
}

// OFFSET: 0x00DE0E40
int SkillUseLevels::Lookup(int id) const {
  const auto value = static_cast<std::uint32_t>(id);
  if (value - 1u <= 0x1F3Du) {
    return this->use[value];
  }

  if (id > 10000) {
    return this->additional[value - 10000u];
  }

  if (value - 8000u <= 60u) {
    return this->homunculus[value - 8000u];
  }

  return 0;
}

// OFFSET: 0x00E07540
// FUNCTION: fn_23746
bool ClientSkills::Update(
  const CSkillInfo& value, unsigned char notify, unsigned char updateUnknown000C, LuaScript& script, const SkillUseLevels& levels, SkillWindows& windows
) {
  {
    CSkillInfo snapshot(value);
    const auto update = [](CSkillInfo& target, const CSkillInfo& source, bool copy000C, bool copy0030) {
      target.level = source.level;
      std::memcpy(&target.unknown_0014, &source.unknown_0014, sizeof(target.unknown_0014));
      std::memcpy(&target.unknown_001C, &source.unknown_001C, sizeof(target.unknown_001C));
      std::memcpy(&target.unknown_0018, &source.unknown_0018, sizeof(target.unknown_0018));

      if (copy000C) {
        std::memcpy(&target.unknown_000C, &source.unknown_000C, sizeof(target.unknown_000C));
      }

      if (copy0030) {
        target.unknown_0030 = source.unknown_0030;
      }

      target.unknown_0034 = source.unknown_0034;
    };

    CSkillInfo* grouped = this->bundle.FindGrouped(snapshot.id);
    if (grouped) {
      update(*grouped, snapshot, true, true);
    } else if (snapshot.available) {
      if (CSkillInfo* extra = this->bundle.FindExtra(snapshot.id)) {
        update(*extra, snapshot, true, false);
      } else {
        snapshot.unknown_0024 = UINT32_MAX;
        snapshot.unknown_002C = 0;
        this->bundle.AddExtra(snapshot, windows.windows);
      }
    }

    if (grouped || snapshot.available) {
      if (CSkillInfo::UsesLevel(script, static_cast<int>(snapshot.id)) && snapshot.level != 0) {
        const int saved = levels.Lookup(static_cast<int>(snapshot.id));
        if (saved <= 0 || saved > snapshot.level) {
          this->bundle.SetUnknownValue(static_cast<int>(snapshot.id), snapshot.level);
        }
      }
    }
  }

  if (CSkillInfo* existing = this->bundle.Find(value.id)) {
    existing->level = value.level;
    std::memcpy(&existing->unknown_0014, &value.unknown_0014, sizeof(existing->unknown_0014));
    std::memcpy(&existing->unknown_001C, &value.unknown_001C, sizeof(existing->unknown_001C));
    std::memcpy(&existing->unknown_0018, &value.unknown_0018, sizeof(existing->unknown_0018));

    if (updateUnknown000C == 1) {
      std::memcpy(&existing->unknown_000C, &value.unknown_000C, sizeof(existing->unknown_000C));
    }

    existing->unknown_0030 = value.unknown_0030;
    existing->unknown_0034 = value.unknown_0034;
    if (notify == 1) {
      windows.Notify();
    }

    return false;
  }

  this->bundle.Add(value);
  if (notify != 0) {
    windows.Notify();
  }

  return true;
}

void CompanionSkills::Notify() {
  if (this->window) {
    this->window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (this->sharedWindow) {
    this->sharedWindow->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

bool CompanionSkills::Update(const CSkillInfo& value, unsigned char notify) {
  for (CSkillInfo& skill : this->skills) {
    if (skill.id == value.id) {
      skill.level = value.level;
      std::memcpy(&skill.unknown_0014, &value.unknown_0014, sizeof(skill.unknown_0014));
      std::memcpy(&skill.unknown_001C, &value.unknown_001C, sizeof(skill.unknown_001C));
      std::memcpy(&skill.unknown_0018, &value.unknown_0018, sizeof(skill.unknown_0018));
      std::memcpy(&skill.unknown_000C, &value.unknown_000C, sizeof(skill.unknown_000C));
      skill.unknown_0034 = value.unknown_0034;

      if (notify) {
        this->Notify();
      }

      return false;
    }
  }

  this->skills.push_back(value);
  if (notify) {
    this->Notify();
  }

  return true;
}

// OFFSET: 0x00E07290
// FUNCTION: fn_23740
bool ClientSkills::UpdateAdditional(const CSkillInfo& value, int) {
  for (CSkillInfo& skill : this->additional) {
    if (skill.id == value.id) {
      skill.level = value.level;
      std::memcpy(&skill.unknown_0014, &value.unknown_0014, sizeof(skill.unknown_0014));
      std::memcpy(&skill.unknown_001C, &value.unknown_001C, sizeof(skill.unknown_001C));
      std::memcpy(&skill.unknown_0018, &value.unknown_0018, sizeof(skill.unknown_0018));
      skill.unknown_0034 = value.unknown_0034;

      return false;
    }
  }

  this->additional.push_back(value);
  return true;
}

// OFFSET: 0x00E072F0
// FUNCTION: fn_23741
bool CSession::UpdateHomunSkill(const CSkillInfo& value, unsigned char notify) {
  for (CSkillInfo& skill : this->m_homunSkills) {
    if (skill.id == value.id) {
      skill.level = value.level;
      skill.unknown_0014 = value.unknown_0014;
      skill.unknown_001C = value.unknown_001C;
      skill.unknown_0018 = value.unknown_0018;
      skill.unknown_000C = value.unknown_000C;
      skill.unknown_0034 = value.unknown_0034;
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
    if (skill.id == value.id) {
      skill.level = value.level;
      skill.unknown_0014 = value.unknown_0014;
      skill.unknown_001C = value.unknown_001C;
      skill.unknown_0018 = value.unknown_0018;
      skill.unknown_000C = value.unknown_000C;
      skill.unknown_0034 = value.unknown_0034;
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
void CSession::AddUnknownId(int id) {
  if (std::find(this->unknown_616C.begin(), this->unknown_616C.end(), id) == this->unknown_616C.end()) {
    this->unknown_616C.push_back(id);
  }
}

// OFFSET: 0x00E07930
// FUNCTION: fn_23747
void CSession::AddStackedItem(ItemInfo item) {
  int index;
  for (index = 0; index < 5; ++index) {
    if (this->unknown_5C68[index].quantity != 0 && this->unknown_5C68[index].unknown_0000[1] == item.unknown_0000[1]) {
      break;
    }
  }

  if (index < 5) {
    this->unknown_5C68[index].quantity += item.quantity;
  } else {
    for (index = 0; index < 5; ++index) {
      if (this->unknown_5C68[index].quantity == 0) {
        this->unknown_5C68[index] = item;
        break;
      }
    }
  }

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0458)) {
    window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E07A10
// FUNCTION: fn_23748
void CSession::AddItemToFreeSlot(ItemInfo item) {
  for (int index = 0; index < 5; ++index) {
    if (this->unknown_5C68[index].quantity == 0) {
      this->unknown_5C68[index] = item;
      break;
    }
  }
}

// OFFSET: 0x00E081E0
// FUNCTION: fn_23752
bool CSession::IsListedId(int id, int variant) const {
  if (variant == 0) {
    return id == 19 || id == 4020 || id == 4042 || id == 4068 || id == 4075 || id == 4104 || id == 4263;
  }

  return id == 20 || id == 4021 || id == 4043 || id == 4069 || id == 4076 || id == 4105 || id == 4264;
}

// OFFSET: 0x00E08290
// FUNCTION: fn_23754
void CSession::ClearItemQuantities() {
  for (int index = 0; index < 5; ++index) {
    this->unknown_5C68[index].quantity = 0;
  }

  this->unknown_6140 = 0;
}

// OFFSET: 0x00E082D0
// FUNCTION: fn_23755
void CSession::ResetItems3500() {
  for (int index = 0; index < 10; ++index) {
    this->unknown_3500[index].Reset();
  }
}

// OFFSET: 0x00E08300
// FUNCTION: fn_23756
void CSession::ClearSlots06EB() {
  this->unknown_06EB.unknown_0000 = 0;
  this->unknown_06EB.itemId = 0;
  this->unknown_06EB.unknown_0005 = 0;
  this->unknown_06F2.unknown_0000 = 0;
  this->unknown_06F2.itemId = 0;
  this->unknown_06F2.unknown_0005 = 0;
}

// OFFSET: 0x00E08330
// FUNCTION: fn_23757
void CSession::ClearShortcutSlots() {
  for (int index = 0; index < 36; ++index) {
    this->m_shortcutSlots[index].unknown_0000 = 0;
    this->m_shortcutSlots[index].itemId = 0;
    this->m_shortcutSlots[index].unknown_0005 = 0;
  }
}

// OFFSET: 0x00E08360
// FUNCTION: fn_23758
void CSession::ClearSecondaryShortcutSlots() {
  std::memset(this->m_secondaryShortcutSlots, 0, sizeof(this->m_secondaryShortcutSlots));
}

// OFFSET: 0x00E08380
// FUNCTION: fn_23759
void CSession::ClearUnknown0420() {
  for (int index = 0; index < 36; ++index) {
    this->unknown_0420[index] = 0;
  }
}

// OFFSET: 0x00E083D0
// FUNCTION: fn_23760
void CSession::EraseUnknownKey(std::int64_t key) {
  auto first = this->unknown_0784.find(key);
  if (first != this->unknown_0784.end()) {
    this->unknown_0784.erase(first);
  }

  auto second = this->unknown_078C.find(key);
  if (second != this->unknown_078C.end()) {
    this->unknown_078C.erase(second);
  }
}

// OFFSET: 0x00E08D50
// FUNCTION: fn_23772
int CSession::CountProhibitionLog(std::uint32_t aid) {
  const int count = static_cast<int>(this->m_prohibitionLog.size());
  const CRenderObject* actor = static_cast<CGameMode*>(g_world.GetGameMode())->scene->GetGameActorByAID(aid);
  if (!actor) {
    return -1;
  }

  const std::uint32_t accountId = actor->render.accountId;
  this->m_prohibitionTarget = accountId;
  int matches = 0;
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
void CSession::DeleteProhibitionLog(std::uint32_t aid, std::uint32_t minutes, WindowDialogs& dialogs) {
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

  dialogs.Confirm(message, 0, true, false, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00E08690
// FUNCTION: fn_23764
std::int64_t CSession::FindUnknown0784(std::int64_t key) const {
  auto found = this->unknown_0784.find(key);
  if (found != this->unknown_0784.end()) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00E086F0
// FUNCTION: fn_23765
std::int64_t CSession::FindUnknown078C(std::int64_t key) const {
  auto found = this->unknown_078C.find(key);
  if (found != this->unknown_078C.end()) {
    return found->second;
  }

  return -1;
}

// OFFSET: 0x00E08750
// FUNCTION: fn_23766
SessionRecord6144 CSession::FindRecord6144(int key) const {
  auto found = this->unknown_6144.find(key);
  if (found != this->unknown_6144.end()) {
    return found->second;
  }

  return SessionRecord6144();
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
ItemInfo CSession::GetItem2B50(int index) const {
  if (index == 15) {
    return ItemInfo(0);
  }

  if (index >= 0 && index < 10) {
    return this->unknown_2B50[index];
  }

  return ItemInfo(0);
}

// OFFSET: 0x00E08F70
// FUNCTION: fn_23774
int CSession::CountItems5C68() const {
  int count = 0;
  for (int index = 0; index < 5; ++index) {
    if (this->unknown_5C68[index].quantity != 0) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00E09080
// FUNCTION: fn_23775
void CSession::SplitDate(__time32_t time, std::uint16_t& year, std::uint16_t& month, std::uint16_t& day) const {
  if (time < 0) {
    time = 0;
  }

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
int CSession::FindUnknown0778(int key) const {
  for (const std::pair<int, int>& entry : this->unknown_0778) {
    if (entry.first == key) {
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
  for (const CSkillInfo& skill : this->m_additionalSkills) {
    if (position == index) {
      return skill;
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
  for (const CSkillInfo& skill : this->m_homunSkills) {
    if (position == index) {
      return skill;
    }

    ++position;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00E251C0
// FUNCTION: fn_23902
void CSession::NotifyHomunSkillWindows() {
  if (g_windowMgr.window_03C4) {
    g_windowMgr.window_03C4->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (g_windowMgr.window_01E8) {
    g_windowMgr.window_01E8->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E25200
// FUNCTION: fn_23903
void CSession::NotifyMercSkillWindows() {
  if (g_windowMgr.window_03CC) {
    g_windowMgr.window_03CC->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (g_windowMgr.window_01E8) {
    g_windowMgr.window_01E8->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E08260
// FUNCTION: fn_23753
void CSession::ResetItems2B50() {
  for (int index = 0; index < 10; ++index) {
    this->unknown_2B50[index].Reset();
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

  if (g_session.IsDoramJob(job)) {
    return 0.8f;
  }

  if ((job >= JT_NOVICE_B && job <= JT_THIEF_B) || job == JT_SUPERNOVICE_B || job == JT_SUPERNOVICE2_B) {
    return 0.75f;
  }

  return 0.82f;
}

// OFFSET: 0x00E084B0
// FUNCTION: fn_23761
void ShortcutBindings::ClearAdditionalSkills(const unsigned char& currentRow, LuaScript& script, ModeEvents& modeEvents) {
  for (int slot = 0; slot < 36; ++slot) {
    const NormalItemSlot& value = this->banks[0][slot];
    if (value.itemId != 0 && value.unknown_0000 != 0 && value.itemId >= 10000 && value.itemId < 10020) {
      this->Assign(1, 0, 0, slot, 0, currentRow, script, modeEvents);
    }
  }

  for (int slot = 0; slot < 36; ++slot) {
    const NormalItemSlot& value = this->banks[1][slot];
    if (value.itemId != 0 && value.unknown_0000 != 0 && value.itemId >= 10000 && value.itemId < 10020) {
      this->Assign(1, 0, 0, slot, 1, currentRow, script, modeEvents);
    }
  }
}

// OFFSET: 0x00E089A0
// FUNCTION: fn_23768
int CharacterConfigSave::SkillLevel(int id) const {
  if (id - 8000u <= 60u) {
    return g_session.HomunSkillLevel(id);
  }

  if (id - 8200u <= 41u || id - 8400u <= 57u) {
    return g_session.MercSkillLevel(id);
  }

  return this->UseLevel(id);
}

// OFFSET: 0x00E094E0
// FUNCTION: fn_23778
CRenderObject* CSession::FindGameActor(std::uint32_t aid) const {
  CGameMode* mode = static_cast<CGameMode*>(g_world.GetGameMode());
  if (mode == nullptr) {
    return nullptr;
  }

  return mode->scene->GetGameActorByAID(aid);
}
