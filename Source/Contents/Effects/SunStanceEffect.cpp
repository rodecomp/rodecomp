#include "Contents/Effects/SunStanceEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0072D910
// FUNCTION: fn_07494
void CSunStanceEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0072D980
// FUNCTION: fn_07495
void CSunStanceEffect::Slot01() {
  for (int index = 0; index < 3; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    const char* texture;
    switch (this->id) {
    case 1214:
    default:
      texture = "effect\\sun_stance\\sun_stance.tga";
      break;
    case 1215:
      texture = "effect\\sun_stance\\lunar_stance.tga";
      break;
    case 1216:
      texture = "effect\\sun_stance\\star_stance.tga";
      break;
    case 1217:
      if (index == 0) {
        texture = "effect\\sun_stance\\sun_stance.tga";
      } else if (index == 1) {
        texture = "effect\\sun_stance\\lunar_stance.tga";
      } else {
        texture = "effect\\sun_stance\\star_stance.tga";
      }
      break;
    }
    particle->SetTexture(texture);

    particle->unknown_0004 = this->id;
    particle->unknown_000C = 1;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->alpha = 255;
    particle->scale = 3.0f;
    particle->unknown_0054 = static_cast<float>(index) * 2.0943947f;
    InitParticleQuad(particle->vertices);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072DB00
// FUNCTION: fn_07496
bool CSunStanceEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0072DB10
// FUNCTION: fn_07497
void CSunStanceEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    matrix rotation;
    rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
    matrix scaling;
    scaling.SetScale(particle->scale, particle->scale, 1.0f);
    matrix result;
    result.Compose(scaling, rotation);
    particle->transform = result;

    particle->unknown_0054 += 0.02f;
    const float angle = particle->unknown_0054;
    const float cosine = std::cos(angle) * 8.0f;
    const float sine = std::sin(angle) * 8.0f;
    particle->position.z = cosine + this->position[2];
    particle->position.y = this->position[1] - 10.0f;
    particle->position.x = sine + this->position[0];
  }
}

// OFFSET: 0x0072DDD0
// FUNCTION: fn_07498
void CSunStanceEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
