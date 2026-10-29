#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLEPOOL_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTICLEPOOL_H

#include <cstddef>
#include <map>

class CParticle;

class ParticlePool {
public:
  ParticlePool();
  ~ParticlePool();

  CParticle* Acquire();
  void Clear();
  void Release(CParticle* particle);
  bool IsActive(CParticle* particle);
  void ActivateAll();
  void Add(CParticle* particle);

  /* 0000 */ std::map<CParticle*, bool> particles;
  /* 0008 */ std::size_t active;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ParticlePool) == 0xC);
static_assert(offsetof(ParticlePool, active) == 0x8);
#endif

#endif
