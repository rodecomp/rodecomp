#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/Skills/SkillInfo.h"
#include "Script/LuaScript.h"

#include <cstring>

// OFFSET: 0x00773480
// FUNCTION: fn_08311
CSkillInfo::CSkillInfo() : available(1), id(0), level(0), name(nullptr), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
}

// OFFSET: 0x00773400
// FUNCTION: fn_08310
CSkillInfo::CSkillInfo(int) : available(0), id(0), level(0), name("Unknown-Skill"), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
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

  const auto requested = slot->unknown_0005;
  return requested != 0 && (this->id != slot->itemId || this->level >= requested);
}

// OFFSET: 0x00773550
// FUNCTION: fn_08313
int CSkillInfo::UseApAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseApAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x00773620
// FUNCTION: fn_08314
int CSkillInfo::UseSpAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseSpAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x007736F0
// FUNCTION: fn_08315
const char* CSkillInfo::DisplayName(LuaScript& script) const {
  return CSkillInfo::DisplayName(script, static_cast<int>(this->id));
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

// OFFSET: 0x007739F0
// FUNCTION: fn_08319
void __stdcall Sub7739F0() {
}

// OFFSET: 0x00773A00
// FUNCTION: fn_08320
bool Sub773A00(int skillId) {
  switch (skillId) {
  case 28:
  case 29:
  case 30:
  case 34:
  case 35:
  case 53:
  case 54:
  case 66:
  case 67:
  case 68:
  case 71:
  case 72:
  case 73:
  case 77:
  case 78:
  case 138:
  case 139:
  case 231:
  case 234:
  case 235:
  case 236:
  case 237:
  case 255:
  case 280:
  case 281:
  case 282:
  case 283:
  case 286:
  case 289:
  case 374:
  case 402:
  case 463:
  case 464:
  case 465:
  case 466:
  case 467:
  case 468:
  case 469:
  case 470:
  case 471:
  case 472:
  case 479:
  case 1015:
  case 2001:
  case 2020:
  case 2026:
  case 2027:
  case 2050:
  case 2051:
  case 2052:
  case 2053:
  case 2341:
  case 2344:
  case 2345:
  case 2346:
  case 2347:
  case 2348:
  case 2451:
  case 2515:
  case 8221:
      return true;
  default:
    return false;
  }
}

// OFFSET: 0x00773CB0
// FUNCTION: fn_08321
bool Sub773CB0(int skillId) {
  return skillId == 0x1998 || skillId == 0x19C9;
}

// OFFSET: 0x00773CD0

// OFFSET: 0x007737C0
// FUNCTION: fn_08316
const char* CSkillInfo::IdName(LuaScript& script) const {
  return CSkillInfo::IdName(script, static_cast<int>(this->id));
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

// OFFSET: 0x007745B0
// FUNCTION: fn_08324
bool Sub7745B0(int skillId) {
  switch (skillId) {
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 27:
  case 29:
  case 30:
  case 32:
  case 33:
  case 77:
  case 86:
  case 340:
  case 690:
  case 731:
  case 2008:
  case 2533:
  case 2534:
  case 2536:
  case 5004:
      return false;
  default:
    return true;
  }
}
