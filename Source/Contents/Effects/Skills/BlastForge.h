#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BLASTFORGE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BLASTFORGE_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/Particle.h"

#include <cstddef>
#include <cstdint>

class CBlastForge : public CEffect {
public:
  CBlastForge();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void RestartBlastParticles();
  void ProcessBlastParticles();

  /* 00CC */ CEZ2STRParticle* loopParticle;
  /* 00D0 */ CEZ2STRParticle* loopBottomParticle;
  /* 00D4 */ CEZ2STRParticle* blastParticle;
  /* 00D8 */ CEZ2STRParticle* blastBottomParticle;
  /* 00DC */ unsigned char openToggle;
  /* 00DD */ unsigned char padding_00DD[3];
  /* 00E0 */ std::int64_t startTime;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBlastForge) == 0xE8);
static_assert(offsetof(CBlastForge, openToggle) == 0xDC);
static_assert(offsetof(CBlastForge, startTime) == 0xE0);
#endif

#endif
