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

// INLINED
static void SetParticleOffset(CParticle& particle, vector3d offset) {
  particle.offset = offset;
}

// OFFSET: 0x00B7A8C0
// FUNCTION: fn_46034
CStormGustEffect::CStormGustEffect() : CEffect(EF_STORMGUST) {
}

// OFFSET: 0x00B7A940
// FUNCTION: fn_46035
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
// FUNCTION: fn_46036
__forceinline void CStormGustEffect::SetupParticle(CParticle* particle) {
  if (particle) {
    particle->SetTexture("effect\\ice.tga");
    particle->alpha = 250;

    const int speedRoll = rand() % 25;
    const float speed = static_cast<float>((speedRoll >= 0 ? speedRoll : 0) + 3);
    const int angleRoll = rand() % 360;
    const float angle = static_cast<float>(angleRoll >= 0 ? angleRoll : 0);
    const float z = ClientCosine(static_cast<int>(angle)) * -speed;
    const float x = ClientSine(static_cast<int>(angle)) * speed;
    SetParticleOffset(*particle, vector3d{x, 20.0f, z});

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
// FUNCTION: fn_46039
__forceinline void CStormGustEffect::UpdateAlpha(CParticle* particle) {
  if (particle) {
    const int frame = this->frame;
    int alpha = particle->alpha;
    const int remaining = 215 - frame;
    int fade = 0;
    if (frame == remaining - 10) {
      const float step = -(alpha / (remaining - frame));
      fade = static_cast<int>(step);
    }

    alpha += fade;
    alpha = alpha < 0 ? 0 : alpha;
    alpha = alpha > 254 ? 254 : alpha;
    particle->alpha = alpha;
  }
}

// OFFSET: 0x00B7B020
// FUNCTION: fn_46037
__forceinline void CStormGustEffect::UpdateParticle(CParticle* particle) {
  if (particle) {
    if (particle->elapsedFrames == 3) {
      particle->speedParameter = 0.0f;
    }

    const float speed = particle->speedParameter;
    const matrix& rotation = particle->rotation;
    const float* values = rotation.values;
    vector3d offset{
      values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9],
      values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10],
      values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11],
    };
    offset += particle->offset;
    particle->offset = offset;

    matrix translation;
    std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
    translation.MakeTranslate(offset.x, offset.y, offset.z);
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
