#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <new>
#include <utility>
#include <vector>

class CGameActor;
class CRenderObject;
class CSkillObjectAction;
class LuaScript;
struct EffectVector3;
struct NormalItemSlot;

struct EfstActionMessage {
  /* 0000 */ const char* text;
  /* 0004 */ std::uint32_t color;
};

class CSkillAction {
public:
  virtual bool Slot00() {
    return false;
  }
  virtual int Slot01() {
    return 8;
  }
  virtual int Slot02() {
    return 7;
  }
  virtual bool Slot03(int argument) {
    return false;
  }
  virtual bool Slot04() {
    return false;
  }
  virtual bool Slot05() {
    return false;
  }
  virtual bool Slot06() {
    return false;
  }
  virtual bool Slot07() {
    return false;
  }
  virtual void Slot08(int level, CGameActor* source) {
  }
  virtual void Slot09(int argument1, int argument2, int argument3) {
  }
  virtual void Slot10(int argument1, int argument2, int argument3, int argument4, int argument5) {
  }
  virtual void Slot11(int level, CGameActor* source, CGameActor* target) {
  }
  virtual void Slot12(int argument1, int argument2, int argument3, int argument4, int argument5) {
  }
  virtual bool Slot13() {
    return false;
  }
  virtual int Slot14() {
    return 0;
  }
  virtual void Slot15(int level, CGameActor* source, CGameActor* target) {
  }
  virtual void Slot16(int level, CGameActor* source, CGameActor* target) {
  }
};

class CEfstAction {
public:
  virtual void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  }
  virtual void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  }
  virtual void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  }
  virtual EfstActionMessage Slot03() {
    return {nullptr, 0xFFFF00};
  }
  virtual EfstActionMessage Slot04() {
    return {nullptr, 0xFF};
  }
};

struct SkillInfoEntry {
  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ std::uint32_t unknown_0004;
};

class CSkillInfo {
public:
  CSkillInfo();
  explicit CSkillInfo(int unused);
  CSkillInfo(const CSkillInfo& other);
  CSkillInfo& operator=(const CSkillInfo& other);
  virtual ~CSkillInfo();
  bool IsValid() const;
  bool MatchesShortcut(const NormalItemSlot* slot) const;
  int UseApAmount(LuaScript& script, int skillLevel) const;
  int UseSpAmount(LuaScript& script, int skillLevel) const;
  const char* DisplayName(LuaScript& script) const;
  const char* IdName(LuaScript& script) const;
  static const char* DisplayName(LuaScript& script, int skillId);
  static const char* IdName(LuaScript& script, int skillId);
  static bool UsesLevel(LuaScript& script, int skillId);

  /* 0004 */ std::int32_t available;
  /* 0008 */ std::uint32_t id;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::int32_t level;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint32_t unknown_0018;
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ const char* name;
  /* 0024 */ std::uint32_t unknown_0024;
  /* 0028 */ std::uint32_t unknown_0028;
  /* 002C */ std::uint32_t unknown_002C;
  /* 0030 */ std::int16_t unknown_0030;
  /* 0034 */ std::uint32_t unknown_0034;
  /* 0038 */ std::vector<SkillInfoEntry> entries;
};

// VTABLE: vt_00159
class CSkillActionMgr {
public:
  /* 0004 */ std::map<int, std::function<CSkillAction*()>> m_skillActions;
  /* 000C */ std::map<int, std::function<CEfstAction*()>> m_efstActions;
  /* 0014 */ std::map<int, std::function<CSkillObjectAction*()>> m_skillObjectActions;

  virtual ~CSkillActionMgr();

  static CSkillActionMgr* GetInstance();

  bool IsSupportAction(int skillId);
  std::pair<bool, int> ActionSlot01(int skillId);
  std::pair<bool, int> ActionSlot02(int skillId);
  int ActionSlot14(int skillId);
  bool ActionSlot03(int skillId, int argument);
  bool ActionSlot04(int skillId);
  EfstActionMessage EfstSlot03(int efstId);
  EfstActionMessage EfstSlot04(int efstId);
  bool HasEfstAction(int efstId);
};

extern CSkillActionMgr* g_skillActionMgr;

inline CSkillActionMgr* CSkillActionMgr::GetInstance() {
  if (!g_skillActionMgr) {
    g_skillActionMgr = new (std::nothrow) CSkillActionMgr;
  }

  return g_skillActionMgr;
}

void __stdcall Sub7739F0();
bool IsOptionalSelfTargetSkill(int skillId);
bool HidesUseFailureMessage(int skillId);
bool IsSupportSkill(int skillId);
bool ShowsCastEffect(int skillId);
EffectVector3 CellWorldPosition(int column, int row);
static_assert(sizeof(SkillInfoEntry) == 8);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SkillInfoEntry, unknown_0004) == 0x4);
static_assert(sizeof(CSkillInfo) == 68);
static_assert(offsetof(CSkillActionMgr, m_efstActions) == 0x0C);
static_assert(offsetof(CSkillActionMgr, m_skillObjectActions) == 0x14);
static_assert(sizeof(CSkillActionMgr) == 0x1C);
static_assert(sizeof(CSkillAction) == 4);
static_assert(sizeof(CEfstAction) == 4);
static_assert(sizeof(EfstActionMessage) == 8);
#endif

#endif
