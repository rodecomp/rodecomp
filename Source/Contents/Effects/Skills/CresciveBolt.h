#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CRESCIVEBOLT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CRESCIVEBOLT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>

class CEZ2STRParticle;

class CCresciveBolt : public CEffect {
public:
  CCresciveBolt();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ CEZ2STRParticle* particle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCresciveBolt) == 0xD0);
static_assert(offsetof(CCresciveBolt, particle) == 0xCC);
#endif

#endif
