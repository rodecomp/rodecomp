#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EZ2STREFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/Particle.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CEZ2STRScreenEffect : public CEffect {
public:
  void Slot00(int argument) override;
  void Slot04() override;
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
  void Slot29(int id) override;

  /* 00CC */ CEZ2STRParticle* particle;
  /* 00D0 */ std::string name;
  /* 00E8 */ unsigned char fadePulse;
  /* 00EC */ std::uint32_t overlayColor;
  /* 00F0 */ unsigned char followCamera;
  /* 00F8 */ double cameraAngle;
  /* 0100 */ unsigned char jobScaled;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CEZ2STRScreenEffect, particle) == 0xCC);
static_assert(offsetof(CEZ2STRScreenEffect, name) == 0xD0);
static_assert(offsetof(CEZ2STRScreenEffect, overlayColor) == 0xEC);
static_assert(offsetof(CEZ2STRScreenEffect, cameraAngle) == 0xF8);
static_assert(sizeof(CEZ2STRScreenEffect) == 0x108);
#endif

#endif
