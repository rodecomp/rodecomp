#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SKILL_H

#include "Contents/Actors/GameActor.h"

#include <cstddef>
#include <cstdint>

class CSkill : public CGameActor {
public:
  /* 0410 */ unsigned char unknown_0410[0x08];
  /* 0418 */ std::uint32_t skillAid;
  /* 041C */ unsigned char unknown_041C[0x10];
  /* 042C */ int range;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CSkill, skillAid) == 0x418);
static_assert(offsetof(CSkill, range) == 0x42C);
#endif

#endif
