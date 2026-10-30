#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARACTERPARTICLEBUNDLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARACTERPARTICLEBUNDLE_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;
struct RadialGridPoint;

class CharacterParticleBundle : public CEffect {
public:
  CharacterParticleBundle();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  virtual bool Slot06() = 0;
  ~CharacterParticleBundle() override;
  virtual void Slot34() = 0;
  virtual void Slot35();
  virtual void Slot36(CEZ2STRParticle* particle) {
  }
  virtual void Slot37(CEZ2STRParticle* particle);
  virtual void Slot38(CEZ2STRParticle* particle);

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ long long startTime;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ std::vector<CEZ2STRParticle*> particles;
};

class CChopChopClawMarks : public CharacterParticleBundle {
public:
  bool Slot06() override;
  ~CChopChopClawMarks() override;
  void Slot34() override;
  void Slot35() override;
  void Slot36(CEZ2STRParticle* particle) override;

  const std::vector<RadialGridPoint>& GetRing(int radius);

  /* 00E8 */ int unknown_00E8 = 0;
  /* 00EC */ int unknown_00EC = 0;
  /* 00F0 */ float offsetX;
  /* 00F4 */ float offsetY;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CharacterParticleBundle, startTime) == 0xD0);
static_assert(offsetof(CharacterParticleBundle, particles) == 0xDC);
static_assert(offsetof(CChopChopClawMarks, offsetY) == 0xF4);
static_assert(sizeof(CChopChopClawMarks) == 0xF8);
#endif

#endif
