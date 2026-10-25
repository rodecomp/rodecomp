#include "Contents/Skills/PlayerSkillBundle.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

std::size_t SkillIdHash::operator()(std::uint32_t id) const {
  std::uint32_t hash = 0x811C9DC5u;

  for (unsigned shift = 0; shift < 32; shift += 8) {
    hash = (hash ^ ((id >> shift) & 255)) * 16777619u;
  }

  return hash;
}

// OFFSET: 0x00770A30
// FUNCTION: fn_08260
CPlayerSkillBundle::lcEXCInfo::lcEXCInfo() {
}

// OFFSET: 0x00770E90
CPlayerSkillBundle::lcEXCInfo::~lcEXCInfo() = default;

// OFFSET: 0x0076FB70
// FUNCTION: fn_08253
CPlayerSkillBundle::CPlayerSkillBundle() : index(8) {
}

// OFFSET: 0x00770830
// FUNCTION: fn_08258
// OFFSET: 0x00770D40
// FUNCTION: fn_08266
CPlayerSkillBundle::~CPlayerSkillBundle() = default;

// OFFSET: 0x00773390
// FUNCTION: fn_08309
void CPlayerSkillBundle::Add(const CSkillInfo& value) {
  this->skills.push_back(value);
  if (value.id == 6579 || value.id == 6580) {
    this->index[value.id] = this->Find(value.id);
  }
}

// OFFSET: 0x00771160
// FUNCTION: fn_08272
void CPlayerSkillBundle::Remove(std::uint32_t id) {
  for (auto entry = this->skills.begin(); entry != this->skills.end(); ++entry) {
    if (entry->id == id) {
      this->index.erase(id);
      this->skills.erase(entry);
      return;
    }
  }
}

// OFFSET: 0x007715B0
// FUNCTION: fn_08279
CSkillInfo* CPlayerSkillBundle::Find(std::uint32_t id) {
  for (CSkillInfo& value : this->skills) {
    if (value.id == id) {
      return &value;
    }
  }

  return nullptr;
}

// OFFSET: 0x00CACF20
// FUNCTION: fn_21049
std::uint32_t CPlayerSkillBundle::ResolveSkillId(std::uint32_t id, int classMode, bool effect1717, bool effect1716) {
  if (classMode != 1 || (id != 6578 && id != 6579)) {
    return id;
  }

  const auto valid = [&](std::uint32_t key) {
    const auto entry = this->index.find(key);
    return entry != this->index.end() && entry->second->IsValid();
  };
  if (effect1717 && valid(6580)) {
    return 6580;
  }

  if (effect1716 && valid(6579)) {
    return 6579;
  }

  return id;
}

// OFFSET: 0x007716B0
// FUNCTION: fn_08280
CSkillInfo CPlayerSkillBundle::Lookup(std::uint32_t id, int classMode, bool effect1717, bool effect1716) {
  if (this->skills.empty()) {
    return CSkillInfo(-1);
  }

  const auto entry = this->index.find(this->ResolveSkillId(id, classMode, effect1717, effect1716));
  if (entry != this->index.end() && entry->second->IsValid()) {
    return *entry->second;
  }

  CSkillInfo* value = this->Find(id);
  return value ? *value : CSkillInfo(-1);
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
    if (entry.id == value.id) {
      entry.level = value.level;
      entry.id = value.id;
      entry.unknown_0014 = value.unknown_0014;
      entry.unknown_0018 = value.unknown_0018;
      entry.unknown_0034 = value.unknown_0034;

      return false;
    }
  }

  this->pending.push_back(value);
  return true;
}

// OFFSET: 0x00771880
// FUNCTION: fn_08282
CSkillInfo CPlayerSkillBundle::Pending(std::uint32_t id) {
  for (const CSkillInfo& value : this->pending) {
    if (value.id == id) {
      return value;
    }
  }

  return CSkillInfo(-1);
}

// OFFSET: 0x00771560
// FUNCTION: fn_08277
CSkillInfo* CPlayerSkillBundle::FindExtra(std::uint32_t id) {
  for (CSkillInfo& value : this->extra) {
    if (value.id == id) {
      return &value;
    }
  }

  return nullptr;
}

// OFFSET: 0x00771450
// FUNCTION: fn_08275
void CPlayerSkillBundle::ClearExtra() {
  this->extra.clear();
  this->extraIds.clear();
}

// OFFSET: 0x007713E0
// FUNCTION: fn_08274
void CPlayerSkillBundle::AddExtra(const CSkillInfo& value, WindowManagerWindows& windows) {
  this->extraIds.push_back(value.id);
  if (UIWindow* window = windows.LookupWindow(37)) {
    window->SendMsg(nullptr, 161, 0, 0, 0, 0);
  }

  this->extra.push_back(value);
}

// OFFSET: 0x007714A0
// FUNCTION: fn_08276
bool CPlayerSkillBundle::RemoveExtra(std::uint32_t id, WindowManagerWindows& windows) {
  for (auto entry = this->extra.begin(); entry != this->extra.end(); ++entry) {
    if (entry->id != id) {
      continue;
    }

    this->extra.erase(entry);
    const auto item = std::find(this->extraIds.begin(), this->extraIds.end(), id);
    if (item != this->extraIds.end()) {
      this->extraIds.erase(item);
      if (UIWindow* window = windows.LookupWindow(37)) {
        window->SendMsg(nullptr, 161, 0, 0, 0, 0);
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

  for (CSkillInfo& value : this->extra) {
    value.unknown_0024 = number++;
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
  for (auto& [group, values] : this->groups) {
    for (CSkillInfo& value : values) {
      if (value.id == id) {
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
  return entry == this->groups.end() ? missing : entry->second;
}

// OFFSET: 0x00771F30
// FUNCTION: fn_08292
std::size_t CPlayerSkillBundle::GroupSize(int group) const {
  const auto entry = this->groups.find(group);
  return entry == this->groups.end() ? 0 : entry->second.size();
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
  return value && value->level > value->unknown_0030;
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

  lcEXCInfo& entry = this->levelChanges[static_cast<int>(value.id)];
  entry.previous = previous->level;
  entry.target = value.level;
  this->changed[static_cast<int>(value.id)] = true;
}

// OFFSET: 0x00771250
// FUNCTION: fn_08273
bool CPlayerSkillBundle::PreviousLevel(int id, int& output) const {
  const auto entry = this->levelChanges.find(id);
  output = entry == this->levelChanges.end() ? 0 : entry->second.previous;
  return entry != this->levelChanges.end();
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
    if (this->levelChanges.erase(removed)) {
      this->changed.erase(removed);
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
  for (auto& [id, value] : this->changed) {
    value = true;
  }
}

// OFFSET: 0x00771AF0
// FUNCTION: fn_08286
void CPlayerSkillBundle::ResetFirstLevelChange() {
  if (!this->levelChanges.empty()) {
    this->levelChanges.begin()->second.previous = 0;
  }
}

// OFFSET: 0x00771FA0
// FUNCTION: fn_08294
void CPlayerSkillBundle::SetUnknownValue(int id, int value) {
  if (static_cast<unsigned>(id) - 1u <= 0x1F3D) {
    this->unknown_0064[id] = value;
  }
}

// OFFSET: 0x00771B40
// FUNCTION: fn_08287
void CPlayerSkillBundle::ForEachGrouped(std::function<bool(CSkillInfo*)> callback) {
  for (auto& [group, values] : this->groups) {
    for (CSkillInfo& value : values) {
      if (!callback(&value)) {
        return;
      }
    }
  }
}
