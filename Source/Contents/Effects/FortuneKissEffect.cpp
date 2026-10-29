#include "Contents/Effects/FortuneKissEffect.h"

#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

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
