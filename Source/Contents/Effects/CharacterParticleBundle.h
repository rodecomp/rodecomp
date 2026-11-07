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

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  virtual bool IsAlive() = 0;
  ~CharacterParticleBundle() override;
  virtual void EmitParticles() = 0;
  virtual void OnInit();
  virtual void UpdateParticlePosition(CEZ2STRParticle* particle) {
  }
  virtual void ProcessParticle(CEZ2STRParticle* particle);
  virtual void RenderParticle(CEZ2STRParticle* particle);

  /* 00CC */ int unknown_00CC;
  /* 00D0 */ long long startTime;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ std::vector<CEZ2STRParticle*> particles;
};

class CChopChopClawMarks : public CharacterParticleBundle {
public:
  bool IsAlive() override;
  ~CChopChopClawMarks() override;
  void EmitParticles() override;
  void OnInit() override;
  void UpdateParticlePosition(CEZ2STRParticle* particle) override;

  const std::vector<RadialGridPoint>& GetRing(int radius);
  void SpawnParticle();

  /* 00E8 */ long long lastSpawnTime = 0;
  /* 00F0 */ float offsetX;
  /* 00F4 */ float offsetY;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CharacterParticleBundle, startTime) == 0xD0);
static_assert(offsetof(CharacterParticleBundle, particles) == 0xDC);
static_assert(offsetof(CChopChopClawMarks, lastSpawnTime) == 0xE8);
static_assert(offsetof(CChopChopClawMarks, offsetY) == 0xF4);
static_assert(sizeof(CChopChopClawMarks) == 0xF8);
#endif

#endif
