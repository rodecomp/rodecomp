#include "Contents/Effects/Skills/ScatterEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B75D10
// FUNCTION: fn_19167
CScatterEffect::CScatterEffect() : CEffect(-1) {
}

// OFFSET: 0x00B75E70
// FUNCTION: fn_19169
void CScatterEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B75EB0
// FUNCTION: fn_19170
void CScatterEffect::Open() {
  for (int i = 0; i < 50; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    if (this->id == 1002) {
      this->effectLifetime = 300;
      particle->SetSprite("이팩트\\sakura01.spr");
      particle->SetAction("이팩트\\sakura01.act");
      const int scaleRoll = rand() % 6;
      particle->scale = static_cast<float>((scaleRoll >= 0 ? scaleRoll : 0) + 22) / 70.0f;
      const int delayRoll = rand() % 3;
      particle->frameDelay = static_cast<float>((delayRoll >= 0 ? delayRoll : 0) + 2) * 0.15f;
      const int spinRoll = rand() % 360;
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(spinRoll >= 0 ? spinRoll : 0);
      const int angleRoll = rand() % 360;
      const int angle = angleRoll >= 0 ? angleRoll : 0;
      particle->velocity.x = 0.0f;
      particle->velocity.y = 0.0f;
      particle->velocity.z = 0.0f;
      particle->alpha = 250;
      particle->phaseParameter = angle;
      particle->destinationBlend = 6;
    }

    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B76120
// FUNCTION: fn_19171
bool CScatterEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B76130
// FUNCTION: fn_19172
void CScatterEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      particle->alpha = std::ranges::max(particle->alpha - 3, 0);

      const int angle = particle->phaseParameter;
      const float rise = particle->frameDelay;
      const float driftX = g_cosTable[angle] * 0.2f;
      const float driftZ = g_sinTable[angle] * 0.2f;
      particle->velocity.y += rise;
      particle->velocity.x += driftX;
      particle->velocity.z += driftZ;

      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(particle->velocity.x + 0.0f, particle->velocity.y - 40.0f, particle->velocity.z + 0.0f);
      particle->transform = translate;
    }
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x00B76390
// FUNCTION: fn_19173
void CScatterEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Draw(1);
    }
  }
}
