#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_WHIRLWINDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_WHIRLWINDEFFECT_H

#include "Contents/Effects/RagEffect.h"

#include <chrono>
#include <cstddef>

struct CWhirlwindEffect : CRagEffect {
  CWhirlwindEffect() : duration(250) {
  }

  /* 11CA8 */ int duration;
  /* 11CAC */ int unknown_11CAC;
  /* 11CB0 */ std::chrono::steady_clock::time_point endTime;

  bool OnProcess() override;

  void SetLifetime(int milliseconds);
  void SetDuration(int duration);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWhirlwindEffect) == 0x11CB8);
static_assert(offsetof(CWhirlwindEffect, duration) == 0x11CA8);
static_assert(offsetof(CWhirlwindEffect, endTime) == 0x11CB0);
#endif

#endif
