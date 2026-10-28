#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H

#include "Contents/Skills/PlayerSkillBundle.h"

class UIWindow;

struct SkillWindows {
  UIWindow*& first;
  UIWindow*& second;
  WindowManagerWindows& windows;

  void Notify();
};

struct SkillUseLevels {
  const std::vector<int>& use;
  const std::vector<int>& additional;
  const std::vector<int>& homunculus;

  int Lookup(int id) const;
};

struct CompanionSkills {
  std::list<CSkillInfo>& skills;
  UIWindow*& window;
  UIWindow*& sharedWindow;

  bool Update(const CSkillInfo& value, unsigned char notify);
  void Notify();
};

struct ActiveSkillEffect {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint32_t unknown_0010;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint32_t unknown_0018;
};

struct ClientSkills {
  CPlayerSkillBundle& bundle;
  const std::list<CSkillInfo>& firstRange;
  const std::list<CSkillInfo>& otherRanges;
  std::list<CSkillInfo>& additional;
  const int& classMode;
  const std::vector<ActiveSkillEffect>& effects;

  bool HasEffect(std::uint32_t id) const;
  CSkillInfo LookupFirst(std::uint32_t id) const;
  CSkillInfo LookupOther(std::uint32_t id) const;
  CSkillInfo LookupAdditional(std::uint32_t id) const;
  CSkillInfo Lookup(std::uint32_t id) const;

  bool UpdateAdditional(const CSkillInfo& value, int unused);

  void Remove(std::uint32_t id, SkillWindows& windows);

  bool Update(
    const CSkillInfo& value, unsigned char notify, unsigned char updateUnknown000C, LuaScript& script, const SkillUseLevels& levels, SkillWindows& windows
  );
};

static_assert(sizeof(ActiveSkillEffect) == 28);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ActiveSkillEffect, unknown_0018) == 0x18);
#endif

float GetSkillFactor(int job);

#endif
