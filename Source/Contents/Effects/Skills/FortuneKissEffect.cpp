#include "Contents/Effects/Skills/FortuneKissEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00701520
// FUNCTION: fn_06994
void CFortuneKiss::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00701560
// FUNCTION: fn_06995
void CFortuneKiss::Open() {
  CParticle* particle = CreateParticle(&this->positionVector, 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\kiss.bmp");
  particle->effectId = this->id;
  particle->columnIndex = 0;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  particle->alpha = 255;
  particle->angle = 0;
  particle->phaseParameter = 0;
  particle->scale = 1.0f;
  const int drift = rand() % 5;
  float* values = &particle->animationParameter;
  values[1] = 0.0f;
  values[0] = -5.0f - static_cast<float>(drift);
  particle->InitParticleQuad(particle->vertices);
  this->angle = rand() % 359;
  this->particles.push_back(particle);
}

// OFFSET: 0x00701700
// FUNCTION: fn_06996
bool CFortuneKiss::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00701710
// FUNCTION: fn_06997
void CFortuneKiss::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      ++this->angle;
      if (this->angle > 360) {
        this->angle = 0;
      }

      float& offset = particle->animationParameter;
      float& width = particle->animationParameters[0];
      const float cosine = g_cosTable[this->angle];
      const float newOffset = offset - cosine * 0.05;
      const float newWidth = cosine * 0.02 + width;
      offset = newOffset;
      width = newWidth;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.AppendXRotation(3.141592f);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(newWidth + 10.0f, 20.0f, 1.0f);
      matrix translate;
      translate.IsIdentity();
      translate.MakeTranslate(0.0f, newOffset, 0.0f);
      matrix result;
      result.Compose(scale, rotation);
      matrix first = result;
      result.Compose(first, translate);
      particle->transform = result;
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00701B10
// FUNCTION: fn_06998
void CFortuneKiss::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
