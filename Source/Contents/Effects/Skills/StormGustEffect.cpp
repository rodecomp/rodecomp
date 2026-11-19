#include "Contents/Effects/Skills/StormGustEffect.h"
#include "UI/GameSettings/TalkType.h"

#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B7A8C0
CStormGustEffect::CStormGustEffect() : CEffect(EF_STORMGUST) {
}

// OFFSET: 0x00B7A940
__declspec(noinline) CStormGustEffect::~CStormGustEffect() {
}

// OFFSET: 0x00B7AA20
// FUNCTION: fn_19211
void CStormGustEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B7AC50
__forceinline void CStormGustEffect::SetupParticle(CParticle* particle) {
  if (particle) {
    particle->SetTexture("effect\\ice.tga");
    particle->alpha = 250;

    const int speedRoll = rand() % 25;
    const float speed = static_cast<float>((speedRoll >= 0 ? speedRoll : 0) + 3);
    const int angleRoll = rand() % 360;
    const float angle = static_cast<float>(angleRoll >= 0 ? angleRoll : 0);
    vector3d velocity;
    velocity.z = ClientCosine(static_cast<int>(angle)) * -speed;
    velocity.x = ClientSine(static_cast<int>(angle)) * speed;
    velocity.y = 20.0f;
    particle->offset = velocity;

    const int scaleRoll = rand() % 100;
    particle->speedParameter = 1.5f;
    particle->scale = static_cast<float>((scaleRoll >= 0 ? scaleRoll : 0) + 400) / 100.0f;

    matrix& rotation = particle->rotation;
    rotation.IsIdentity();
    rotation.MakeXRotation(100.0f / 180.0f * 3.141592f);
    const int yawRoll = rand() % 360;
    rotation.AppendYRotation(static_cast<float>(yawRoll >= 0 ? yawRoll : 0) / 180.0f * 3.141592f);
  }
}

// OFFSET: 0x00B7AA60
// FUNCTION: fn_19212
void CStormGustEffect::Open() {
  if (g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
    return;
  }

  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 1);
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = index;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    this->SetupParticle(particle);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B7ADA0
// FUNCTION: fn_19213
bool CStormGustEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B7B2E0
__forceinline void CStormGustEffect::UpdateAlpha(CParticle* particle) {
  if (particle) {
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
}

// OFFSET: 0x00B7B020
__forceinline void CStormGustEffect::UpdateParticle(CParticle* particle) {
  if (particle) {
    if (particle->elapsedFrames == 3) {
      particle->speedParameter = 0.0f;
    }

    const float speed = particle->speedParameter;
    const matrix& rotation = particle->rotation;
    const float* values = rotation.values;
    const float moveX = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9];
    const float moveY = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10];
    const float moveZ = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11];
    const float offsetX = particle->offsetX + moveX;
    const float offsetY = particle->offsetY + moveY;
    const float offsetZ = particle->offsetZ + moveZ;
    particle->offsetX = offsetX;
    particle->offsetY = offsetY;
    particle->offsetZ = offsetZ;

    matrix translation;
    std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
    translation.MakeTranslate(offsetX, offsetY, offsetZ);
    matrix scale;
    std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
    scale.SetScale(particle->scale, particle->scale, 1.0f);
    matrix transform;
    transform.Compose(scale, translation);
    particle->transform = transform;

    this->UpdateAlpha(particle);
  }
}

// OFFSET: 0x00B7ADB0
// FUNCTION: fn_19214
void CStormGustEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    this->UpdateParticle(particle);

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B7B2A0
// FUNCTION: fn_46038
__forceinline void CStormGustEffect::RenderParticle(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B7B240
// FUNCTION: fn_19215
void CStormGustEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->RenderParticle(particle);
  }
}
