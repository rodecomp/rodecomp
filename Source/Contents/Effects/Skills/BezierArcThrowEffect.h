#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BEZIERARCTHROWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_BEZIERARCTHROWEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <string>

class CBezierArcThrowEffect : public CEffect {
public:
  // INLINED
  __forceinline explicit CBezierArcThrowEffect(const std::string& name) : CEffect(-1), m_name(name), m_target() {}

  /* 00CC */ bool m_active;
  /* 00D0 */ float m_progress;
  /* 00D4 */ CEZ2STRParticle* m_particle;
  /* 00D8 */ std::string m_name;
  /* 00F0 */ vector3d m_target;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override { return this->m_active; }

  void SetTarget(const vector3d& target);
};

class CThunderingOrb : public CBezierArcThrowEffect {
public:
  explicit CThunderingOrb(const std::string& name);
};

class CWindBomb : public CBezierArcThrowEffect {
public:
  explicit CWindBomb(const std::string& name);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBezierArcThrowEffect) == 0xFC);
static_assert(offsetof(CBezierArcThrowEffect, m_active) == 0xCC);
static_assert(offsetof(CBezierArcThrowEffect, m_particle) == 0xD4);
static_assert(offsetof(CBezierArcThrowEffect, m_name) == 0xD8);
static_assert(offsetof(CBezierArcThrowEffect, m_target) == 0xF0);
#endif

#endif
