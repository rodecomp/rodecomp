#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_MERCENARYAI_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_MERCENARYAI_H

#include <cstddef>
#include <cstdint>

struct lua_State;

class CMercenaryAI {
public:
  /* 0004 */ lua_State* state;
  /* 0008 */ std::uint32_t actionStarted;
  /* 000C */ std::uint32_t actionDuration;
  /* 0010 */ int kind;
  /* 0014 */ std::uint32_t lastTick;

  virtual ~CMercenaryAI();

  explicit CMercenaryAI(int kind);
  void Shutdown();
};

int ClampTargetAttackRange(std::uint32_t aid, int distance);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMercenaryAI) == 0x18);
static_assert(offsetof(CMercenaryAI, state) == 0x4);
static_assert(offsetof(CMercenaryAI, actionStarted) == 0x8);
static_assert(offsetof(CMercenaryAI, actionDuration) == 0xC);
static_assert(offsetof(CMercenaryAI, kind) == 0x10);
static_assert(offsetof(CMercenaryAI, lastTick) == 0x14);
#endif

#endif
