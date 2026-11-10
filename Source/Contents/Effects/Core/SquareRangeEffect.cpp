#include "Contents/Effects/Core/SquareRangeEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4C5E0
// FUNCTION: fn_18960
CSquareRangeEffect::CSquareRangeEffect() : CEffect(-1), size(1.0f), resize(0), particles() {
}

// OFFSET: 0x00B4C750
// FUNCTION: fn_18962
void CSquareRangeEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B4C790
// FUNCTION: fn_18963
void CSquareRangeEffect::Open() {
  if (this->id == 991) {
    this->size = 7.0f;
  }

  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->alpha = 255;
  particle->SetColor(255, 255, 255);
  particle->SetTexture("effect\\SquareRange.tga");
  particle->scale = this->size * 2.5f;
  if (this->id == 991) {
    this->effectLifetime = 300;
    particle->SetColor(92, 92, 92);
    particle->destinationBlend = 6;
  }

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

// OFFSET: 0x00B4C8E0
// FUNCTION: fn_18965
void CSquareRangeEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (this->resize) {
        particle->scale = this->size * 2.5f;
        this->resize = 0;
      }

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(1.5707964f);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);
      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B4CB40
// FUNCTION: fn_18966
void CSquareRangeEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
