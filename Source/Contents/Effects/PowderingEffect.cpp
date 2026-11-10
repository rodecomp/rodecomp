#include "Contents/Effects/PowderingEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00708E70
// FUNCTION: fn_07085
CPowderingEffect::CPowderingEffect() : CEffect(-1), particleGridSize(0) {
}

// OFFSET: 0x007092C0
// FUNCTION: fn_07088
void CPowderingEffect::Close() {
  for (auto it = this->smokeParticles.begin(); it != this->smokeParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->smokeParticles.clear();

  for (auto it = this->spriteParticles.begin(); it != this->spriteParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->spriteParticles.clear();
}

// OFFSET: 0x00709330
// FUNCTION: fn_07089
void CPowderingEffect::Open() {
  if (this->effectLevel == 1 || this->effectLevel == 2) {
    this->particleGridSize = 3;
  } else if (this->effectLevel == 3 || this->effectLevel == 4) {
    this->particleGridSize = 5;
  } else if (this->effectLevel == 5) {
    this->particleGridSize = 7;
  }

  for (int i = 0; i < this->particleGridSize * this->particleGridSize; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;

    particle->SetTexture("smoke2.bmp");

    const int span1 = this->particleGridSize * 5;
    int r1 = 0;
    if (span1 != 0) {
      r1 = std::max(rand() % span1, 0);
    }
    const int r1Final = r1 - span1 / 2 + 1;

    const int span2 = this->particleGridSize * 5;
    int r2 = 0;
    if (span2 != 0) {
      r2 = std::max(rand() % span2, 0);
    }
    const int r2Final = r2 - span2 / 2 + 1;

    particle->offsetY = 0.0f;
    particle->offsetX = static_cast<float>(r1Final);
    particle->offsetZ = static_cast<float>(r2Final);

    this->InitSmokeParticle(particle);
    InitParticleQuad(particle->vertices);
    this->smokeParticles.push_back(particle);
  }

  for (int i = 0; i < this->particleGridSize * this->particleGridSize; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(
      CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1)
    );
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;

    particle->SetSprite("이팩트\\particle2.spr");
    particle->SetAction("이팩트\\particle2.act");

    this->InitSpriteParticle(particle);
    InitParticleQuad(particle->vertices);
    this->spriteParticles.push_back(particle);
  }
}

// OFFSET: 0x00709570
// FUNCTION: fn_07090
void CPowderingEffect::InitSpriteParticle(CParticle* particle) {
  CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);

  const int span1 = this->particleGridSize * 5;
  int r1 = 0;
  if (span1 != 0) {
    r1 = std::max(rand() % span1, 0);
  }
  const int r1Final = r1 - span1 / 2 + 1;

  const float yVal = static_cast<float>(std::max(rand() % 7, 0) + 7);

  const int span2 = this->particleGridSize * 5;
  int r2 = 0;
  if (span2 != 0) {
    r2 = std::max(rand() % span2, 0);
  }
  const int r2Final = r2 - span2 / 2 + 1;

  spriteParticle->velocity.y = -yVal;
  spriteParticle->velocity.x = static_cast<float>(r1Final);
  spriteParticle->velocity.z = static_cast<float>(r2Final);

  const int r3 = std::max(rand() % 3, 0) + 1;

  spriteParticle->alpha = 0;
  spriteParticle->animationState = 0;
  spriteParticle->scale = 0.5f - static_cast<float>(r3) * 0.1f;
}

// OFFSET: 0x00709690
// FUNCTION: fn_07091
void CPowderingEffect::InitEvents() {
  if (this->id == 1104) {
    this->AddEvent(0, "effect\\su_cn_powdering.wav", 0, 0);
  }
}

// OFFSET: 0x007096B0
// FUNCTION: fn_07092
void CPowderingEffect::InitSmokeParticle(CParticle* particle) {
  const int r1 = rand() % 50;
  particle->scale = 0.0f;
  const int r2 = rand() % 21;
  particle->alpha = 0;

  const int r2Clamped = std::max(r2, 0);
  particle->angleParameter = r2Clamped + 30;

  const int r1Clamped = std::max(r1, 0);
  particle->speedParameter =
    (static_cast<float>(r1Clamped + 1) * 0.1f + 6.0f) / static_cast<float>(particle->angleParameter * 2);

  const int r3 = std::max(rand() % 100, 0);
  const int targetAlpha = r3 + 50;
  *reinterpret_cast<int*>(particle->effectParameters) = targetAlpha;
  *reinterpret_cast<float*>(particle->animationParameters) = static_cast<float>(targetAlpha / particle->angleParameter);

  const int r4 = rand() % 100;
  const int r5 = rand() % 50;
  const short s4 = static_cast<short>(std::max(r4, 0));
  const short s5 = static_cast<short>(std::max(r5, 0));
  particle->SetColor(115 - s5, 255, 150 - s4);
  particle->animationState = 0;
}

// OFFSET: 0x007097B0
// FUNCTION: fn_07093
bool CPowderingEffect::IsAlive() {
  for (CParticle* particle : this->smokeParticles) {
    if (particle->animationState != 2) {
      return true;
    }
  }

  for (CParticle* particle : this->spriteParticles) {
    if (particle->animationState != 2) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00709800
// FUNCTION: fn_07094
void CPowderingEffect::OnProcess() {
  for (auto it = this->smokeParticles.begin(); it != this->smokeParticles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->animationState == 2) {
      continue;
    }

    const int targetAlpha = *reinterpret_cast<int*>(particle->effectParameters);
    const float scale = particle->speedParameter + particle->scale;
    int alpha = particle->alpha;
    const int step = targetAlpha / particle->angleParameter;

    if (particle->animationState == 0) {
      alpha += step;
      if (targetAlpha <= alpha) {
        alpha = targetAlpha;
        particle->animationState = 1;
      }
    } else if (particle->animationState == 1) {
      alpha -= step;
      if (alpha < 0) {
        if (!this->ActorExists()) {
          alpha = 0;
          particle->animationState = 2;
        } else {
          particle->animationState = 0;
          this->InitSmokeParticle(particle);
          continue;
        }
      }
    }

    particle->alpha = alpha;
    particle->scale = scale;

    matrix scaleMat;
    scaleMat.SetScale(scale, scale, 1.0f);

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    matrix rotation;
    rotation.MakeXRotation(mode->camera->camera.pitch / 180.0f * 3.141592f);

    matrix result;
    result.Compose(scaleMat, rotation);
    particle->transform = result;

    particle->position = {
      this->position[0] + 0.0f + particle->offsetX,
      this->position[1] - 10.0f + particle->offsetY,
      this->position[2] + 0.0f + particle->offsetZ
    };
    ++particle->elapsedFrames;
  }

  for (auto it = this->spriteParticles.begin(); it != this->spriteParticles.end(); ++it) {
    CSpriteParticle* particle = *it;
    if (!particle || particle->animationState == 2) {
      continue;
    }

    int alpha = particle->alpha;
    if (particle->animationState == 0) {
      alpha += std::max(rand() % 30, 0);
      if (alpha >= 255) {
        alpha = 254;
        particle->animationState = 1;
      }
    } else if (particle->animationState == 1) {
      alpha -= std::max(rand() % 30, 0);
      if (alpha < 0) {
        if (!this->ActorExists()) {
          alpha = 0;
          particle->animationState = 2;
        } else {
          particle->animationState = 0;
          this->InitSpriteParticle(particle);
          continue;
        }
      }
    }

    vector3d velocity = particle->velocity;
    particle->alpha = alpha;
    velocity.y += static_cast<float>(std::max(rand() % 10, 0)) * 0.01f;
    particle->velocity = velocity;
    ++particle->elapsedFrames;
    particle->position = {velocity.x + this->position[0], velocity.y + this->position[1], velocity.z + this->position[2]};
  }
}

// OFFSET: 0x00709E40
// FUNCTION: fn_07095
void CPowderingEffect::Render() {
  for (auto it = this->smokeParticles.begin(); it != this->smokeParticles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->animationState != 2) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->spriteParticles.begin(); it != this->spriteParticles.end(); ++it) {
    CSpriteParticle* particle = *it;
    if (particle && particle->animationState != 2) {
      particle->Draw(1);
    }
  }
}
