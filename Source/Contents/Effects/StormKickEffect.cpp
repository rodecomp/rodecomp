#include "Contents/Effects/StormKickEffect.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B7B340
// FUNCTION: fn_19216
CStormKickEffect::CStormKickEffect() : CEffect(-1) {
}

// OFFSET: 0x00B7B4F0
// FUNCTION: fn_19218
void CStormKickEffect::CreateStorms(int layer) {
  for (int index = 0; index < 10; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture("effect\\storm2.tga");
    particle->unknown_0004 = this->id;
    particle->unknown_000C = index;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->unknown_00C0 = 40.0f;
    particle->alpha = 0;
    *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(std::max(rand() % 360, 0));
    particle->scale = 2.0f;
    particle->unknown_0054 = 50.0f;
    float* motion = reinterpret_cast<float*>(particle->unknown_0058);
    motion[0] = 5.0f;
    motion[1] = static_cast<float>(layer);

    switch (this->id) {
    case 435:
      particle->SetColor(50, 50, 255);
      break;
    case 459:
      particle->SetColor(164, 136, 55);
      break;
    case 460:
      particle->SetColor(255, 255, 255);
      break;
    case 461:
      particle->SetColor(130, 130, 255);
      break;
    case 464:
      if (motion[1] == 0.0f) {
        particle->SetColor(255, 255, 255);
      } else if (motion[1] == 1.0f) {
        particle->SetColor(255, 55, 55);
      }
      break;
    case 465:
      particle->SetColor(155, 255, 155);
      break;
    case 1223:
      particle->SetColor(255, 255, 50);
      break;
    }

    InitParticleQuad(particle->vertices);
    this->storms.push_back(particle);
    this->UpdateStorm(particle);
  }
}

// OFFSET: 0x00B7B6C0
// FUNCTION: fn_19219
void CStormKickEffect::CreateClouds(int layer) {
  int ring = 0;
  for (int angle = 0; angle < 360; angle += 90) {
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\cloud11.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->alpha = 0;
      particle->unknown_0050 = 30;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
      particle->unknown_004C = std::max(rand() % 21, 0) + 80;
      particle->unknown_0054 = 1.0f;

      if (this->id == 1223) {
        if (layer == 0) {
          particle->unknown_00C0 = 28.0f - ring * 5.0f;
          particle->scale = 10.0f - ring * 2.0f;
        } else if (layer == 1) {
          particle->unknown_00C0 = 18.0f - ring * 5.0f;
          particle->scale = 5.0f - ring;
        }
      } else if (layer == 0) {
        particle->unknown_00C0 = 48.0f - ring * 5.0f;
        particle->scale = 14.0f - ring * 2.0f;
      } else if (layer == 1) {
        particle->unknown_00C0 = 28.0f - ring * 5.0f;
        particle->scale = 6.0f - ring;
      }

      InitParticleQuad(particle->vertices);
      this->clouds.push_back(particle);
    }

    ++ring;
  }
}

// OFFSET: 0x00B7B8D0
// FUNCTION: fn_19220
void CStormKickEffect::UpdateStorm(CParticle* particle) {
  if (!particle) {
    return;
  }

  const float* motion = reinterpret_cast<const float*>(particle->unknown_0058);
  const int segment = particle->unknown_000C;
  float radius = particle->unknown_00C0;
  const float height = -particle->unknown_0054 + segment * motion[0];
  int angle = static_cast<int>(static_cast<float>(segment * 45) + *reinterpret_cast<const float*>(&particle->unknown_00C4)) % 360;
  for (int step = 1; step <= segment; ++step) {
    radius -= (2.2f - step * 0.2f) * particle->scale;
  }

  EffectPoint corners[4];
  angle = (angle + 90) % 360;
  corners[0].x = radius * g_cosTable[angle];
  corners[0].y = height - 1.0f;
  corners[0].z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  corners[1].x = radius * g_cosTable[angle];
  corners[1].y = height - 1.0f;
  corners[1].z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  corners[2].x = radius * g_cosTable[angle];
  corners[2].y = height - 1.0f;
  corners[2].z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
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
  for (CParticle* particle : this->storms) {
    FreeParticle(particle);
  }
  this->storms.clear();

  for (CParticle* particle : this->clouds) {
    FreeParticle(particle);
  }
  this->clouds.clear();
}

// OFFSET: 0x00B7BC90
// FUNCTION: fn_19222
void CStormKickEffect::Slot01() {
  g_modeMgr.GetGameMode();

  switch (this->id) {
  case 435:
  case 459:
  case 460:
  case 461:
  case 465:
    this->CreateStorms(0);
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  case 464:
    this->CreateStorms(0);
    this->CreateStorms(1);
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  case 1223:
    this->CreateClouds(0);
    this->CreateClouds(1);
    break;
  }

  switch (this->id) {
  case 435:
  case 459:
  case 460:
  case 461:
  case 464:
    this->unknown_002C = 100;
    break;
  case 465:
    this->unknown_002C = 1000;
    break;
  case 1223:
    this->unknown_002C = 60;
    break;
  }
}

// OFFSET: 0x00B7BDD0
// FUNCTION: fn_19223
void CStormKickEffect::Slot12() {
  switch (this->id) {
  case 435:
    this->AddEvent(0, "effect\\T_회오리차기.wav", 5, 0);
    break;
  case 460:
    this->AddEvent(0, "ice_titan_attack.wav", 5, 0);
    break;
  case 464:
    this->AddEvent(0, "effect\\REVERBERATION.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B7BF60
// FUNCTION: fn_19224
bool CStormKickEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B7BF70
// FUNCTION: fn_19225
void CStormKickEffect::Slot03() {
  for (auto it = this->storms.begin(); it != this->storms.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      int alpha = particle->alpha;
      if (particle->unknown_00E8 > 60 && this->id != 465) {
        alpha = std::max(alpha - 1, 0);
      }
      if (particle->unknown_00E8 <= 40) {
        ++alpha;
      }
      particle->alpha = alpha;

      float& rotation = *reinterpret_cast<float*>(&particle->unknown_00C4);
      rotation = static_cast<float>(static_cast<int>(rotation - static_cast<float>(std::max(rand() % 3, 0) + 3)) % 360);

      matrix transform;
      std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
      transform.MakeYRotation(rotation / 180.0f * 3.141592f);
      particle->transform = transform;
    }

    ++particle->unknown_00E8;
  }

  for (auto it = this->clouds.begin(); it != this->clouds.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      float& rotation = *reinterpret_cast<float*>(&particle->unknown_00C4);
      const float nextRotation = static_cast<float>(static_cast<int>(rotation + 5.0f) % 360);
      const int spread = std::min(particle->unknown_0050 + 3, 180);
      particle->unknown_0050 = spread;
      const int frame = particle->unknown_00E8;
      int alpha = particle->alpha;
      rotation = nextRotation;

      if (this->id == 1223) {
        if (frame > 30) {
          alpha = std::max(alpha - 3, 0);
        } else {
          alpha += 3;
        }
      } else if (frame > 50) {
        alpha = std::max(alpha - 1, 0);
      } else if (frame < 12) {
        ++alpha;
      }
      particle->alpha = alpha;
      if (frame > 12) {
        particle->scale += 0.1f;
      }

      const int segment = particle->unknown_000C;
      const float radius = particle->unknown_0054;
      const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(spread) / 20.0f) * segment) + nextRotation) % 360;
      const int tilt = particle->unknown_004C;
      const float horizontal = radius * g_cosTable[tilt];
      const float vertical = radius * g_sinTable[tilt];

      EffectPoint corners[4];
      corners[3].x = g_cosTable[angle] * particle->scale;
      corners[3].y = -particle->unknown_00C0;
      corners[3].z = g_sinTable[angle] * particle->scale;
      corners[1].x = corners[3].x + g_cosTable[angle] * horizontal;
      corners[1].y = corners[3].y - vertical;
      corners[1].z = corners[3].z + g_sinTable[angle] * horizontal;

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

      this->lastPoints[0] = corners[3];
      this->lastPoints[1] = corners[1];
    }

    ++particle->unknown_00E8;
  }

  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    return;
  }

  float angle;
  switch (this->id) {
  case 435:
  case 1223:
    if (this->frame > 47) {
      return;
    }
    angle = actor->render.angle + 30.0f;
    break;
  case 459:
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
