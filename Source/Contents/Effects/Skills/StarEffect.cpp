#include "Contents/Effects/Skills/StarEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
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

struct StarParticleState {
  /* 0000 */ float angles[4];
  /* 0010 */ float lifetime;
  /* 0014 */ float texture;
  /* 0018 */ float baseScale;
  /* 001C */ float phase;
};

static_assert(sizeof(StarParticleState) == 0x20);

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
      particle->columnIndex = index;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      this->SetupParticle(particle);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B79FA0
// FUNCTION: fn_19205
void CStarEffect::SetupParticle(CParticle* particle) {
  StarParticleState& state = *reinterpret_cast<StarParticleState*>(&particle->animationParameter);

  particle->elapsedFrames = 0;
  particle->alpha = 0;
  float* angle = state.angles;
  for (int index = 0; index < 4; ++index) {
    *angle++ = static_cast<float>(Random(360));
  }

  state.lifetime = static_cast<float>(Random(200) + 300);
  state.texture = 1.0f;
  state.baseScale = static_cast<float>(Random(20)) + 20.0f;
  state.phase = static_cast<float>(Random(360));
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

// OFFSET: 0x00B7A1C0
// FUNCTION: fn_19208
void CStarEffect::UpdateParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  StarParticleState& state = *reinterpret_cast<StarParticleState*>(&particle->animationParameter);
  const int frame = particle->elapsedFrames;
  if (frame % 14 == 0) {
    int texture = static_cast<int>(state.texture) + 1;
    if (texture > 6) {
      texture = 1;
    }
    state.texture = static_cast<float>(texture);
    const std::string name = FormatOwnedString("effect\\star%02d.bmp", texture);
    particle->SetTexture(name.c_str());
  }

  int alpha = particle->alpha;
  if (particle->elapsedFrames < 80) {
    alpha += 3;
  }
  if (particle->elapsedFrames > static_cast<int>(state.lifetime)) {
    --alpha;
    if (alpha <= 0) {
      this->SetupParticle(particle);
    }
  }
  particle->alpha = alpha;

  float* angle = state.angles;
  for (int pair = 0; pair < 2; ++pair) {
    int target = static_cast<int>(angle[1]);
    int current = static_cast<int>(angle[0]);
    if (target > current) {
      current = std::min(current + 1, 359);
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

  const vector3d offset = *reinterpret_cast<const vector3d*>(&particle->offsetX);
  particle->offsetY = offset.y;
  particle->offsetX = g_sinTable[static_cast<int>(state.angles[0])] * 0.05f + offset.x;
  particle->offsetZ = g_sinTable[static_cast<int>(state.angles[2])] * 0.05f + offset.z;
  particle->position.x = particle->offsetX + this->position[0];
  particle->position.y = offset.y;
  particle->position.z = particle->offsetZ + this->position[2];

  int phase = static_cast<int>(state.phase + 1.0f);
  if (phase >= 360) {
    phase -= 360;
  }
  phase = phase >= 0 ? phase : 0;
  particle->scale = state.baseScale * g_sinTable[phase] * 0.05f + state.baseScale;
  state.phase = static_cast<float>(phase);

  matrix rotation;
  std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
  rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
  matrix scale;
  std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
  scale.SetScale(particle->scale, particle->scale, 1.0f);
  matrix transform;
  transform.Compose(scale, rotation);
  particle->transform = transform;
}

// OFFSET: 0x00B7A540
// FUNCTION: fn_19209
void CStarEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
