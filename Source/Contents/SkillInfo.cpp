#include "Contents/NormalItemSlots.h"
#include "Contents/SkillInfo.h"
#include "Script/LuaScript.h"

#include <bit>
#include <cstring>

// OFFSET: 0x00773480
// FUNCTION: fn_08311
CSkillInfo::CSkillInfo()
    : available(1), id(0), level(0), name(nullptr), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
}

// OFFSET: 0x00773400
// FUNCTION: fn_08310
CSkillInfo::CSkillInfo(int)
    : available(0), id(0), level(0), name("Unknown-Skill"), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
}

// OFFSET: 0x0076FD70
// FUNCTION: fn_08254
CSkillInfo::CSkillInfo(const CSkillInfo& other) {
  *this = other;
}

// OFFSET: 0x009149F0
// FUNCTION: fn_12543
CSkillInfo& CSkillInfo::operator=(const CSkillInfo& other) {
  this->available = other.available;
  if (this == &other) {
    return *this;
  }
  this->id = other.id;
  // Preserve the unknown member representations, including fields left
  // untouched by the constructors, without evaluating indeterminate integers.
  std::memcpy(&this->unknown_000C, &other.unknown_000C, sizeof(this->unknown_000C));
  this->level = other.level;
  std::memcpy(&this->unknown_0014, &other.unknown_0014, sizeof(this->unknown_0014));
  std::memcpy(&this->unknown_0018, &other.unknown_0018, sizeof(this->unknown_0018));
  std::memcpy(&this->unknown_001C, &other.unknown_001C, sizeof(this->unknown_001C));
  this->name = other.name;
  std::memcpy(&this->unknown_0024, &other.unknown_0024, sizeof(this->unknown_0024));
  this->unknown_0028 = other.unknown_0028;
  std::memcpy(&this->unknown_002C, &other.unknown_002C, sizeof(this->unknown_002C));
  this->unknown_0030 = other.unknown_0030;
  this->unknown_0034 = other.unknown_0034;
  this->entries = other.entries;
  return *this;
}

// OFFSET: 0x00773500
// FUNCTION: fn_08312
CSkillInfo::~CSkillInfo() = default;

// OFFSET: 0x00774710
// FUNCTION: fn_08325
bool CSkillInfo::IsValid() const {
  return this->available != 0 && this->level != 0;
}

// OFFSET: 0x008AC440
// FUNCTION: fn_11732
bool CSkillInfo::MatchesShortcut(const NormalItemSlot* slot) const {
  if (!slot || !this->IsValid()) {
    return false;
  }
  const auto requested = std::bit_cast<std::int16_t>(slot->unknown_0005);
  return requested != 0 && (this->id != slot->itemId || this->level >= requested);
}

// OFFSET: 0x00773550
// FUNCTION: fn_08313
int CSkillInfo::UseApAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseApAmount", "dd>d", std::bit_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x00773620
// FUNCTION: fn_08314
int CSkillInfo::UseSpAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseSpAmount", "dd>d", std::bit_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x007736F0
// FUNCTION: fn_08315
const char* CSkillInfo::DisplayName(LuaScript& script) const {
  return CSkillInfo::DisplayName(script, std::bit_cast<int>(this->id));
}

// OFFSET: 0x00773920
// FUNCTION: fn_08318
const char* CSkillInfo::DisplayName(LuaScript& script, int skillId) {
  if (skillId == 5042) {
    skillId = 5028;
  } else if (skillId == 5043) {
    skillId = 5036;
  }
  const char* result = nullptr;
  return script.Call("GetSkillName", "d>s", skillId, &result) ? result : "Unknown-Skill";
}

// OFFSET: 0x007737C0
// FUNCTION: fn_08316
const char* CSkillInfo::IdName(LuaScript& script) const {
  return CSkillInfo::IdName(script, std::bit_cast<int>(this->id));
}

// OFFSET: 0x00773870
// FUNCTION: fn_08317
const char* CSkillInfo::IdName(LuaScript& script, int skillId) {
  const char* result = nullptr;
  return script.Call("GetSkillIdName", "d>s", skillId, &result) ? result : "Zero Skill";
}

// OFFSET: 0x00774500
// FUNCTION: fn_08323
bool CSkillInfo::UsesLevel(LuaScript& script, int skillId) {
  bool result = false;
  return script.Call("IsLevelUseSkill", "d>b", skillId, &result) ? result : false;
}
