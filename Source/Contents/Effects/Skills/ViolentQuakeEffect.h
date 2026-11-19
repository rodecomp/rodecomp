#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_VIOLENTQUAKEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_VIOLENTQUAKEEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CViolentQuakeEffect : public CEffect {
public:
  CViolentQuakeEffect();
  ~CViolentQuakeEffect() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void InitAllBloom();

  /* 00CC */ std::vector<CEZ2STRParticle*> emitters;

private:
  void InitViolentQuake();
  void ProcessEmitter(CEZ2STRParticle* emitter);
  void RenderEmitter(CEZ2STRParticle* emitter);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CViolentQuakeEffect) == 0xD8);
#endif

#endif
