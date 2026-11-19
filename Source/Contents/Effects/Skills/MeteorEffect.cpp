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

// OFFSET: 0x007074E0
// FUNCTION: fn_46197
__forceinline void CMeteorEffect::CreateMeteor() {
  CParticle* particle = CreateSpriteParticle(&this->positionVector, 1);
  if (!particle) {
    return;
  }

  CSpriteParticle* sprite = static_cast<CSpriteParticle*>(particle);

  sprite->effectId = this->id;
  sprite->columnIndex = 0;
  sprite->rowIndex = 0;
  sprite->groupIndex = 1;
  sprite->SetSprite("이팩트\\su_cn_meteor\\su_cn_meteor.spr");
  sprite->SetAction("이팩트\\su_cn_meteor\\su_cn_meteor.act");
  sprite->renderOptions |= 0x200;
  sprite->frameDelay = 4.0f;
  sprite->depthOffset = 0.00015f;
  sprite->loopMode = 1;
  if (this->id == EF_UNK_1106) {
    sprite->alpha = 100;
  }

  this->particles.push_back(particle);
}

// OFFSET: 0x007075A0
// FUNCTION: fn_46198
__forceinline void CMeteorEffect::CreateMeteorShower() {
  const long long counts[5] = {3, 3, 5, 5, 7};
  const long long count = counts[std::min<unsigned int>(this->effectLevel, 5) - 1];
  const long long span = count * 5;

  for (long long i = 0; i < count; ++i) {
    const int range = static_cast<int>(span) + 1;
    const float halfSpan = static_cast<float>(span) * 0.5f;
    vector3d pos = this->positionVector;
    pos.x = static_cast<float>(RandomIndex(range)) - halfSpan + pos.x;
    pos.z = static_cast<float>(RandomIndex(range)) - halfSpan + pos.z;

    CParticle* particle = CreateSpriteParticle(&pos, 1);
    if (!particle) {
      break;
    }

    CSpriteParticle* sprite = static_cast<CSpriteParticle*>(particle);

    sprite->effectId = this->id;
    sprite->columnIndex = static_cast<int>(i);
    sprite->rowIndex = 0;
    sprite->groupIndex = 1;
    sprite->SetSprite("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.spr");
    sprite->SetAction("이팩트\\su_lunaticcarrotbeat\\su_lunaticcarrotbeat.act");
    sprite->renderOptions |= 0x200;
    sprite->frameDelay = 4.0f;
    sprite->depthOffset = 0.00015f;
    sprite->loopMode = 1;
    if (this->id == EF_UNK_1099) {
      sprite->alpha = 100;
    }

    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00707810
// FUNCTION: fn_07056
void CMeteorEffect::Open() {
  switch (this->id) {
    case EF_UNK_1099:
    case EF_UNK_1111:
      this->CreateMeteorShower();
      break;
    case EF_UNK_1106:
    case EF_UNK_1112:
      this->CreateMeteor();
      break;
  }
}

// OFFSET: 0x00707B40
// FUNCTION: fn_07057
bool CMeteorEffect::IsAlive() {
  return !this->particles.empty();
}

// OFFSET: 0x00707C40
// FUNCTION: fn_46199
__forceinline void CMeteorEffect::UpdateParticle(CSpriteParticle* particle) {
  if (particle) {
    particle->UpdateFrame();
    if (this->quakeFrame == static_cast<long long>(particle->frame)) {
      g_modeMgr.GetGameMode()->SendMsg(MM_QUAKE, 0, 0, 0, 0);
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00707B50
// FUNCTION: fn_07058
void CMeteorEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ) {
    if (IsParticleAlive(*it) == true) {
      this->UpdateParticle(static_cast<CSpriteParticle*>(*it));
      it++;
    } else {
      FreeParticle(*it);
      it = this->particles.erase(it);
    }
  }
}

// OFFSET: 0x00707D00
// FUNCTION: fn_46200
__forceinline void CMeteorEffect::RenderParticle(CSpriteParticle* particle) {
  if (particle) {
    particle->Draw(0);
  }
}

// OFFSET: 0x00707CA0
// FUNCTION: fn_07059
void CMeteorEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (IsParticleAlive(*it) == true) {
      this->RenderParticle(static_cast<CSpriteParticle*>(particle));
    }
  }
}
