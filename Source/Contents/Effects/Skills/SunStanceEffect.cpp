#include "Contents/Effects/Skills/SunStanceEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0072D5C0
// FUNCTION: fn_07491
CSunStanceEffect::CSunStanceEffect() : CEffect(-1) {
}

// OFFSET: 0x0072D640
// FUNCTION: fn_07492
__declspec(noinline) CSunStanceEffect::~CSunStanceEffect() {
}

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
void CSunStanceEffect::Open() {
  for (int index = 0; index < 3; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (!particle) {
      continue;
    }

    switch (this->id) {
    case 1214:
      particle->SetTexture("effect\\sun_stance\\sun_stance.tga");
      break;
    case 1215:
      particle->SetTexture("effect\\sun_stance\\lunar_stance.tga");
      break;
    case 1216:
      particle->SetTexture("effect\\sun_stance\\star_stance.tga");
      break;
    case 1217:
      if (index == 0) {
        particle->SetTexture("effect\\sun_stance\\sun_stance.tga");
      } else if (index == 1) {
        particle->SetTexture("effect\\sun_stance\\lunar_stance.tga");
      } else {
        particle->SetTexture("effect\\sun_stance\\star_stance.tga");
      }
      break;
    default:
      particle->SetTexture("effect\\sun_stance\\sun_stance.tga");
      break;
    }

    particle->effectId = this->id;
    int* indices = &particle->groupIndex;
    indices[1] = 1;
    indices[2] = 0;
    indices[0] = 1;
    particle->alpha = 255;
    particle->scale = 3.0f;
    particle->animationParameter = static_cast<float>(index) * 2.0943947f;
    particle->InitParticleQuad(particle->vertices);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072DB00
// FUNCTION: fn_07496
bool CSunStanceEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0072DB10
// FUNCTION: fn_07497
void CSunStanceEffect::OnProcess() {
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

    particle->animationParameter += 0.02f;
    const float angle = particle->animationParameter;
    const float cosine = ClientCos(angle) * 8.0f;
    const float sine = ClientSin(angle) * 8.0f;
    const float z = cosine + this->position[2];
    const float x = sine + this->position[0];
    const float y = this->position[1] - 10.0f;
    particle->position.z = z;
    particle->position.y = y;
    particle->position.x = x;
  }
}

// OFFSET: 0x0072DDD0
// FUNCTION: fn_07498
void CSunStanceEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
