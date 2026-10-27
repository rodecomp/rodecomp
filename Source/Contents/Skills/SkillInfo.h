#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLINFO_H

#include <cstddef>
#include <cstdint>
#include <vector>

class LuaScript;
struct NormalItemSlot;
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

static_assert(sizeof(SkillInfoEntry) == 8);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SkillInfoEntry, unknown_0004) == 0x4);
static_assert(sizeof(CSkillInfo) == 68);
#endif

#endif
