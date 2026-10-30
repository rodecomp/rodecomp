#include "Contents/Effects/SkillEffects.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/MapSkyMgr.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x007077D0
// FUNCTION: fn_07055
void CMeteorEffect::Close() {
  for (CParticle* particle : this->unknown_00CC) {
    FreeParticle(particle);
  }

  this->unknown_00CC.clear();
}

// OFFSET: 0x00707810
// FUNCTION: fn_07056
void CMeteorEffect::Slot01() {
  switch (this->id) {
    case 1099:
    case 1111: {
      const long long counts[6] = {0, 3, 3, 5, 5, 7};
      unsigned int level = this->unknown_00B0;
      if (level > 5) {
        level = 5;
      }

      const long long count = counts[level];
      const int span = static_cast<int>(count * 5);
      const float halfSpan = static_cast<float>(span) * g_depthAverageHalf;
      const int spanPlus1 = span + 1;

      for (long long i = 0; i < count; ++i) {
        vector3d pos;
        const int r1 = spanPlus1 != 0 ? std::max(rand() % spanPlus1, 0) : 0;
        pos.x = static_cast<float>(r1) - halfSpan + this->position[0];
        pos.y = this->position[1];
        const int r2 = spanPlus1 != 0 ? std::max(rand() % spanPlus1, 0) : 0;
        pos.z = static_cast<float>(r2) - halfSpan + this->position[2];

        CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(&pos, 1));
        if (!particle) {
          break;
        }

        particle->unknown_0004 = this->id;
        particle->unknown_000C = static_cast<int>(i);
        particle->unknown_0010 = 0;
        particle->unknown_0008 = 1;
        particle->SetSprite("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.spr");
        particle->SetAction("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.act");
        particle->unknown_00EC |= 0x200;
        particle->frameDelay = 4.0f;
        particle->depthOffset = 0.00015f;
        particle->loopMode = 1;
        if (this->id == 1099) {
          particle->alpha = 100;
        }

        this->unknown_00CC.push_back(particle);
      }
      break;
    }

    case 1106:
    case 1112: {
      CSpriteParticle* particle = static_cast<CSpriteParticle*>(
        CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1)
      );
      if (!particle) {
        return;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = 0;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->SetSprite("이팩트\\su_cn_meteor\\su_cn_meteor.spr");
      particle->SetAction("이팩트\\su_cn_meteor\\su_cn_meteor.act");
      particle->unknown_00EC |= 0x200;
      particle->frameDelay = 4.0f;
      particle->depthOffset = 0.00015f;
      particle->loopMode = 1;
      if (this->id == 1106) {
        particle->alpha = 100;
      }

      this->unknown_00CC.push_back(particle);
      break;
    }

    default:
      break;
  }
}

// OFFSET: 0x00707B40
// FUNCTION: fn_07057
bool CMeteorEffect::Slot06() {
  return !this->unknown_00CC.empty();
}

// OFFSET: 0x00707B50
// FUNCTION: fn_07058
void CMeteorEffect::Slot03() {
  for (auto it = this->unknown_00CC.begin(); it != this->unknown_00CC.end(); ) {
    CParticle* particle = *it;
    if (particle->life == 0.0f || static_cast<float>(particle->unknown_00E8) < particle->life * 60.0f) {
      if (particle) {
        CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);
        spriteParticle->UpdateFrame();
        if (static_cast<long long>(spriteParticle->frame) == *reinterpret_cast<const long long*>(&this->unknown_00D8)) {
          g_modeMgr.GetGameMode()->SendMsg(122, 0, 0, 0, 0);
        }
        ++particle->unknown_00E8;
      }
      ++it;
    } else {
      FreeParticle(particle);
      it = this->unknown_00CC.erase(it);
    }
  }
}

// OFFSET: 0x00707CA0
// FUNCTION: fn_07059
void CMeteorEffect::Slot04() {
  for (CParticle* particle : this->unknown_00CC) {
    if ((particle->life == 0.0f || particle->life * 60.0f > static_cast<float>(particle->unknown_00E8)) && particle) {
      static_cast<CSpriteParticle*>(particle)->sub_B40F30(0);
    }
  }
}
