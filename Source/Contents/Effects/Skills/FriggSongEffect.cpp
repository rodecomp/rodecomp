#include "Contents/Effects/Skills/FriggSongEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00701BB0
// FUNCTION: fn_06999
CFriggSongEffect::CFriggSongEffect() : CEffect(0x423) {
  this->effectLifetime = EffectLifetime(0x423);
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
  for (int segment = 0; segment <= 21; ++segment) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture("effect\\ring_blue.tga");
    particle->effectId = this->id;
    particle->columnIndex = segment;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->alpha = 50;
    particle->SetColor(50, 200, 50);
    particle->angleParameter = 360;
    particle->speedParameter = 120.0f;
    particle->phaseParameter = 90;
    float& baseAngle = particle->angle;
    const int row = particle->rowIndex;
    baseAngle = static_cast<float>(row * 10);
    particle->scale = static_cast<float>(row) * 0.5f + 2.5f;
    (&particle->animationParameter)[row] = 0.0f;
    particle->elapsedFrames = particle->rowIndex;

    int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * particle->columnIndex) + baseAngle) % 360;
    if (particle->columnIndex == 20) {
      angle = static_cast<int>(baseAngle);
    }

    const int tilt = particle->phaseParameter;
    const float spread = particle->speedParameter * g_cosTable[tilt];
    const float height = particle->speedParameter * g_sinTable[tilt];
    const float cosine = g_cosTable[angle];
    const float sine = g_sinTable[angle];
    const float offsetX = spread * cosine;
    const float offsetZ = spread * sine;

    EffectPoint points[2];
    const float innerX = particle->scale * cosine;
    const float innerY = 0.0f;
    const float innerZ = particle->scale * sine;
    const float outerX = innerX + offsetX;
    const float outerY = 0.0f - height;
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
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x007021A0
// FUNCTION: fn_07003
void CFriggSongEffect::InitEvents() {
  this->AddEvent(0, "effect\\wm_frigg_song.wav", 0, 0);
}

// OFFSET: 0x007023D0
// FUNCTION: fn_07004
bool CFriggSongEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x007023E0
// FUNCTION: fn_07005
void CFriggSongEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      const int age = particle->elapsedFrames;
      const float baseAngle = particle->angle;
      const int angle = static_cast<int>(baseAngle - static_cast<float>(age));
      float& height = particle->animationParameter;
      if (age < 20) {
        height = 1.0f;
      } else if (age <= 140) {
        height = (std::max)(0.0f, g_sinTable[age - 20] * particle->speedParameter);
      } else {
        const float speed = particle->speedParameter;
        particle->speedParameter = (std::max)(0.0f, speed - 0.5f - speed * 0.01f);
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
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x007027A0
// FUNCTION: fn_07006
void CFriggSongEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

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
