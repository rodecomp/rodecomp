#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CANEOFEVILEYEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CANEOFEVILEYEEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/Particle.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class CCaneOfEvilEyeEffect : public CEffect {
public:
  CCaneOfEvilEyeEffect();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  CEZ2STRParticle* CreateEye(int type, const float* position);
  CEZ2STRParticle* CreateLight(int type, const float* position);
  void SpawnLights();
  void RemoveNullEmitters();
  void Finish();
  void UpdateEyes();
  void UpdateLights();

  /* 00CC */ std::uint32_t startTime;
  /* 00D0 */ std::vector<CEZ2STRParticle*> eyes;
  /* 00DC */ std::vector<CEZ2STRParticle*> lights;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCaneOfEvilEyeEffect) == 0xE8);
static_assert(offsetof(CCaneOfEvilEyeEffect, eyes) == 0xD0);
static_assert(offsetof(CCaneOfEvilEyeEffect, lights) == 0xDC);
#endif

#endif
