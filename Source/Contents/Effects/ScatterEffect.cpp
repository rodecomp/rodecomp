#include "Contents/Effects/ScatterEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
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
  for (CSpriteParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B75EB0
// FUNCTION: fn_19170
void CScatterEffect::Slot01() {
  for (int i = 0; i < 50; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    if (this->id == 1002) {
      this->unknown_002C = 300;
      particle->SetSprite("이팩트\\sakura01.spr");
      particle->SetAction("이팩트\\sakura01.act");
      particle->scale = static_cast<float>(std::max(rand() % 6, 0) + 22) / 70.0f;
      particle->frameDelay = static_cast<float>(std::max(rand() % 3, 0) + 2) * 0.15f;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(std::max(rand() % 360, 0));
      const int angle = std::max(rand() % 360, 0);
      particle->velocity.x = 0.0f;
      particle->velocity.y = 0.0f;
      particle->velocity.z = 0.0f;
      particle->alpha = 250;
      particle->unknown_004C = angle;
      particle->destinationBlend = 6;
    }

    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B76120
// FUNCTION: fn_19171
bool CScatterEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B76130
// FUNCTION: fn_19172
void CScatterEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CSpriteParticle* particle = *it) {
      particle->alpha -= 3;
      if (particle->alpha < 0) {
        particle->alpha = 0;
      }

      const int angle = particle->unknown_004C;
      particle->velocity.y += particle->frameDelay;
      particle->velocity.x += g_cosTable[angle] * 0.2f;
      particle->velocity.z += g_sinTable[angle] * 0.2f;

      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(particle->velocity.x + 0.0f, particle->velocity.y - 40.0f, particle->velocity.z + 0.0f);
      particle->transform = translate;
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B76390
// FUNCTION: fn_19173
void CScatterEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->sub_B40F30(1);
    }
  }
}
