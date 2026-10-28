#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/Skills/SkillInfo.h"
#include "Script/LuaScript.h"

#include <cstring>
#include <memory>

// GLOBAL: gl_24640
CSkillActionMgr* g_skillActionMgr;

// INLINED
CSkillActionMgr::~CSkillActionMgr() = default;

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
// FUNCTION: fn_08322
bool Sub773CD0(int skillId) {
  switch (skillId) {
  case 0x4:
  case 0x1C:
  case 0x1D:
  case 0x22:
  case 0x23:
  case 0x35:
  case 0x36:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x6C:
  case 0x8A:
  case 0x8B:
  case 0xE7:
  case 0xEA:
  case 0xEB:
  case 0xEC:
  case 0xED:
  case 0xFF:
  case 0x100:
  case 0x106:
  case 0x118:
  case 0x119:
  case 0x11A:
  case 0x11B:
  case 0x169:
  case 0x1BD:
  case 0x1BE:
  case 0x1BF:
  case 0x1C0:
  case 0x1C1:
  case 0x1C2:
  case 0x1C3:
  case 0x1C4:
  case 0x1C5:
  case 0x1C6:
  case 0x1C7:
  case 0x1C8:
  case 0x1C9:
  case 0x1CA:
  case 0x1CC:
  case 0x1CD:
  case 0x1CE:
  case 0x1CF:
  case 0x1D0:
  case 0x1D1:
  case 0x1DF:
  case 0x1EE:
  case 0x234:
  case 0x23C:
  case 0x23D:
  case 0x23E:
  case 0x23F:
  case 0x241:
  case 0x251:
  case 0x26D:
  case 0x26E:
  case 0x272:
  case 0x27C:
  case 0x27D:
  case 0x281:
  case 0x282:
  case 0x283:
  case 0x285:
  case 0x286:
  case 0x2B0:
  case 0x2B1:
  case 0x2B2:
  case 0x2B3:
  case 0x2B6:
  case 0x2D9:
  case 0x3F7:
  case 0x7D1:
  case 0x7DC:
  case 0x7EA:
  case 0x7F9:
  case 0x7FA:
  case 0x7FB:
  case 0x802:
  case 0x803:
  case 0x804:
  case 0x805:
  case 0x8E3:
  case 0x8EF:
  case 0x912:
  case 0x925:
  case 0x929:
  case 0x92B:
  case 0x92C:
  case 0x94F:
  case 0x975:
  case 0x979:
  case 0x993:
  case 0x9B0:
  case 0x9B2:
  case 0x9D3:
  case 0x9E9:
  case 0xA24:
  case 0xA25:
  case 0xA26:
  case 0xA27:
  case 0xA30:
  case 0xA34:
  case 0xBD1:
  case 0xBD2:
  case 0xBD3:
  case 0xBD7:
  case 0xBD8:
  case 0xBD9:
  case 0xBDA:
  case 0x13AB:
  case 0x13AE:
  case 0x13AF:
  case 0x13B0:
  case 0x13B1:
  case 0x13C8:
  case 0x1494:
  case 0x1495:
  case 0x1497:
  case 0x1498:
  case 0x149B:
  case 0x14A0:
  case 0x14A1:
  case 0x14A2:
  case 0x14B2:
  case 0x14B3:
  case 0x14DB:
  case 0x14F6:
  case 0x14F8:
  case 0x1F55:
  case 0x2022:
  case 0x2023:
  case 0x2024:
  case 0x2025:
  case 0x2026:
  case 0x2027:
  case 0x202E:
  case 0x202F:
  case 0x2030:
    return true;
  default:
    return CSkillActionMgr::GetInstance()->Sub77B4A0(skillId);
  }
}

// OFFSET: 0x0077B4A0
// FUNCTION: fn_08413
bool CSkillActionMgr::Sub77B4A0(int skillId) {
  auto it = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(it != this->m_skillActions.end() ? it->second() : nullptr);
  return action ? action->Slot00() : false;
}

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
