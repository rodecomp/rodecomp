#include "Contents/Effects/Skills/FlowerCastingEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>

// OFFSET: 0x00B666F0
// FUNCTION: fn_46001
CFlowerCastingEffect::CFlowerCastingEffect() : CEffect(-1) {
}

// OFFSET: 0x00B66790
// FUNCTION: fn_46002
__declspec(noinline) CFlowerCastingEffect::~CFlowerCastingEffect() {
}

// OFFSET: 0x00B66870
// FUNCTION: fn_19059
void CFlowerCastingEffect::AddRing(int kind) {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->SetTexture("effect\\ring_blue.tga");
      particle->angleParameter = 315;
      particle->alpha = 0;

      float& baseAngle = particle->angle;
      if (kind == 0) {
        if (ring == 0) {
          particle->scale = 4.5f;
          particle->phaseParameter = 85;
          particle->speedParameter = 27.0f;
          baseAngle = 0.0f;
        } else if (ring == 1) {
          particle->scale = 5.0f;
          particle->phaseParameter = 80;
          particle->speedParameter = 28.0f;
          baseAngle = 90.0f;
        } else if (ring == 2) {
          particle->scale = 5.5f;
          particle->phaseParameter = 70;
          particle->speedParameter = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 3) {
          particle->scale = 5.0f;
          particle->phaseParameter = 75;
          particle->speedParameter = 30.0f;
          baseAngle = 270.0f;
        }
      } else if (kind == 1) {
        if (ring == 0) {
          particle->scale = 5.5f;
          particle->phaseParameter = 50;
          particle->speedParameter = 30.0f;
          baseAngle = 90.0f;
        } else if (ring == 1) {
          particle->scale = 6.0f;
          particle->phaseParameter = 55;
          particle->speedParameter = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 2) {
          particle->scale = 6.5f;
          particle->phaseParameter = 65;
          particle->speedParameter = 28.0f;
          baseAngle = 270.0f;
        } else if (ring == 3) {
          particle->scale = 6.0f;
          particle->phaseParameter = 60;
          particle->speedParameter = 27.0f;
          baseAngle = 0.0f;
        }
      }

      particle->SetColor(100, 100, 255);
      particle->animationParameter = 0.0f;
      particle->animationParameters[0] = particle->speedParameter;
      particle->InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// INLINED
template <bool framesFirst>
static __forceinline void UpdateFlowerSegmentVertices(CFlowerCastingEffect* effect, CParticle* particle) {
  if (!particle) {
    return;
  }

  const int segment = particle->columnIndex;
  float radius;
  if (framesFirst) {
    const int frames = particle->elapsedFrames;
    radius = g_sinTable[segment * 9] * particle->speedParameter;
    if (frames <= 90) {
      radius *= g_sinTable[frames];
    }
  } else {
    radius = g_sinTable[segment * 9] * particle->speedParameter;
    if (particle->elapsedFrames <= 90) {
      radius *= g_sinTable[particle->elapsedFrames];
    }
  }
  radius = (std::max)(radius, 0.0f);

  const float baseAngle = particle->angle;
  const int angle = static_cast<int>(static_cast<int>(particle->angleParameter / 20.0f) * segment + baseAngle) % 360;
  const int tilt = particle->phaseParameter;
  const float cosine = g_cosTable[angle];
  const float sine = g_sinTable[angle];
  const float spread = radius * g_cosTable[tilt];
  const float height = radius * g_sinTable[tilt];
  const float offsetX = spread * cosine;
  const float offsetZ = spread * sine;

  EffectPoint points[2];
  EffectPoint& inner = points[0];
  EffectPoint& outer = points[1];
  inner.x = particle->scale * cosine;
  inner.y = 0.0f;
  inner.z = particle->scale * sine;
  outer.x = inner.x + offsetX;
  outer.y = 0.0f - height;
  outer.z = inner.z + offsetZ;

  particle->vertices[3].x = inner.x;
  particle->vertices[3].y = inner.y;
  particle->vertices[3].z = inner.z;
  particle->vertices[1].x = outer.x;
  particle->vertices[1].y = outer.y;
  particle->vertices[1].z = outer.z;

  if (segment != 0) {
    particle->vertices[0].x = effect->lastPoints[1].x;
    particle->vertices[0].y = effect->lastPoints[1].y;
    particle->vertices[0].z = effect->lastPoints[1].z;
    particle->vertices[2].x = effect->lastPoints[0].x;
    particle->vertices[2].y = effect->lastPoints[0].y;
    particle->vertices[2].z = effect->lastPoints[0].z;
  }

  effect->lastPoints[0].x = inner.x;
  effect->lastPoints[0].y = inner.y;
  effect->lastPoints[0].z = inner.z;
  effect->lastPoints[1].x = outer.x;
  effect->lastPoints[1].y = outer.y;
  effect->lastPoints[1].z = outer.z;
}

// OFFSET: 0x00B66AA0
// FUNCTION: fn_46003
void CFlowerCastingEffect::UpdateSegmentVertices(CParticle* particle) {
  UpdateFlowerSegmentVertices<true>(this, particle);
}

// OFFSET: 0x00B66C60
// FUNCTION: fn_19060
void CFlowerCastingEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B66CA0
// FUNCTION: fn_19061
void CFlowerCastingEffect::Open() {
  this->effectLifetime = 400;

  CWorld* world = g_modeMgr.GetGameMode()->scene;
  const vector3d& player = world->player->render.position;
  g_soundMgr->PlayWave("effect\\T_안락한마법.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);

  this->AddRing(0);
  this->AddRing(1);
}

// OFFSET: 0x00B66D20
// FUNCTION: fn_19062
bool CFlowerCastingEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B66F90
// FUNCTION: fn_46004
__forceinline void CFlowerCastingEffect::UpdateSegment(CParticle* particle) {
  if (!particle || particle->elapsedFrames <= 0) {
    return;
  }

  int phase = particle->elapsedFrames;
  phase %= 90;
  particle->speedParameter = g_sinTable[phase * 4] * 2.5f + particle->animationParameters[0];

  const int age = particle->elapsedFrames;
  int alpha = particle->alpha;
  if (age > 110) {
    const int faded = alpha - 1;
    alpha = faded >= 0 ? faded : 0;
  } else if (age < 20) {
    alpha += 5;
    if (alpha > 180) {
      alpha = 180;
    }
  }
  particle->alpha = alpha;
  UpdateFlowerSegmentVertices<false>(this, particle);
}

// OFFSET: 0x00B66D30
// FUNCTION: fn_19063
void CFlowerCastingEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = particle->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];

    this->UpdateSegment(particle);
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B67290
// FUNCTION: fn_46005
__forceinline void CFlowerCastingEffect::RenderSegment(CParticle* particle) {
  if (!particle || particle->columnIndex <= 0) {
    return;
  }

  const int segment = particle->columnIndex;
  ParticleQuad quad;
  quad.billboard = false;
  quad.unused = false;
  quad.depth = true;
  quad.top = 0.0f;
  quad.bottom = 1.0f;
  quad.left = static_cast<float>(segment - 1) / 21.0f;
  quad.right = static_cast<float>(segment) / 21.0f;
  particle->Render(quad);
}

// OFFSET: 0x00B671E0
// FUNCTION: fn_19064
void CFlowerCastingEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderSegment(*it);
  }
}
