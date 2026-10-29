#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARACTERPARTICLEBUNDLE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CHARACTERPARTICLEBUNDLE_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CharacterParticleBundle : public CEffect {
public:
  CharacterParticleBundle();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  ~CharacterParticleBundle() override;
  virtual void Slot34() = 0;
  virtual void Slot35();
  virtual void Slot36();

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
};

class CChopChopClawMarks : public CharacterParticleBundle {
public:
  bool Slot06() override;
  ~CChopChopClawMarks() override;
  void Slot34() override;
  void Slot35() override;
  void Slot36() override;

  /* 00E8 */ int unknown_00E8 = 0;
  /* 00EC */ int unknown_00EC = 0;
  /* 00F0 */ int unknown_00F0[2];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CharacterParticleBundle, unknown_00E4) == 0xE4);
static_assert(sizeof(CChopChopClawMarks) == 0xF8);
#endif

#endif
