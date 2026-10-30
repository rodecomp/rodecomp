#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CGameActor;

class CEZ2STRScreenEffect : public CEffect {
public:
  CEZ2STRScreenEffect();
  void Slot00(int argument) override;
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot13(int argument) override;
  void Slot14(int argument) override;
  void Slot15(int argument) override;
  void Slot16(int argument) override;
  void Slot17(float argument) override;
  void Slot20(int argument) override;
  void Slot21(int argument) override;
  void Slot22(float argument) override;
  void Slot23(float argument) override;
  void Slot24(int argument1, int argument2, int argument3, int argument4, int argument5) override;
  vector3d Slot25() override;
  int Slot28() override;
  void Slot29(int id) override;
  bool Slot33() override;
  virtual void Slot34(CEZ2STRParticle* particle, int id);

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
