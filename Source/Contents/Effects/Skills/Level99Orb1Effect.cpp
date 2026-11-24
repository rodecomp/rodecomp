#include "Contents/Effects/Skills/Level99Orb1Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

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

// OFFSET: 0x00B6B210
// FUNCTION: fn_19089
CLevel99Orb1Effect::CLevel99Orb1Effect() : CEffect(976) {
}

// OFFSET: 0x00B6B2B0
// FUNCTION: fn_46229
__declspec(noinline) CLevel99Orb1Effect::~CLevel99Orb1Effect() {
}

// OFFSET: 0x00B6B390
// FUNCTION: fn_19091
void CLevel99Orb1Effect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B6B3D0
// FUNCTION: fn_19092
void CLevel99Orb1Effect::Open() {
  for (int row = 0; row < 4; ++row) {
    for (int column = 0; column < 4; ++column) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->SetTexture("effect\\whitelight.tga");
        particle->effectId = this->id;
        particle->columnIndex = column;
        particle->rowIndex = row;
        particle->groupIndex = 2;
        particle->speedParameter = 0.15f;
        particle->scale = 1.8f;
        particle->SetColor(80, 80, 255);
        particle->alpha = 0;

        EffectPoint offset;
        offset.y = static_cast<float>(Random(100));
        this->offsets[row * 4 + column] = offset;

        for (int i = 0; i < 4; ++i) {
          particle->animationValues[i] = static_cast<float>(Random(360));
        }

        this->particles.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B6B520
// FUNCTION: fn_19093
bool CLevel99Orb1Effect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6B530
// FUNCTION: fn_19094
void CLevel99Orb1Effect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    this->UpdateParticleForProcess(particle);

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B6B960
// FUNCTION: fn_46230
__declspec(noinline) void CLevel99Orb1Effect::UpdateParticle(CParticle* particle) {
  if (particle) {
    const int index = particle->columnIndex + particle->rowIndex * 4;
    const float speed = particle->speedParameter;
    const float heightBeforeUpdate = this->offsets[index].y;
    if (heightBeforeUpdate < 0.0f) {
      if (heightBeforeUpdate < -30.0f) {
        EffectPoint offset;
        offset.y = static_cast<float>(Random(100));
        this->offsets[index] = offset;

        for (int i = 0; i < 4; ++i) {
          particle->animationValues[i] = static_cast<float>(Random(360));
        }

        particle->alpha = 0;
      } else {
        for (int i = 0; i < 2; ++i) {
          const int current = static_cast<int>(particle->animationValues[i]);
          const int delta = static_cast<int>(particle->animationValues[i + 2]) - current;
          particle->animationValues[i] = static_cast<float>(current + (delta > 0 ? 3 : -2));
          if (std::abs(delta) < 3) {
            if (delta > 0) {
              particle->animationValues[i + 2] = static_cast<float>(Random(current));
            } else {
              particle->animationValues[i + 2] = static_cast<float>(359 - Random(current));
            }
          }
        }

        this->offsets[index].x += speed * g_sinTable[static_cast<int>(particle->animationValues[0])];
        this->offsets[index].z += speed * g_sinTable[static_cast<int>(particle->animationValues[1])];
        if (this->offsets[index].y < -20.0f) {
          particle->alpha = std::max(particle->alpha - 4, 0);
        } else if (particle->alpha != 255) {
          particle->alpha = 255;
        }
      }
    }

    this->offsets[index].y -= speed;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);

    matrix translation;
    std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
    translation.MakeTranslate(this->offsets[index].x, this->offsets[index].y, this->offsets[index].z);

    matrix scale;
    std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
    scale.SetScale(particle->scale, particle->scale, 1.0f);

    matrix scaled;
    scaled.Compose(scale, rotation);
    matrix result;
    result.Compose(scaled, translation);
    particle->transform = result;
  }
}

// INLINED
void CLevel99Orb1Effect::UpdateParticleForProcess(CParticle* particle) {
  if (particle) {
    const int index = particle->columnIndex + particle->rowIndex * 4;
    const float speed = particle->speedParameter;
    if (this->offsets[index].y < 0.0f) {
      if (this->offsets[index].y < -30.0f) {
        const float height = static_cast<float>(Random(100));
        this->offsets[index].x = 0.0f;
        this->offsets[index].y = height;
        this->offsets[index].z = 0.0f;

        for (int i = 0; i < 4; ++i) {
          particle->animationValues[i] = static_cast<float>(Random(360));
        }

        particle->alpha = 0;
      } else {
        for (int i = 0; i < 2; ++i) {
          const int current = static_cast<int>(particle->animationValues[i]);
          const int delta = static_cast<int>(particle->animationValues[i + 2]) - current;
          particle->animationValues[i] = static_cast<float>(current + (delta > 0 ? 3 : -2));
          if (std::abs(delta) < 3) {
            if (delta > 0) {
              particle->animationValues[i + 2] = static_cast<float>(Random(current));
            } else {
              particle->animationValues[i + 2] = static_cast<float>(359 - Random(current));
            }
          }
        }

        this->offsets[index].x += speed * g_sinTable[static_cast<int>(particle->animationValues[0])];
        this->offsets[index].z += speed * g_sinTable[static_cast<int>(particle->animationValues[1])];
        if (this->offsets[index].y < -20.0f) {
          const int alpha = particle->alpha - 4;
          particle->alpha = alpha >= 0 ? alpha : 0;
        } else if (particle->alpha != 255) {
          particle->alpha = 255;
        }
      }
    }

    this->offsets[index].y -= speed;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);

    matrix translation;
    std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
    translation.MakeTranslate(this->offsets[index].x, this->offsets[index].y, this->offsets[index].z);

    matrix scale;
    std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
    scale.SetScale(particle->scale, particle->scale, 1.0f);

    matrix scaled;
    scaled.Compose(scale, rotation);
    matrix result;
    result.Compose(scaled, translation);
    particle->transform = result;
  }
}

// OFFSET: 0x00B6BD30
// FUNCTION: fn_19095
void CLevel99Orb1Effect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->RenderParticle(particle);
  }
}

// OFFSET: 0x00B6BDB0
// FUNCTION: fn_46231
__forceinline void CLevel99Orb1Effect::RenderParticle(CParticle* particle) {
  if (particle) {
    for (int pass = 0; pass < 2; ++pass) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
