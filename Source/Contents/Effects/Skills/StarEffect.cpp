#include "Contents/Effects/Skills/StarEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {

// INLINED
__forceinline int Random(int range) {
  if (range == 0) {
    return 0;
  }

  const int value = rand() % range;
  return value >= 0 ? value : 0;
}

}

// OFFSET: 0x00B79C80
// FUNCTION: fn_46025
CStarEffect::CStarEffect() : CEffect(EF_CLOUD9), unknown_00FC(0) {
}

// OFFSET: 0x00B79D20
// FUNCTION: fn_46026
__declspec(noinline) CStarEffect::~CStarEffect() {
}

// OFFSET: 0x00B79ED0
// FUNCTION: fn_19203
void CStarEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B79F10
// FUNCTION: fn_19204
void CStarEffect::Open() {
  for (int ring = 0; ring < 65; ++ring) {
    for (int index = 0; index < 4; ++index) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      int* indices = &particle->groupIndex;
      indices[1] = index;
      indices[2] = ring;
      indices[0] = 1;
      this->SetupParticle(particle);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B79FA0
// FUNCTION: fn_19205
void CStarEffect::SetupParticle(CParticle* particle) {
  particle->elapsedFrames = 0;
  particle->alpha = 0;
  float* angle = particle->starState.angles;
  for (int index = 0; index < 4; ++index) {
    *angle++ = static_cast<float>(Random(360));
  }

  particle->starState.lifetime = static_cast<float>(Random(200) + 300);
  particle->starState.texture = 1.0f;
  particle->starState.baseScale = static_cast<float>(Random(20)) + 20.0f;
  particle->starState.phase = static_cast<float>(Random(360));
  particle->SetTexture("effect\\star01.bmp");

  float speedZ = static_cast<float>(Random(200) + 25);
  const int lift = Random(10);
  float speedX = static_cast<float>(Random(200) + 25);
  const float speedY = static_cast<float>(lift) + 80.0f;
  if (Random(2) == 0) {
    speedX *= -1.0f;
  }
  if (Random(2) == 0) {
    speedZ *= -1.0f;
  }

  particle->offsetX = speedX;
  particle->offsetY = speedY;
  particle->offsetZ = speedZ;
}

// OFFSET: 0x00B7A160
// FUNCTION: fn_19206
bool CStarEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B7A170
// FUNCTION: fn_19207
void CStarEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    this->UpdateParticle(particle);
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B79E00
// FUNCTION: fn_46027
__forceinline void CStarEffect::AdvanceTexture(CParticle* particle) {
  StarParticleState& state = particle->starState;
  const int frame = particle->elapsedFrames;
  if (frame % 14 == 0) {
    int texture = static_cast<int>(state.texture) + 1;
    if (texture > 6) {
      texture = 1;
    }
    state.texture = static_cast<float>(texture);
    particle->SetTexture(FormatOwnedString("effect\\star%02d.bmp", texture).c_str());
  }
}

// OFFSET: 0x00B7A5E0
// FUNCTION: fn_46029
__forceinline void CStarEffect::UpdateAlpha(CParticle* particle) {
  int alpha = particle->alpha;
  if (particle->elapsedFrames < 80) {
    alpha += 3;
  }
  if (particle->elapsedFrames > static_cast<int>(particle->starState.lifetime)) {
    --alpha;
    if (alpha <= 0) {
      this->SetupParticle(particle);
    }
  }
  particle->alpha = alpha;
}

// OFFSET: 0x00B7A620
// FUNCTION: fn_46030
__forceinline void CStarEffect::UpdatePosition(CParticle* particle) {
  StarParticleState& state = particle->starState;
  float* angle = state.angles;
  for (int pair = 0; pair < 2; ++pair) {
    int target = static_cast<int>(angle[1]);
    int current = static_cast<int>(angle[0]);
    if (target > current) {
      ++current;
      if (current > 359) {
        current = 359;
      }
      if (current > target) {
        target = Random(current);
      }
    } else {
      const int previous = current - 1;
      current = previous >= 0 ? previous : 0;
      if (current <= target) {
        target = 359 - Random(current);
      }
    }
    angle[1] = static_cast<float>(target);
    angle[0] = static_cast<float>(current);
    angle += 2;
  }

  vector3d offset = particle->offset;
  offset.x = g_sinTable[static_cast<int>(state.angles[0])] * 0.05f + offset.x;
  offset.z = g_sinTable[static_cast<int>(state.angles[2])] * 0.05f + offset.z;
  particle->offset = offset;
  particle->position = vector3d{offset.x + this->position[0], offset.y, offset.z + this->position[2]};
}

// OFFSET: 0x00B7A760
// FUNCTION: fn_46031
__forceinline void CStarEffect::UpdateScale(CParticle* particle) {
  StarParticleState& state = particle->starState;
  int phase = static_cast<int>(state.phase + 1.0f);
  if (phase >= 360) {
    phase -= 360;
  }
  phase = phase >= 0 ? phase : 0;
  particle->scale = state.baseScale * g_sinTable[phase] * 0.05f + state.baseScale;
  state.phase = static_cast<float>(phase);
}

// OFFSET: 0x00B7A7C0
// FUNCTION: fn_46032
__forceinline void CStarEffect::UpdateTransform(CParticle* particle) {
  matrix rotation;
  std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
  rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
  const float particleScale = particle->scale;
  matrix scale;
  std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
  scale.SetScale(particleScale, particleScale, 1.0f);
  matrix transform;
  transform.Compose(scale, rotation);
  particle->transform = transform;
}

// OFFSET: 0x00B7A1C0
// FUNCTION: fn_19208
void CStarEffect::UpdateParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  this->AdvanceTexture(particle);
  this->UpdateAlpha(particle);
  this->UpdatePosition(particle);
  this->UpdateScale(particle);
  this->UpdateTransform(particle);
}

// OFFSET: 0x00B7A5A0
// FUNCTION: fn_46028
__forceinline void CStarEffect::RenderParticle(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B7A540
// FUNCTION: fn_19209
void CStarEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->RenderParticle(particle);
  }
}
