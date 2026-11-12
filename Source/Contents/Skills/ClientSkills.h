#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_CLIENTSKILLS_H

#include "Contents/Skills/PlayerSkillBundle.h"

struct ActiveSkillEffect {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint32_t expiration;
  /* 0008 */ std::uint32_t value1;
  /* 000C */ std::uint32_t value2;
  /* 0010 */ std::uint32_t value3;
  /* 0014 */ std::uint32_t totalDuration;
  /* 0018 */ std::uint32_t remainingDuration;
};

extern std::vector<ActiveSkillEffect> g_activeSkillEffects;

static_assert(sizeof(ActiveSkillEffect) == 28);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ActiveSkillEffect, remainingDuration) == 0x18);
#endif

void InitMapTypeInfoTable();
float GetSkillFactor(int job);

#endif
