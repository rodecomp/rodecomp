#include "Contents/Effects/Core/SquareRangeEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4C5E0
// FUNCTION: fn_18960
CSquareRangeEffect::CSquareRangeEffect() : CEffect(-1), size(1.0f), resize(0), particles() {
}

// OFFSET: 0x00B4C670
// FUNCTION: fn_46220
__declspec(noinline) CSquareRangeEffect::~CSquareRangeEffect() {
}

// OFFSET: 0x00B4C750
// FUNCTION: fn_18962
void CSquareRangeEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B4C860
// FUNCTION: fn_46221
__forceinline void CSquareRangeEffect::SetParticleStyle(CParticle* particle) {
  if (this->id == EF_KG_KAGEHUMI) {
    this->effectLifetime = 300;
    particle->SetColor(92, 92, 92);
    particle->destinationBlend = 6;
  }
}

// OFFSET: 0x00B4C8A0
// FUNCTION: fn_46222
__forceinline void CSquareRangeEffect::SetInitialSize() {
  if (this->id == EF_KG_KAGEHUMI) {
    this->size = 7.0f;
  }
}

// OFFSET: 0x00B4C790
// FUNCTION: fn_18963
void CSquareRangeEffect::Open() {
  this->SetInitialSize();

  CParticle* particle = CreateParticle(&this->positionVector, 5);
  if (!particle) {
    return;
  }

  particle->alpha = 255;
  particle->SetColor(255, 255, 255);
  particle->SetTexture("effect\\SquareRange.tga");
  particle->scale = this->size * 2.5f;
  this->SetParticleStyle(particle);

  this->particles.push_back(particle);
}

// OFFSET: 0x00B4C8C0
// FUNCTION: fn_18964
bool CSquareRangeEffect::IsAlive() {
  if (this->id == 991) {
    return !this->IsFinished();
  }

  return true;
}

// OFFSET: 0x00B4CA30
// FUNCTION: fn_46223
__forceinline void CSquareRangeEffect::UpdateParticle(CParticle* particle) {
  if (particle) {
    if (this->resize) {
      particle->scale = this->size * 2.5f;
      this->resize = 0;
    }

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeXRotation(1.570796f);
    matrix scale;
    std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
    scale.SetScale(particle->scale, particle->scale, 1.0f);
    matrix transform;
    transform.Compose(scale, rotation);
    particle->transform = transform;
  }
}

// OFFSET: 0x00B4C8E0
// FUNCTION: fn_18965
void CSquareRangeEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    this->UpdateParticle(particle);

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B4CBA0
// FUNCTION: fn_46224
__forceinline void CSquareRangeEffect::RenderParticle(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B4CB40
// FUNCTION: fn_18966
void CSquareRangeEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderParticle(*it);
  }
}
