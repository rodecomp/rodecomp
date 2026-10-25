#include "Contents/Skills/ClientSkills.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstring>

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

// OFFSET: 0x00E251C0
// OFFSET: 0x00E25200
void CompanionSkills::Notify() {
  if (this->window) {
    this->window->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (this->sharedWindow) {
    this->sharedWindow->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E072F0
// OFFSET: 0x00E07380
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
