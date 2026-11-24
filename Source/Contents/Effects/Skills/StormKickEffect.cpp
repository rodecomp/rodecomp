#include "Contents/Effects/Skills/StormKickEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace {
// INLINED
__forceinline int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}
}

// OFFSET: 0x00B7B340
// FUNCTION: fn_19216
CStormKickEffect::CStormKickEffect() : CEffect(-1) {
}

// OFFSET: 0x00B7B3F0
// FUNCTION: fn_46017
__declspec(noinline) CStormKickEffect::~CStormKickEffect() {
}

// OFFSET: 0x00B7BE30
// FUNCTION: fn_46019
__forceinline void CStormKickEffect::SetStormColor(CParticle* particle, int effectId) {
  switch (effectId) {
  case EF_STORMKICK:
    particle->SetColor(50, 50, 255);
    break;
  case EF_STORMKICK1:
    particle->SetColor(164, 136, 55);
    break;
  case EF_STORMKICK2:
    particle->SetColor(255, 255, 255);
    break;
  case EF_STORMKICK3:
    particle->SetColor(130, 130, 255);
    break;
  case EF_STORMKICK6:
    if (0.0f == particle->animationParameters[1]) {
      particle->SetColor(255, 255, 255);
    } else if (1.0f == particle->animationParameters[1]) {
      particle->SetColor(255, 55, 55);
    }
    break;
  case EF_STORMKICK7:
    particle->SetColor(155, 255, 155);
    break;
  case EF_STORMKICK8:
    particle->SetColor(255, 255, 50);
    break;
  }
}

// OFFSET: 0x00B7B4F0
// FUNCTION: fn_19218
void CStormKickEffect::CreateStorms(int layer) {
  for (int index = 0; index < 10; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture("effect\\storm2.tga");
    particle->effectId = this->id;
    particle->columnIndex = index;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->speedParameter = 40.0f;
    particle->alpha = 0;
    particle->angle = static_cast<float>(ClampedRandom(360));
    particle->scale = 2.0f;
    particle->animationParameter = 50.0f;
    float* motion = particle->animationParameters;
    motion[0] = 5.0f;
    motion[1] = static_cast<float>(layer);

    this->SetStormColor(particle, this->id);

    particle->InitParticleQuad(particle->vertices);
    this->storms.push_back(particle);
    this->UpdateStorm(particle);
  }
}

// OFFSET: 0x00B7B6C0
// FUNCTION: fn_19219
void CStormKickEffect::CreateClouds(int layer) {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\cloud11.tga");
      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->alpha = 0;
      particle->angleParameter = 30;
      particle->angle = static_cast<float>(ring * 90);
      particle->phaseParameter = ClampedRandom(21) + 80;
      particle->animationParameter = 1.0f;

      if (this->id != EF_STORMKICK8) {
        if (layer == 0) {
          particle->speedParameter = 48.0f - ring * 5.0f;
          particle->scale = 14.0f - ring * 2.0f;
        } else if (layer == 1) {
          particle->speedParameter = 28.0f - ring * 5.0f;
          particle->scale = 6.0f - ring;
        }
      } else if (layer == 0) {
        particle->speedParameter = 28.0f - ring * 5.0f;
        particle->scale = 10.0f - ring * 2.0f;
      } else if (layer == 1) {
        particle->speedParameter = 18.0f - ring * 5.0f;
        particle->scale = 5.0f - ring;
      }

      particle->InitParticleQuad(particle->vertices);
      this->clouds.push_back(particle);
    }
  }
}

// OFFSET: 0x00B7B8D0
// FUNCTION: fn_19220
void CStormKickEffect::UpdateStorm(CParticle* particle) {
  if (!particle) {
    return;
  }

  const float* motion = particle->animationParameters;
  const float depth = -particle->animationParameter;
  const int segment = particle->columnIndex;
  float radius = particle->speedParameter;
  const float height = segment * motion[0] + depth;
  int angle = static_cast<int>(static_cast<float>(segment * 45) + particle->angle);
  angle = angle % 360;
  if (segment > 0) {
    for (int step = 1; step <= segment; ++step) {
      radius -= (2.2f - step * 0.2f) * particle->scale;
    }
  }

  EffectPoint corners[4];
  angle += 90;
  angle = angle % 360;
  corners[0].x = radius * g_cosTable[angle];
  corners[0].y = height - 1.0f;
  corners[0].z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  corners[1].x = radius * g_cosTable[angle];
  corners[1].y = height - 1.0f;
  corners[1].z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  corners[2].x = radius * g_cosTable[angle];
  corners[2].y = height - 1.0f;
  corners[2].z = radius * g_sinTable[angle];
  angle += 90;
  angle = angle % 360;
  corners[3].x = radius * g_cosTable[angle];
  corners[3].y = height - 1.0f;
  corners[3].z = radius * g_sinTable[angle];

  particle->vertices[0].x = corners[2].x;
  particle->vertices[0].y = corners[2].y;
  particle->vertices[0].z = corners[2].z;
  particle->vertices[1].x = corners[1].x;
  particle->vertices[1].y = corners[1].y;
  particle->vertices[1].z = corners[1].z;
  particle->vertices[2].x = corners[3].x;
  particle->vertices[2].y = corners[3].y;
  particle->vertices[2].z = corners[3].z;
  particle->vertices[3].x = corners[0].x;
  particle->vertices[3].y = corners[0].y;
  particle->vertices[3].z = corners[0].z;
}

// OFFSET: 0x00B7BC20
// FUNCTION: fn_19221
void CStormKickEffect::Close() {
  for (auto it = this->storms.begin(); it != this->storms.end(); ++it) {
    FreeParticle(*it);
  }
  this->storms.clear();

  for (auto it = this->clouds.begin(); it != this->clouds.end(); ++it) {
    FreeParticle(*it);
  }
  this->clouds.clear();
}

// OFFSET: 0x00B7BD70
// FUNCTION: fn_46018
__forceinline void CStormKickEffect::SetEffectLifetime() {
  switch (this->id) {
  case EF_STORMKICK:
  case EF_STORMKICK1:
  case EF_STORMKICK2:
  case EF_STORMKICK3:
  case EF_STORMKICK6:
    this->effectLifetime = 100;
    break;
  case EF_STORMKICK7:
    this->effectLifetime = 1000;
    break;
  case EF_STORMKICK8:
    this->effectLifetime = 60;
    break;
  }
}

// OFFSET: 0x00B7BC90
// FUNCTION: fn_19222
void CStormKickEffect::Open() {
  g_modeMgr.GetGameMode();

  switch (this->id) {
  case EF_STORMKICK6:
    this->CreateStorms(0);
    this->CreateStorms(1);
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  case EF_STORMKICK:
  case EF_STORMKICK1:
  case EF_STORMKICK2:
  case EF_STORMKICK3:
  case EF_STORMKICK7:
    this->CreateStorms(0);
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  case EF_STORMKICK8:
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  }

  this->SetEffectLifetime();
}

// OFFSET: 0x00B7BDD0
// FUNCTION: fn_19223
void CStormKickEffect::InitEvents() {
  switch (this->id) {
  case EF_STORMKICK:
    this->AddEvent(0, "effect\\T_회오리차기.wav", 5, 0);
    break;
  case EF_STORMKICK2:
    this->AddEvent(0, "ice_titan_attack.wav", 5, 0);
    break;
  case EF_STORMKICK6:
    this->AddEvent(0, "effect\\REVERBERATION.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B7BF60
// FUNCTION: fn_19224
bool CStormKickEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B7C3D0
// FUNCTION: fn_46020
__forceinline void CStormKickEffect::RotateActor() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  float angle;
  switch (this->id) {
  case EF_STORMKICK:
  case EF_STORMKICK8:
    if (this->frame > 47) {
      return;
    }
    angle = actor->render.angle + 30.0f;
    break;
  case EF_STORMKICK1:
    if (this->frame > 50) {
      return;
    }
    angle = actor->render.angle + 14.0f;
    break;
  default:
    return;
  }

  actor->render.angle = angle;
  if (angle > 360.0f) {
    actor->render.angle = angle - 360.0f;
  }
}

// OFFSET: 0x00B7C440
// FUNCTION: fn_46021
__forceinline void CStormKickEffect::AdvanceStorm(CParticle* particle) {
  if (particle) {
    int alpha = particle->alpha;
    if (particle->elapsedFrames > 60 && this->id != EF_STORMKICK7) {
      alpha = alpha - 1 < 0 ? 0 : alpha - 1;
    }
    if (particle->elapsedFrames <= 40) {
      ++alpha;
    }
    particle->alpha = alpha;

    float& rotation = particle->angle;
    const int jitter = rand() % 3;
    const int turned = static_cast<int>(rotation - static_cast<float>((jitter >= 0 ? jitter : 0) + 3));
    rotation = static_cast<float>(turned % 360);

    matrix transform;
    std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
    transform.MakeYRotation(rotation / 180.0f * 3.141592f);
    particle->transform = transform;
  }
}

// OFFSET: 0x00B7C560
// FUNCTION: fn_46022
__forceinline void CStormKickEffect::AdvanceCloud(CParticle* particle) {
  if (particle) {
    float& rotation = particle->angle;
    const float nextRotation = static_cast<float>(static_cast<int>(rotation + 5.0f) % 360);
    const int spread = std::min(particle->angleParameter + 3, 180);
    particle->angleParameter = spread;
    const int frame = particle->elapsedFrames;
    int alpha = particle->alpha;
    rotation = nextRotation;

    if (this->id == EF_STORMKICK8) {
      if (frame > 30) {
        alpha -= 3;
        if (alpha < 0) {
          alpha = 0;
        }
      } else {
        alpha += 3;
      }
    } else if (frame > 50) {
      alpha -= 1;
      alpha = alpha < 0 ? 0 : alpha;
    } else if (frame < 12) {
      ++alpha;
    }
    particle->alpha = alpha;
    float scale = particle->scale;
    if (frame > 12) {
      scale += 0.1f;
    }
    particle->scale = scale;

    const int segment = particle->columnIndex;
    const float radius = particle->animationParameter;
    const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(spread) / 20.0f) * segment) + nextRotation) % 360;
    const int tilt = particle->phaseParameter;
    const float horizontal = radius * g_cosTable[tilt];
    const float vertical = radius * g_sinTable[tilt];

    const float offsetX = horizontal * g_cosTable[angle];
    const float offsetZ = horizontal * g_sinTable[angle];

    EffectPoint corners[4];
    const float innerX = g_cosTable[angle] * particle->scale;
    const float innerY = -particle->speedParameter;
    const float innerZ = g_sinTable[angle] * particle->scale;
    const float outerX = innerX + offsetX;
    const float outerY = innerY - vertical;
    const float outerZ = offsetZ + innerZ;

    corners[3].x = innerX;
    corners[3].y = innerY;
    corners[3].z = innerZ;
    corners[1].x = outerX;
    corners[1].y = outerY;
    corners[1].z = outerZ;

    particle->vertices[3].x = corners[3].x;
    particle->vertices[3].y = corners[3].y;
    particle->vertices[3].z = corners[3].z;
    particle->vertices[1].x = corners[1].x;
    particle->vertices[1].y = corners[1].y;
    particle->vertices[1].z = corners[1].z;
    if (segment != 0) {
      particle->vertices[0].x = this->lastPoints[1].x;
      particle->vertices[0].y = this->lastPoints[1].y;
      particle->vertices[0].z = this->lastPoints[1].z;
      particle->vertices[2].x = this->lastPoints[0].x;
      particle->vertices[2].y = this->lastPoints[0].y;
      particle->vertices[2].z = this->lastPoints[0].z;
    }

    this->lastPoints[0].x = corners[3].x;
    this->lastPoints[0].y = corners[3].y;
    this->lastPoints[0].z = corners[3].z;
    this->lastPoints[1].x = corners[1].x;
    this->lastPoints[1].y = corners[1].y;
    this->lastPoints[1].z = corners[1].z;
  }
}

// OFFSET: 0x00B7BF70
// FUNCTION: fn_19225
void CStormKickEffect::OnProcess() {
  for (auto it = this->storms.begin(); it != this->storms.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    this->AdvanceStorm(particle);
    ++particle->elapsedFrames;
  }

  for (auto it = this->clouds.begin(); it != this->clouds.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    this->AdvanceCloud(particle);
    ++particle->elapsedFrames;
  }

  this->RotateActor();
}

// OFFSET: 0x00B7CA90
// FUNCTION: fn_46024
__forceinline void CStormKickEffect::RenderCloud(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int texPart = particle->columnIndex;
  if (texPart <= 0) {
    return;
  }

  ParticleQuad quad;
  quad.billboard = false;
  quad.unused = false;
  quad.depth = true;
  quad.top = 0.0f;
  quad.bottom = 1.0f;
  quad.left = static_cast<float>(texPart - 1) / 21.0f;
  quad.right = static_cast<float>(texPart) / 21.0f;
  particle->Render(quad);
}

// OFFSET: 0x00B7C990
// FUNCTION: fn_46023
__forceinline void CStormKickEffect::RenderStorm(CParticle* particle) {
  const int alpha = particle->alpha;
  const float* motion = particle->animationParameters;

  switch (this->id) {
  case EF_STORMKICK1:
  case EF_STORMKICK2:
  case EF_STORMKICK3:
  case EF_STORMKICK7:
    particle->destinationBlend = 6;
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    break;
  case EF_STORMKICK6:
    if (0.0f == motion[1]) {
      particle->destinationBlend = 6;
      particle->alpha = alpha / 3;
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
      particle->alpha = alpha;
    } else if (1.0f == motion[1]) {
      particle->destinationBlend = 2;
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
    break;
  default:
    particle->destinationBlend = 2;
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    break;
  }
}

// OFFSET: 0x00B7C7D0
// FUNCTION: fn_19226
void CStormKickEffect::Render() {
  for (auto it = this->storms.begin(); it != this->storms.end(); ++it) {
    CParticle* particle = *it;
    this->RenderStorm(particle);
  }

  for (auto it = this->clouds.begin(); it != this->clouds.end(); ++it) {
    CParticle* particle = *it;
    this->RenderCloud(particle);
  }
}
