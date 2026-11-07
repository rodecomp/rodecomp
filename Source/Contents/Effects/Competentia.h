#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMPETENTIA_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMPETENTIA_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCompetentia : public CEffect {
public:
  CCompetentia();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetLifeTime(int argument) override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCompetentia) == 0xD8);
static_assert(offsetof(CCompetentia, particles) == 0xCC);
#endif

#endif
