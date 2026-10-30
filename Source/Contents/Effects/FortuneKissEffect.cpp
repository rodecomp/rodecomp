#include "Contents/Effects/FortuneKissEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00701520
// FUNCTION: fn_06994
void CFortuneKiss::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00701560
// FUNCTION: fn_06995
void CFortuneKiss::Slot01() {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\kiss.bmp");
  particle->unknown_0004 = this->id;
  particle->unknown_000C = 0;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  particle->alpha = 255;
  particle->unknown_00C4 = 0;
  particle->unknown_004C = 0;
  particle->scale = 1.0f;
  particle->unknown_0054 = -5.0f - static_cast<float>(rand() % 5);
  *reinterpret_cast<float*>(particle->unknown_0058) = 0.0f;
  InitParticleQuad(particle->vertices);
  this->angle = rand() % 359;
  this->particles.push_back(particle);
}

// OFFSET: 0x00701700
// FUNCTION: fn_06996
bool CFortuneKiss::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00701710
// FUNCTION: fn_06997
void CFortuneKiss::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      ++this->angle;
      if (this->angle > 360) {
        this->angle = 0;
      }

      float& offset = *reinterpret_cast<float*>(&particle->unknown_0048[0xC]);
      float& width = *reinterpret_cast<float*>(&particle->unknown_0048[0x10]);
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
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00701B10
// FUNCTION: fn_06998
void CFortuneKiss::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
