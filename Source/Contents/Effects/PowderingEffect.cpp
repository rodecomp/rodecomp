#include "Contents/Effects/PowderingEffect.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/MapSkyMgr.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00708E70
// FUNCTION: fn_07085
CPowderingEffect::CPowderingEffect() : CEffect(-1), unknown_00E4(0) {
}

// OFFSET: 0x007092C0
// FUNCTION: fn_07088
void CPowderingEffect::Close() {
  for (auto it = this->smokeParticles.begin(); it != this->smokeParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->smokeParticles.clear();

  for (auto it = this->spriteParticles.begin(); it != this->spriteParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->spriteParticles.clear();
}

// OFFSET: 0x00709330
// FUNCTION: fn_07089
void CPowderingEffect::Slot01() {
  if (this->unknown_00B0 == 1 || this->unknown_00B0 == 2) {
    this->unknown_00E4 = 3;
  } else if (this->unknown_00B0 == 3 || this->unknown_00B0 == 4) {
    this->unknown_00E4 = 5;
  } else if (this->unknown_00B0 == 5) {
    this->unknown_00E4 = 7;
  }

  const int smokeCount = this->unknown_00E4 * this->unknown_00E4;
  for (int i = 0; i < smokeCount; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetTexture("smoke2.bmp");

    const int span1 = this->unknown_00E4 * 5;
    int r1 = 0;
    if (span1 != 0) {
      r1 = std::max(rand() % span1, 0);
    }
    const int r1Final = r1 - span1 / 2 + 1;

    const int span2 = this->unknown_00E4 * 5;
    int r2 = 0;
    if (span2 != 0) {
      r2 = std::max(rand() % span2, 0);
    }
    const int r2Final = r2 - span2 / 2 + 1;

    particle->unknown_00D4 = 0.0f;
    particle->unknown_00D0 = static_cast<float>(r1Final);
    particle->unknown_00D8 = static_cast<float>(r2Final);

    this->sub_7096B0(particle);
    InitParticleQuad(particle->vertices);
    this->smokeParticles.push_back(particle);
  }

  const int spriteCount = this->unknown_00E4 * this->unknown_00E4;
  for (int i = 0; i < spriteCount; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(
      CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1)
    );
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetSprite("이팩트\\particle2.spr");
    particle->SetAction("이팩트\\particle2.act");

    this->sub_709570(particle);
    InitParticleQuad(particle->vertices);
    this->spriteParticles.push_back(particle);
  }
}

// OFFSET: 0x00709570
// FUNCTION: fn_07090
void CPowderingEffect::sub_709570(CParticle* particle) {
  CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);

  const int span1 = this->unknown_00E4 * 5;
  int r1 = 0;
  if (span1 != 0) {
    r1 = std::max(rand() % span1, 0);
  }
  const int r1Final = r1 - span1 / 2 + 1;

  const int yVal = std::max(rand() % 7, 0) + 7;

  const int span2 = this->unknown_00E4 * 5;
  int r2 = 0;
  if (span2 != 0) {
    r2 = std::max(rand() % span2, 0);
  }
  const int r2Final = r2 - span2 / 2 + 1;

  spriteParticle->velocity.y = -static_cast<float>(yVal);
  spriteParticle->velocity.x = static_cast<float>(r1Final);
  spriteParticle->velocity.z = static_cast<float>(r2Final);

  const int r3 = std::max(rand() % 3, 0) + 1;

  spriteParticle->alpha = 0;
  spriteParticle->unknown_00A8 = 0;
  spriteParticle->scale = g_depthAverageHalf - static_cast<float>(r3) * 0.1f;
}

// OFFSET: 0x00709690
// FUNCTION: fn_07091
void CPowderingEffect::Slot12() {
  if (this->id == 1104) {
    this->AddEvent(0, "effect\\su_cn_powdering.wav", 0, 0);
  }
}
