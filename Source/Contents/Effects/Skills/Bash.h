#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BASH_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BASH_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CBash : public CEffect {
public:
  CBash();
  ~CBash() override;
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ std::vector<CParticle*> rays;
  /* 00D8 */ std::vector<CParticle*> sparks;
  /* 00E4 */ float screenX;
  /* 00E8 */ float screenY;

private:
  void CreateRays();
  void CreateSparks();
  void UpdateRay(CParticle* particle);
  void UpdateSpark(CParticle* particle);
  void RenderRay(CParticle* particle);
  void RenderSpark(CParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CBash, rays) == 0xCC);
static_assert(offsetof(CBash, sparks) == 0xD8);
static_assert(offsetof(CBash, screenY) == 0xE8);
static_assert(sizeof(CBash) == 0xEC);
#endif

#endif
