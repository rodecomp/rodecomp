#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMPETENTIA_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMPETENTIA_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CCompetentia : public CEffect {
public:
  CCompetentia();

  bool Slot06() override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCompetentia) == 0xD8);
static_assert(offsetof(CCompetentia, particles) == 0xCC);
#endif

#endif
