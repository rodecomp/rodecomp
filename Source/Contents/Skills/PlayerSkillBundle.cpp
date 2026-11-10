#include "Contents/Skills/PlayerSkillBundle.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

bool IsIndexedSkill(int id);

using SkillGroupTreeValue = std::_Tree_val<std::_Tree_simple_types<std::pair<const int, std::list<CSkillInfo>>>>;
extern template std::_Tree_unchecked_const_iterator<SkillGroupTreeValue, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<SkillGroupTreeValue, std::_Iterator_base0>::operator++() noexcept;

// INLINED
std::size_t SkillIdHash::operator()(std::uint32_t id) const {
  return std::hash<std::uint32_t>()(id);
}

// INLINED
CPlayerSkillBundle::lcEXCInfo::lcEXCInfo() {
}

// OFFSET: 0x00770A30
// FUNCTION: fn_08260
CPlayerSkillBundle::lcEXCInfo::~lcEXCInfo() = default;

// OFFSET: 0x0076FB70
// FUNCTION: fn_08253
CPlayerSkillBundle::CPlayerSkillBundle() {
}

// OFFSET: 0x00770830
// FUNCTION: fn_08258
CPlayerSkillBundle::~CPlayerSkillBundle() = default;

// OFFSET: 0x00773390
// FUNCTION: fn_08309
void CPlayerSkillBundle::Add(const CSkillInfo& value) {
  this->skills.push_back(value);
  if (IsIndexedSkill(value.id)) {
    this->index[value.id] = this->Find(value.id);
  }
}

// OFFSET: 0x00771160
// FUNCTION: fn_08272
void CPlayerSkillBundle::Remove(std::uint32_t id) {
  const auto entry = std::find_if(this->skills.begin(), this->skills.end(),
                                  [id](const CSkillInfo& value) { return value.id == id; });
  if (entry == this->skills.end()) {
    return;
  }

  const auto indexed = this->index.find(entry->id);
  if (indexed != this->index.end()) {
    this->index.erase(indexed);
  }

  this->skills.erase(entry);
}

// OFFSET: 0x007715B0
// FUNCTION: fn_08279
CSkillInfo* CPlayerSkillBundle::Find(std::uint32_t id) {
  const auto entry = std::find_if(this->skills.begin(), this->skills.end(),
                                  [id](const CSkillInfo& value) { return id == value.id; });
  return entry != this->skills.end() ? &*entry : nullptr;
}

__declspec(noinline) std::uint32_t ResolveSkillId(std::uint32_t id);

// OFFSET: 0x00CACF20
// FUNCTION: fn_21049
std::uint32_t ResolveSkillId(std::uint32_t id) {
  if (g_session.GetPlayerSkillMode() != 1 || (id != 6578 && id != 6579)) {
    return id;
  }

  CSkillInfo* skill;
  if (g_session.HasSkillEffect(1717) && g_session.m_skillBundle.FindIndexed(6580, skill) && skill->IsValid()) {
    return 6580;
  }

  if (g_session.HasSkillEffect(1716) && g_session.m_skillBundle.FindIndexed(6579, skill) && skill->IsValid()) {
    return 6579;
  }

  return id;
}

// OFFSET: 0x007716B0
// FUNCTION: fn_08280
CSkillInfo CPlayerSkillBundle::Lookup(std::uint32_t id) {
  if (this->skills.empty()) {
    return CSkillInfo(-1);
  }

  const std::uint32_t key = id == 6578 || id == 6579 ? ResolveSkillId(id) : id;
  const auto entry = this->index.find(key);
  if (entry != this->index.end() && entry->second->IsValid()) {
    return *entry->second;
  }

  const auto found = std::find_if(this->skills.begin(), this->skills.end(),
                                  [id](const CSkillInfo& value) { return value.id == id; });
  if (found != this->skills.end()) {
    return *found;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x007717F0
// FUNCTION: fn_08281
bool CPlayerSkillBundle::FindIndexed(std::uint32_t id, CSkillInfo*& output) {
  const auto entry = this->index.find(id);
  if (entry == this->index.end()) {
    return false;
  }

  output = entry->second;
  return true;
}

// OFFSET: 0x00770EC0
// FUNCTION: fn_08269
bool CPlayerSkillBundle::UpdatePending(const CSkillInfo& value) {
  for (CSkillInfo& entry : this->pending) {
    if (value.id == entry.id) {
      entry.level = value.level;
      entry.id = value.id;
      entry.spCost = value.spCost;
      entry.upgradable = value.upgradable;
      entry.apCost = value.apCost;

      return false;
    }
  }

  this->pending.push_back(value);
  return true;
}

// OFFSET: 0x00771880
// FUNCTION: fn_08282
CSkillInfo CPlayerSkillBundle::Pending(std::uint32_t id) {
  if (this->pending.empty()) {
    return CSkillInfo(-1);
  }

  const auto entry = std::find_if(this->pending.begin(), this->pending.end(),
                                  [id](const CSkillInfo& value) { return value.id == id; });
  if (entry != this->pending.end()) {
    return *entry;
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00771560
// FUNCTION: fn_08277
CSkillInfo* CPlayerSkillBundle::FindExtra(std::uint32_t id) {
  const auto entry = std::find_if(this->extra.begin(), this->extra.end(),
                                  [id](const CSkillInfo& value) { return id == value.id; });
  return entry != this->extra.end() ? &*entry : nullptr;
}

// OFFSET: 0x00771450
// FUNCTION: fn_08275
void CPlayerSkillBundle::ClearExtra() {
  this->extra.clear();
  this->extraIds.clear();
}

// OFFSET: 0x007713E0
// FUNCTION: fn_08274
void CPlayerSkillBundle::AddExtra(const CSkillInfo& value) {
  this->extraIds.push_back(value.id);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_SKILLLISTWND)) {
    window->SendMsg(nullptr, UM_REFRESH_EXTRA_SKILL_TAB, 0, 0, 0, 0);
  }

  this->extra.push_back(value);
}

// OFFSET: 0x007714A0
// FUNCTION: fn_08276
bool CPlayerSkillBundle::RemoveExtra(std::uint32_t id) {
  for (auto entry = this->extra.begin(); entry != this->extra.end(); ++entry) {
    if (id != entry->id) {
      continue;
    }

    this->extra.erase(entry);
    const auto item = std::find(this->extraIds.begin(), this->extraIds.end(), id);
    if (item != this->extraIds.end()) {
      this->extraIds.erase(item);
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_SKILLLISTWND)) {
        window->SendMsg(nullptr, UM_REFRESH_EXTRA_SKILL_TAB, 0, 0, 0, 0);
      }
    }

    return true;
  }

  return false;
}

// OFFSET: 0x00771590
// FUNCTION: fn_08278
void CPlayerSkillBundle::NumberExtra() {
  unsigned number = 0;

  for (auto entry = this->extra.begin(); entry != this->extra.end(); ++entry, ++number) {
    entry->treePosition = number;
  }
}

// OFFSET: 0x00771C00
// FUNCTION: fn_08288
void CPlayerSkillBundle::AddGrouped(int group, const CSkillInfo& value) {
  this->groups[group].push_back(value);
}

// OFFSET: 0x00771D20
// FUNCTION: fn_08289
void CPlayerSkillBundle::ClearGroups() {
  this->groups.clear();
}

// OFFSET: 0x00771D50
// FUNCTION: fn_08290
CSkillInfo* CPlayerSkillBundle::FindGrouped(std::uint32_t id) {
  const auto end = this->groups.end();
  for (auto group = this->groups.begin(); group != end; ++group) {
    for (CSkillInfo& value : group->second) {
      if (id == value.id) {
        return &value;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00771DA0
// FUNCTION: fn_08291
std::list<CSkillInfo>& CPlayerSkillBundle::Group(int group) {
  static std::list<CSkillInfo> missing;
  const auto entry = this->groups.find(group);
  return entry != this->groups.end() ? entry->second : missing;
}

// OFFSET: 0x00771F30
// FUNCTION: fn_08292
std::size_t CPlayerSkillBundle::GroupSize(int group) const {
  const auto entry = this->groups.find(group);
  return entry != this->groups.end() ? entry->second.size() : 0;
}

// OFFSET: 0x00771F80
// FUNCTION: fn_08293
int CPlayerSkillBundle::GroupedLevel(std::uint32_t id) {
  const CSkillInfo* value = this->FindGrouped(id);
  return value ? value->level : 0;
}

// OFFSET: 0x00771980
// FUNCTION: fn_08284
bool CPlayerSkillBundle::HasHigherLevel(std::uint32_t id) {
  const CSkillInfo* value = this->FindGrouped(id);
  return value ? value->level > value->learnedLevel : false;
}

// OFFSET: 0x00770F20
// FUNCTION: fn_08270
void CPlayerSkillBundle::RecordLevelChange(const CSkillInfo& value) {
  if (value.level == 0) {
    return;
  }

  CSkillInfo* previous = this->FindGrouped(value.id);
  if (!previous || previous->level >= value.level) {
    return;
  }

  const int id = static_cast<int>(previous->id);
  lcEXCInfo change;
  change.previous = previous->level;
  change.target = value.level;
  this->levelChanges[id] = change;
  this->changed[id] = true;
}

// OFFSET: 0x00771250
// FUNCTION: fn_08273
std::pair<bool, int> CPlayerSkillBundle::PreviousLevel(int id) const {
  const auto entry = this->levelChanges.find(id);
  const bool found = entry != this->levelChanges.end();
  return found ? std::make_pair(true, entry->second.previous) : std::make_pair(false, 0);
}

// OFFSET: 0x00771FC0
// FUNCTION: fn_08295
void CPlayerSkillBundle::ApplyLevelChange(CSkillInfo& value) {
  const int id = static_cast<int>(value.id);
  const auto entry = this->levelChanges.find(id);
  if (entry != this->levelChanges.end()) {
    lcEXCInfo& change = entry->second;
    if (value.level == change.previous || change.target < value.level) {
      this->removals.push_back(id);
    } else if (change.previous < value.level && value.level < change.target) {
      change.previous = value.level;
      value.level = change.target;
    }
  }

  for (int removed : this->removals) {
    const auto change = this->levelChanges.find(removed);
    if (change != this->levelChanges.end()) {
      this->levelChanges.erase(change);
      const auto flag = this->changed.find(removed);
      if (flag != this->changed.end()) {
        this->changed.erase(flag);
      }
    }
  }

  this->removals.clear();
}

// OFFSET: 0x00770FD0
// FUNCTION: fn_08271
void CPlayerSkillBundle::ClearLevelChanges() {
  this->levelChanges.clear();
  this->changed.clear();
  this->removals.clear();
}

// OFFSET: 0x00771920
// FUNCTION: fn_08283
void CPlayerSkillBundle::MarkLevelChanges() {
  for (auto entry = this->changed.begin(); entry != this->changed.end(); ++entry) {
    entry->second = true;
  }
}

// OFFSET: 0x00771AF0
// FUNCTION: fn_08286
void CPlayerSkillBundle::ResetFirstLevelChange() {
  const auto entry = this->levelChanges.begin();
  if (entry != this->levelChanges.end()) {
    entry->second.previous = 0;
  }
}

// OFFSET: 0x00771FA0
// FUNCTION: fn_08294
void CPlayerSkillBundle::SetUnknownValue(int id, int value) {
  if (static_cast<unsigned>(id) - 1u <= 0x1F3D) {
    this->skillValues[id] = value;
  }
}

// OFFSET: 0x00771B40
// FUNCTION: fn_08287
void CPlayerSkillBundle::ForEachGrouped(std::function<bool(CSkillInfo*)> callback) {
  for (auto group = this->groups.begin(); group != this->groups.end(); ++group) {
    for (CSkillInfo& value : group->second) {
      if (!callback(&value)) {
        return;
      }
    }
  }
}

// OFFSET: 0x007719B0
// FUNCTION: fn_08285
bool IsIndexedSkill(int id) {
  switch (id) {
  case AT_FERAL_CLAW:
  case AT_ALPHA_CLAW:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x007719D0

// OFFSET: 0x00771A50
// FUNCTION: fn_45559
void CPlayerSkillBundle::TakePreviousLevel(int& level, int id, int key) {
  if (this->levelChanges.find(key) == this->levelChanges.end()) {
    return;
  }

  if (key == id) {
    if (this->changed[id]) {
      level = this->levelChanges[id].previous;
      this->changed[id] = false;
    }
  } else {
    level = this->levelChanges[key].previous;
  }
}
