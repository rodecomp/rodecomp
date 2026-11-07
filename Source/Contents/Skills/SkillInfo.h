#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H

#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <new>
#include <utility>
#include <vector>

class CGameActor;
class CRenderObject;
class LuaScript;
struct NormalItemSlot;

struct EfstActionMessage {
  /* 0000 */ const char* text;
  /* 0004 */ std::uint32_t color;
};

class CSkillAction {
public:
  virtual bool IsSupport() {
    return false;
  }
  virtual int GetSkillCastMotion() {
    return 8;
  }
  virtual int GetSkillAttackMotion() {
    return 7;
  }
  virtual bool KeepSkillAttackMotion(int argument) {
    return false;
  }
  virtual bool IsSkillWithoutMotion() {
    return false;
  }
  virtual bool KeepSkillAttackSpeed() {
    return false;
  }
  virtual bool SuppressSupportSkillArrow() {
    return false;
  }
  virtual bool SuppressAttackSkillArrow() {
    return false;
  }
  virtual void OnSkillCast(int level, CGameActor* source) {
  }
  virtual void OnTargetSkillCast(int argument1, int argument2, int argument3) {
  }
  virtual void OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) {
  }
  virtual void OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  }
  virtual void OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  }
  virtual bool UseGroundSkillLastFrame() {
    return false;
  }
  virtual int GetSkillScopeType() {
    return 0;
  }
  virtual void OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  }
  virtual void OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  }
};

class CEfstAction {
public:
  virtual void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  }
  virtual void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  }
  virtual void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  }
  virtual EfstActionMessage GetEfstBeginMessage() {
    return {nullptr, 0xFFFF00};
  }
  virtual EfstActionMessage GetEfstEndMessage() {
    return {nullptr, 0xFF};
  }
};

class CSkillObjectAction {
public:
  virtual void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  }
  virtual void OnSkillObjectRemove(CRenderObject* object) {
  }
};

struct SkillInfoEntry {
  // INLINED
  SkillInfoEntry& operator=(const SkillInfoEntry& other) {
    this->unknown_0000 = other.unknown_0000;
    this->unknown_0004 = other.unknown_0004;
    return *this;
  }

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
  __declspec(noinline) bool IsValid() const;
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

  // INLINED
  __forceinline CSkillActionMgr() = default;
  virtual ~CSkillActionMgr();

  static CSkillActionMgr* GetInstance();

  __declspec(noinline) bool IsSupportAction(int skillId);
  std::pair<bool, int> ActionSlot01(int skillId);
  std::pair<bool, int> ActionSlot02(int skillId);
  int ActionSlot14(int skillId);
  bool ActionSlot03(int skillId, int argument);
  bool ActionSlot04(int skillId);
  EfstActionMessage EfstSlot03(int efstId);
  EfstActionMessage EfstSlot04(int efstId);
  bool HasEfstAction(int efstId);
  bool ActionSlot07(int skillId);
  bool ActionSlot06(int skillId);
  bool ActionSlot13(int skillId);
  bool ActionSlot05(int skillId);
  bool ActionSlot16(int skillId, int level, CGameActor* source, CGameActor* target);
  bool EfstSlot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6);
  bool ActionSlot08(int skillId, int level, CGameActor* source);
  bool ActionSlot09(int skillId, int argument1, int argument2, int argument3);
  bool ActionSlot10(int skillId, int level, CGameActor* source, short column, short row);
  bool EfstSlot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor);
  bool ActionSlot12(int skillId, int column, int row, int argument2, int argument3);
  bool ActionSlot15(int skillId, int level, CGameActor* source, CGameActor* target);
  bool EfstSlot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor);
  bool ObjectSlot01(int skillId, CRenderObject* object);
  bool ObjectSlot00(int skillId, int argument, CRenderObject* object, CGameActor* source);
  bool ActionSlot11(int skillId, int level, CGameActor* source, CGameActor* target);
  void RegisterEfstAction(int efstId, std::function<CEfstAction*()> factory);
  void RegisterSkillObjectAction(int skillId, std::function<CSkillObjectAction*()> factory);
  void RegisterSkillAction(int skillId, std::function<CSkillAction*()> factory);
};

extern CSkillActionMgr* g_skillActionMgr;

void __stdcall Sub7739F0();
bool IsOptionalSelfTargetSkill(int skillId);
bool HidesUseFailureMessage(int skillId);
bool IsSupportSkill(int skillId);
bool ShowsCastEffect(int skillId);
vector3d CellWorldPosition(int column, int row);
static_assert(sizeof(SkillInfoEntry) == 8);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SkillInfoEntry, unknown_0004) == 0x4);
static_assert(sizeof(CSkillInfo) == 68);
static_assert(offsetof(CSkillActionMgr, m_efstActions) == 0x0C);
static_assert(offsetof(CSkillActionMgr, m_skillObjectActions) == 0x14);
static_assert(sizeof(CSkillActionMgr) == 0x1C);
static_assert(sizeof(CSkillAction) == 4);
static_assert(sizeof(CEfstAction) == 4);
static_assert(sizeof(CSkillObjectAction) == 4);
static_assert(sizeof(EfstActionMessage) == 8);
#endif

#endif
