#include "Contents/Effects/StarEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {

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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B79F10
// FUNCTION: fn_19204
void CStarEffect::Slot01() {
  for (int ring = 0; ring < 65; ++ring) {
    for (int index = 0; index < 4; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = index;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      this->SetupParticle(particle);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B79FA0
// FUNCTION: fn_19205
void CStarEffect::SetupParticle(CParticle* particle) {
  StarParticleState& state = *reinterpret_cast<StarParticleState*>(&particle->unknown_0054);

  particle->unknown_00E8 = 0;
  particle->alpha = 0;
  for (int index = 0; index < 4; ++index) {
    state.angles[index] = static_cast<float>(std::max(rand() % 360, 0));
  }

  state.lifetime = static_cast<float>(std::max(rand() % 200, 0) + 300);
  state.texture = 1.0f;
  state.baseScale = static_cast<float>(std::max(rand() % 20, 0)) + 20.0f;
  state.phase = static_cast<float>(std::max(rand() % 360, 0));
  particle->SetTexture("effect\\star01.bmp");

  float speedZ = static_cast<float>(std::max(rand() % 200, 0) + 25);
  const float speedY = static_cast<float>(std::max(rand() % 10, 0)) + 80.0f;
  float speedX = static_cast<float>(std::max(rand() % 200, 0) + 25);
  if (std::max(rand() % 2, 0) == 0) {
    speedX *= -1.0f;
  }
  if (std::max(rand() % 2, 0) == 0) {
    speedZ *= -1.0f;
  }

  particle->unknown_00D0 = speedX;
  particle->unknown_00D4 = speedY;
  particle->unknown_00D8 = speedZ;
}

// OFFSET: 0x00B7A160
// FUNCTION: fn_19206
bool CStarEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B7A170
// FUNCTION: fn_19207
void CStarEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    this->UpdateParticle(particle);
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B7A1C0
// FUNCTION: fn_19208
void CStarEffect::UpdateParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  StarParticleState& state = *reinterpret_cast<StarParticleState*>(&particle->unknown_0054);
  if (particle->unknown_00E8 % 14 == 0) {
    int texture = static_cast<int>(state.texture) + 1;
    if (texture > 6) {
      texture = 1;
    }
    state.texture = static_cast<float>(texture);
    particle->SetTexture(FormatOwnedString("effect\\star%02d.bmp", texture).c_str());
  }

  const int frame = particle->unknown_00E8;
  int alpha = particle->alpha;
  if (frame < 80) {
    alpha += 3;
  }
  if (frame > static_cast<int>(state.lifetime)) {
    --alpha;
    if (alpha <= 0) {
      this->SetupParticle(particle);
    }
  }
  particle->alpha = alpha;

  for (int pair = 0; pair < 2; ++pair) {
    int current = static_cast<int>(state.angles[pair * 2]);
    int target = static_cast<int>(state.angles[pair * 2 + 1]);
    if (target > current) {
      current = std::min(current + 1, 359);
      if (target < current) {
        target = current ? std::max(rand() % current, 0) : 0;
      }
    } else {
      current = std::max(current - 1, 0);
      if (target >= current) {
        target = 359 - (current ? std::max(rand() % current, 0) : 0);
      }
    }
    state.angles[pair * 2 + 1] = static_cast<float>(target);
    state.angles[pair * 2] = static_cast<float>(current);
  }

  const float offsetX = particle->unknown_00D0;
  const float offsetY = particle->unknown_00D4;
  const float offsetZ = particle->unknown_00D8;
  particle->unknown_00D4 = offsetY;
  particle->unknown_00D0 = g_sinTable[static_cast<int>(state.angles[0])] * 0.05f + offsetX;
  particle->unknown_00D8 = g_sinTable[static_cast<int>(state.angles[2])] * 0.05f + offsetZ;
  particle->position.x = particle->unknown_00D0 + this->position[0];
  particle->position.y = offsetY;
  particle->position.z = particle->unknown_00D8 + this->position[2];

  int phase = static_cast<int>(state.phase + 1.0f);
  if (phase >= 360) {
    phase -= 360;
  }
  phase = std::max(phase, 0);
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
void CStarEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
