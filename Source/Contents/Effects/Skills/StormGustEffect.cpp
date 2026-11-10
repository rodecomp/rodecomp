#include "Contents/Effects/Skills/StormGustEffect.h"

#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B7AA20
// FUNCTION: fn_19211
void CStormGustEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B7AA60
// FUNCTION: fn_19212
void CStormGustEffect::Open() {
  if (g_serviceOptions.Get(229)) {
    return;
  }

  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = index;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->SetTexture("effect\\ice.tga");
    particle->alpha = 250;

    const float speed = static_cast<float>(std::max(0, rand() % 25) + 3);
    const float angle = static_cast<float>(std::max(0, rand() % 360));
    const float velocityZ = ClientCosine(static_cast<int>(angle)) * -speed;
    const float velocityX = ClientSine(static_cast<int>(angle)) * speed;
    particle->offsetX = velocityX;
    particle->offsetY = 20.0f;
    particle->offsetZ = velocityZ;

    particle->speedParameter = 1.5f;
    particle->scale = static_cast<float>(std::max(0, rand() % 100) + 400) / 100.0f;

    matrix& rotation = *reinterpret_cast<matrix*>(particle->rotationStorage);
    rotation.IsIdentity();
    rotation.MakeXRotation(100.0f / 180.0f * 3.141592f);
    rotation.AppendYRotation(static_cast<float>(std::max(0, rand() % 360)) / 180.0f * 3.141592f);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B7ADA0
// FUNCTION: fn_19213
bool CStormGustEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B7ADB0
// FUNCTION: fn_19214
void CStormGustEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    CParticle* particle = *it;
    if (particle) {
      if (particle->elapsedFrames == 3) {
        particle->speedParameter = 0.0f;
      }

      const float speed = particle->speedParameter;
      const matrix& rotation = *reinterpret_cast<const matrix*>(particle->rotationStorage);
      const float* values = rotation.values;
      const float moveX = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9];
      const float moveY = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10];
      const float moveZ = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11];
      particle->offsetX += moveX;
      particle->offsetY += moveY;
      particle->offsetZ += moveZ;

      matrix translation;
      std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
      translation.MakeTranslate(particle->offsetX, particle->offsetY, particle->offsetZ);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);
      matrix transform;
      transform.Compose(scale, translation);
      particle->transform = transform;

      const int remaining = 215 - this->frame;
      float fade = 0.0f;
      if (this->frame == remaining - 10) {
        fade = -(particle->alpha / (remaining - this->frame));
      }

      int alpha = particle->alpha + static_cast<int>(fade);
      if (alpha < 0) {
        alpha = 0;
      }
      if (alpha > 254) {
        alpha = 254;
      }
      particle->alpha = alpha;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B7B240
// FUNCTION: fn_19215
void CStormGustEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
