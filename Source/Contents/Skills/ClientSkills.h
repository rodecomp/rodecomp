#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H

#include "Contents/Skills/PlayerSkillBundle.h"

struct ActiveSkillEffect {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint32_t unknown_0010;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint32_t unknown_0018;
};

extern std::vector<ActiveSkillEffect> g_activeSkillEffects;

static_assert(sizeof(ActiveSkillEffect) == 28);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ActiveSkillEffect, unknown_0018) == 0x18);
#endif

float GetSkillFactor(int job);

#endif
