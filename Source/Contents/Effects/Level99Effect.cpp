#include "Contents/Effects/Level99Effect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B6A0A0
// FUNCTION: fn_19079
CLevel99Effect::CLevel99Effect() : CEffect(200) {
}

// OFFSET: 0x00B6A250
// FUNCTION: fn_19081
void CLevel99Effect::Slot08(int red, int green, int blue) {
  for (CParticle* sparkle : this->sparkles) {
    sparkle->SetColor(red, green, blue);
  }
  for (CParticle* sprite : this->sprites) {
    sprite->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B6AA60
// FUNCTION: fn_19084
void CLevel99Effect::Close() {
  for (CParticle* sparkle : this->sparkles) {
    FreeParticle(sparkle);
  }
  this->sparkles.clear();

  for (CParticle* sprite : this->sprites) {
    FreeParticle(sprite);
  }
  this->sprites.clear();
}

// OFFSET: 0x00B6ABB0
// FUNCTION: fn_19086
bool CLevel99Effect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B6B0E0
// FUNCTION: fn_19088
void CLevel99Effect::Slot04() {
  for (CParticle* sparkle : this->sparkles) {
    if (sparkle) {
      sparkle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (CParticle* sprite : this->sprites) {
    if (sprite && sprite->unknown_000C > 0) {
      const int frame = sprite->unknown_000C;
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(frame - 1) / 21.0f;
      quad.right = static_cast<float>(frame) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      sprite->Render(quad);
    }
  }
}
