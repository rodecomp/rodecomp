#include "Contents/Effects/SmokeEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B763E0
// FUNCTION: fn_19174
CSmokeEffect::CSmokeEffect() : CEffect(44), visibleCount(1) {
}

// OFFSET: 0x00B76540
// FUNCTION: fn_19176
void CSmokeEffect::Close() {
  for (CSpriteParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B76580
// FUNCTION: fn_19177
void CSmokeEffect::Slot01() {
  for (int i = 0; i < 4; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B76710
// FUNCTION: fn_19178
bool CSmokeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B76720
// FUNCTION: fn_19179
void CSmokeEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CSpriteParticle* particle = *it) {
      matrix& rotation = *reinterpret_cast<matrix*>(particle->unknown_0198);
      float& angle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      float& spin = *reinterpret_cast<float*>(&particle->unknown_02D4);
      if (particle->unknown_00E8 < 50) {
        const float speed = particle->frameDelay;
        const float* values = rotation.values;
        particle->velocity.x += values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9];
        particle->velocity.y += values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10];
        particle->velocity.z += values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11];
        angle += spin;
        if (particle->unknown_00E8 > 34) {
          particle->alpha = std::max(particle->alpha - 15, 0);
        }
      } else {
        particle->SetSprite("이팩트\\굴뚝연기.spr");
        particle->SetAction("이팩트\\굴뚝연기.act");
        particle->scale = 1.5f;
        spin = 0.75f;
        particle->frameDelay = static_cast<float>(std::max(rand() % 3, 0) + 3) / 10.0f;
        particle->unknown_00E8 = 0;
        particle->velocity.x = 0.0f;
        particle->velocity.y = 0.0f;
        particle->velocity.z = 0.0f;
        particle->flags = 1;
        particle->alpha = 255;
        rotation.IsIdentity();
        rotation.MakeXRotation(static_cast<float>(std::max(rand() % 30, 0) + 75) / 180.0f * 3.141592f);
        rotation.AppendYRotation(static_cast<float>(std::max(rand() % 360, 0)) / 180.0f * 3.141592f);
        this->visibleCount = static_cast<int>(RandomRange(1.0f, static_cast<float>(this->particles.size())));
      }

      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(particle->velocity.x + 0.0f, particle->velocity.y - 9.0f, particle->velocity.z + 0.0f);
      particle->transform = translate;
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B76D80
// FUNCTION: fn_19180
void CSmokeEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it && (*it)->unknown_000C < this->visibleCount) {
      (*it)->sub_B40F30(1);
    }
  }
}
