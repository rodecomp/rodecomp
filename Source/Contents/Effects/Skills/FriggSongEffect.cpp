#include "Contents/Effects/Skills/FriggSongEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Projection/RenderProjection.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// INLINED
static void SetIndices(CParticle* particle, int effectId, int column, int row, int group) {
  particle->effectId = effectId;
  particle->columnIndex = column;
  particle->rowIndex = row;
  particle->groupIndex = group;
}

// OFFSET: 0x00701BB0
// FUNCTION: fn_06999
CFriggSongEffect::CFriggSongEffect() : CEffect(0x423) {
  this->effectLifetime = EffectLifetime(0x423);
}

// OFFSET: 0x00701C60
// FUNCTION: fn_46106
__declspec(noinline) CFriggSongEffect::~CFriggSongEffect() {
}

// OFFSET: 0x00701D40
// FUNCTION: fn_46107
void CFriggSongEffect::UpdateSegmentVertices(CParticle* particle) {
  if (!particle) {
    return;
  }

  const float speed = particle->speedParameter;
  int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * particle->columnIndex) + particle->angle) % 360;
  if (particle->columnIndex == 20) {
    angle = static_cast<int>(particle->angle);
  }

  const int tilt = particle->phaseParameter;
  const float cosine = g_cosTable[angle];
  const float sine = g_sinTable[angle];
  const float spread = speed * g_cosTable[tilt];
  const float height = speed * g_sinTable[tilt];
  const float offsetX = spread * cosine;
  const float offsetZ = spread * sine;
  EffectPoint points[2];
  const float innerX = particle->scale * cosine;
  const float innerY = 0.0f;
  const float innerZ = particle->scale * sine;
  const float outerY = 0.0f - height;
  const float outerX = innerX + offsetX;
  const float outerZ = innerZ + offsetZ;

  if (particle->columnIndex >= 1 && particle->columnIndex <= 20) {
    particle->vertices[1].x = outerX;
    particle->vertices[1].y = outerY;
    particle->vertices[1].z = outerZ;
    particle->vertices[3].x = innerX;
    particle->vertices[3].y = innerY;
    particle->vertices[3].z = innerZ;
    particle->vertices[0].x = this->lastInner[1].x;
    particle->vertices[0].y = this->lastInner[1].y;
    particle->vertices[0].z = this->lastInner[1].z;
    particle->vertices[2].x = this->lastInner[0].x;
    particle->vertices[2].y = this->lastInner[0].y;
    particle->vertices[2].z = this->lastInner[0].z;
  }

  this->lastInner[0].x = innerX;
  this->lastInner[0].y = innerY;
  this->lastInner[0].z = innerZ;
  this->lastInner[1].x = outerX;
  this->lastInner[1].y = outerY;
  this->lastInner[1].z = outerZ;
}

// OFFSET: 0x007021C0
// FUNCTION: fn_46108
void CFriggSongEffect::SetupParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  particle->alpha = 50;
  particle->SetColor(50, 200, 50);
  const int row = particle->rowIndex;
  particle->angleParameter = 360;
  particle->speedParameter = 120.0f;
  particle->phaseParameter = 90;
  particle->angle = static_cast<float>(row * 10);
  particle->scale = static_cast<float>(row) * 0.5f + 2.5f;
  (&particle->animationParameter)[row] = 0.0f;
  particle->elapsedFrames = particle->rowIndex;
  this->UpdateSegmentVertices(particle);
}

// OFFSET: 0x00701EE0
// FUNCTION: fn_07001
void CFriggSongEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00701F20
// FUNCTION: fn_07002
void CFriggSongEffect::Open() {
  for (int column = 0; column <= 21; ++column) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture("effect\\ring_blue.tga");
    SetIndices(particle, this->id, column, 0, 1);
    particle->alpha = 50;
    particle->SetColor(50, 200, 50);
    const int row = particle->rowIndex;
    particle->angleParameter = 360;
    particle->speedParameter = 120.0f;
    particle->phaseParameter = 90;
    particle->angle = static_cast<float>(row * 10);
    particle->scale = static_cast<float>(row) * 0.5f + 2.5f;
    (&particle->animationParameter)[row] = 0.0f;
    particle->elapsedFrames = particle->rowIndex;

    const float speed = particle->speedParameter;
    int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * particle->columnIndex) + particle->angle) % 360;
    if (particle->columnIndex == 20) {
      angle = static_cast<int>(particle->angle);
    }

    const int tilt = particle->phaseParameter;
    const float cosine = g_cosTable[angle];
    const float sine = g_sinTable[angle];
    const float spread = speed * g_cosTable[tilt];
    const float height = speed * g_sinTable[tilt];
    const float offsetX = spread * cosine;
    EffectPoint points[2];
    const float innerX = particle->scale * cosine;
    const float innerY = 0.0f;
    const float innerZ = particle->scale * sine;
    const float outerX = innerX + offsetX;
    const float outerY = 0.0f - height;
    const float outerZ = innerZ + spread * sine;

    if (particle->columnIndex >= 1 && particle->columnIndex <= 20) {
      particle->vertices[1].x = outerX;
      particle->vertices[1].y = outerY;
      particle->vertices[1].z = outerZ;
      particle->vertices[3].x = innerX;
      particle->vertices[3].y = innerY;
      particle->vertices[3].z = innerZ;
      particle->vertices[0].x = this->lastInner[1].x;
      particle->vertices[0].y = this->lastInner[1].y;
      particle->vertices[0].z = this->lastInner[1].z;
      particle->vertices[2].x = this->lastInner[0].x;
      particle->vertices[2].y = this->lastInner[0].y;
      particle->vertices[2].z = this->lastInner[0].z;
    }

    this->lastInner[0].x = innerX;
    this->lastInner[0].y = innerY;
    this->lastInner[0].z = innerZ;
    this->lastInner[1].x = outerX;
    this->lastInner[1].y = outerY;
    this->lastInner[1].z = outerZ;
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x007021A0
// FUNCTION: fn_07003
void CFriggSongEffect::InitEvents() {
  this->AddEvent(EFFECT_EVENT_SOUND, "effect\\wm_frigg_song.wav", 0, 0);
}

// OFFSET: 0x007023D0
// FUNCTION: fn_07004
bool CFriggSongEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00702600
// FUNCTION: fn_46109
void CFriggSongEffect::UpdateParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int age = particle->elapsedFrames;
  const float baseAngle = particle->angle;
  const int angle = static_cast<int>(baseAngle - static_cast<float>(age));
  float height = 1.0f;
  if (age >= 20) {
    if (age <= 140) {
      const float value = g_sinTable[age - 20] * particle->speedParameter;
      height = value < 0.0f ? 0.0f : value;
    } else {
      const float speed = particle->speedParameter - 0.5f - particle->speedParameter * 0.01f;
      particle->speedParameter = speed < 0.0f ? 0.0f : speed;
      height = particle->speedParameter;
    }
  }
  particle->animationParameter = height;

  matrix rotation;
  std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
  rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
  matrix scale;
  std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
  scale.SetScale(1.0f, particle->animationParameter / 100.0f * 1.1f, 1.0f);
  matrix first = rotation;
  rotation.Compose(first, scale);
  particle->transform = rotation;
}

// OFFSET: 0x007023E0
// FUNCTION: fn_07005
void CFriggSongEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position = this->positionVector;
    CParticle* particle = *it;
    if (particle) {
      const int age = particle->elapsedFrames;
      const float baseAngle = particle->angle;
      const int angle = static_cast<int>(baseAngle - static_cast<float>(age));
      float& height = particle->animationParameter;
      if (age < 20) {
        height = 1.0f;
      } else if (age <= 140) {
        const float value = g_sinTable[age - 20] * particle->speedParameter;
        height = value < 0.0f ? 0.0f : value;
      } else {
        const float speed = particle->speedParameter - 0.5f - particle->speedParameter * 0.01f;
        particle->speedParameter = speed < 0.0f ? 0.0f : speed;
        height = particle->speedParameter;
      }

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(1.0f, height / 100.0f * 1.1f, 1.0f);
      matrix first = rotation;
      rotation.Compose(first, scale);
      particle->transform = rotation;
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00702840
// FUNCTION: fn_46259
__forceinline void CFriggSongEffect::RenderParticle(CParticle* particle) {
  if (particle) {
    const int texPart = particle->columnIndex;
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
}

// OFFSET: 0x007027A0
// FUNCTION: fn_07006
void CFriggSongEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderParticle(*it);
  }
}
