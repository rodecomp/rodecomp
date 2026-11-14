#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLES_EZ2STREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLES_EZ2STREFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CGameActor;

class CEZ2STRScreenEffect : public CEffect {
public:
  CEZ2STRScreenEffect();
  void SetRenderFunc(ParticleSubmit argument) override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void InitEvents() override;
  void AdvanceFrame(float speed) override;
  void SetLifeTime(int argument) override;
  void SetRepeat(int argument) override;
  void SetDelay(int argument) override;
  void SetAngle(float argument) override;
  void SetResource(int argument) override;
  void SetFlags(int argument) override;
  void SetScale(float argument) override;
  void SetAlpha(float argument) override;
  void SetWindowOffset(int argument1, int argument2, int argument3, int argument4, int argument5) override;
  vector3d GetWindowOffset() override;
  int GetFrame() override;
  void SetId(int id) override;
  bool IsVisible() override;
  virtual void InitParticle(CEZ2STRParticle* particle, int id);

  bool ApplyHeadOffset(CEZ2STRParticle* particle);
  bool IsAttachedToHead() const;

  /* 00CC */ CEZ2STRParticle* particle;
  /* 00D0 */ std::string name;
  /* 00E8 */ unsigned char fadePulse;
  /* 00EC */ std::uint32_t overlayColor;
  /* 00F0 */ unsigned char followCamera;
  /* 00F8 */ double cameraAngle;
  /* 0100 */ unsigned char jobScaled;
};

float GetHatEffectOffsetX(int id, const char* name);
int GetHatEffectOffsetY(int id, CGameActor* actor);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEZ2STRScreenEffect, particle) == 0xCC);
static_assert(offsetof(CEZ2STRScreenEffect, name) == 0xD0);
static_assert(offsetof(CEZ2STRScreenEffect, overlayColor) == 0xEC);
static_assert(offsetof(CEZ2STRScreenEffect, cameraAngle) == 0xF8);
static_assert(sizeof(CEZ2STRScreenEffect) == 0x108);
#endif

#endif
