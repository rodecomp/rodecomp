#include "Contents/Effects/Skills/SmokeEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
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
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B76580
// FUNCTION: fn_19177
void CSmokeEffect::Open() {
  for (int i = 0; i < 4; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    if (particle) {
      particle->effectId = this->id;
      particle->columnIndex = i;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B76710
// FUNCTION: fn_19178
bool CSmokeEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B76720
// FUNCTION: fn_19179
void CSmokeEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      matrix& rotation = *reinterpret_cast<matrix*>(particle->rotationStorage);
      float& angle = particle->angle;
      float& spin = particle->spin;
      if (particle->elapsedFrames < 50) {
        const float speed = particle->frameDelay;
        const float* values = rotation.values;
        const float deltaX = values[3] * 0.0f + values[0] * 0.0f + speed * values[6] + values[9];
        const float deltaY = values[4] * 0.0f + values[1] * 0.0f + speed * values[7] + values[10];
        const float deltaZ = values[5] * 0.0f + values[2] * 0.0f + speed * values[8] + values[11];
        particle->velocity.x += deltaX;
        particle->velocity.y += deltaY;
        particle->velocity.z += deltaZ;
        angle += spin;
        if (particle->elapsedFrames > 34) {
          particle->alpha = std::ranges::max(particle->alpha - 15, 0);
        }
      } else {
        particle->SetSprite("이팩트\\굴뚝연기.spr");
        particle->SetAction("이팩트\\굴뚝연기.act");
        particle->scale = 1.5f;
        spin = 0.75f;
        particle->frameDelay = static_cast<float>(std::ranges::max(rand() % 3, 0) + 3) / 10.0f;
        particle->elapsedFrames = 0;
        particle->velocity.x = 0.0f;
        particle->velocity.y = 0.0f;
        particle->velocity.z = 0.0f;
        particle->flags = 1;
        particle->alpha = 255;
        rotation.IsIdentity();
        rotation.MakeXRotation(static_cast<float>(std::ranges::max(rand() % 30, 0) + 75) / 180.0f * 3.141592f);
        rotation.AppendYRotation(static_cast<float>(std::ranges::max(rand() % 360, 0)) / 180.0f * 3.141592f);
        this->visibleCount = static_cast<int>(RandomRange(1.0f, static_cast<float>(this->particles.size())));
      }

      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(particle->velocity.x + 0.0f, particle->velocity.y - 9.0f, particle->velocity.z + 0.0f);
      particle->transform = translate;
    }
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x00B76D80
// FUNCTION: fn_19180
void CSmokeEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it && (*it)->columnIndex < this->visibleCount) {
      (*it)->Draw(1);
    }
  }
}
