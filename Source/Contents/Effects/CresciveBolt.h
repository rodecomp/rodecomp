#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CRESCIVEBOLT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CRESCIVEBOLT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CCresciveBolt : public CEffect {
public:
  CCresciveBolt();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ CEZ2STRParticle* particle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCresciveBolt) == 0xD0);
static_assert(offsetof(CCresciveBolt, particle) == 0xCC);
#endif

#endif
