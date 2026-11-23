#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_COMPETENTIA_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_COMPETENTIA_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CCompetentia : public CEffect {
public:
  CCompetentia();
  ~CCompetentia() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void SetLifeTime(int argument) override;

  /* 00CC */ std::vector<CEZ2STRParticle*> particles;

private:
  void UpdateParticle(CEZ2STRParticle* particle);
  void RenderParticle(CEZ2STRParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCompetentia) == 0xD8);
static_assert(offsetof(CCompetentia, particles) == 0xCC);
#endif

#endif
