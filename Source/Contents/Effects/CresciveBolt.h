#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CRESCIVEBOLT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CRESCIVEBOLT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CParticle;

class CCresciveBolt : public CEffect {
public:
  CCresciveBolt();

  /* 00CC */ CParticle* particle;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCresciveBolt) == 0xD0);
static_assert(offsetof(CCresciveBolt, particle) == 0xCC);
#endif

#endif
