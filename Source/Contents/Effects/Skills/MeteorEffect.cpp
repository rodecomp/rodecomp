#include "Contents/Effects/Skills/SkillEffects.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Mode/ModeMessage.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Contents/Map/MapSkyMgr.h"

#include <algorithm>
#include <cstdlib>

namespace {
// INLINED
__forceinline int RandomIndex(int range) {
  if (range == 0) {
    return 0;
  }
  return std::ranges::max(rand() % range, 0);
}

// INLINED
__forceinline bool IsParticleAlive(const CParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x007077D0
// FUNCTION: fn_07055
void CMeteorEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00707810
// FUNCTION: fn_07056
void CMeteorEffect::Open() {
  switch (this->id) {
    case EF_UNK_1099:
    case EF_UNK_1111: {
      const long long counts[5] = {3, 3, 5, 5, 7};
      unsigned int level = this->effectLevel;
      if (level > 5) {
        level = 5;
      }

      const long long count = counts[level - 1];
      const float halfSpan = static_cast<float>(count * 5) * g_depthAverageHalf;
      const int range = static_cast<int>(count) * 5 + 1;

      for (long long i = 0; i < count; ++i) {
        vector3d pos = this->positionVector;
        pos.x = static_cast<float>(RandomIndex(range)) - halfSpan + pos.x;
        pos.z = static_cast<float>(RandomIndex(range)) - halfSpan + pos.z;

        CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(&pos, 1));
        if (!particle) {
          break;
        }

        particle->effectId = this->id;
        particle->columnIndex = static_cast<int>(i);
        particle->rowIndex = 0;
        particle->groupIndex = 1;
        particle->SetSprite("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.spr");
        particle->SetAction("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.act");
        particle->renderOptions |= 0x200;
        particle->frameDelay = 4.0f;
        particle->depthOffset = 0.00015f;
        particle->loopMode = 1;
        if (this->id == 1099) {
          particle->alpha = 100;
        }

        this->particles.push_back(particle);
      }
      break;
    }

    case EF_UNK_1106:
    case EF_UNK_1112: {
      CSpriteParticle* particle = static_cast<CSpriteParticle*>(
        CreateSpriteParticle(&this->positionVector, 1)
      );
      if (!particle) {
        return;
      }

      particle->effectId = this->id;
      particle->columnIndex = 0;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->SetSprite("이팩트\\su_cn_meteor\\su_cn_meteor.spr");
      particle->SetAction("이팩트\\su_cn_meteor\\su_cn_meteor.act");
      particle->renderOptions |= 0x200;
      particle->frameDelay = 4.0f;
      particle->depthOffset = 0.00015f;
      particle->loopMode = 1;
      if (this->id == 1106) {
        particle->alpha = 100;
      }

      this->particles.push_back(particle);
      break;
    }

    default:
      break;
  }
}

// OFFSET: 0x00707B40
// FUNCTION: fn_07057
bool CMeteorEffect::IsAlive() {
  return !this->particles.empty();
}

// OFFSET: 0x00707B50
// FUNCTION: fn_07058
void CMeteorEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ) {
    CParticle* particle = *it;
    if (IsParticleAlive(particle) == true) {
      if (particle) {
        CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);
        spriteParticle->UpdateFrame();
        if (static_cast<long long>(spriteParticle->frame) == this->quakeFrame) {
          g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
        }
        ++particle->elapsedFrames;
      }
      ++it;
    } else {
      FreeParticle(particle);
      it = this->particles.erase(it);
    }
  }
}

// OFFSET: 0x00707CA0
// FUNCTION: fn_07059
void CMeteorEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (IsParticleAlive(*it) == true && particle) {
      static_cast<CSpriteParticle*>(particle)->Draw(0);
    }
  }
}
