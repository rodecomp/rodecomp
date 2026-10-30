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
CPowderingEffect::CPowderingEffect() : CEffect(-1), unknown_00E4(0) {
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
void CPowderingEffect::Slot01() {
  if (this->unknown_00B0 == 1 || this->unknown_00B0 == 2) {
    this->unknown_00E4 = 3;
  } else if (this->unknown_00B0 == 3 || this->unknown_00B0 == 4) {
    this->unknown_00E4 = 5;
  } else if (this->unknown_00B0 == 5) {
    this->unknown_00E4 = 7;
  }

  const int smokeCount = this->unknown_00E4 * this->unknown_00E4;
  for (int i = 0; i < smokeCount; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetTexture("smoke2.bmp");

    const int span1 = this->unknown_00E4 * 5;
    int r1 = 0;
    if (span1 != 0) {
      r1 = std::max(rand() % span1, 0);
    }
    const int r1Final = r1 - span1 / 2 + 1;

    const int span2 = this->unknown_00E4 * 5;
    int r2 = 0;
    if (span2 != 0) {
      r2 = std::max(rand() % span2, 0);
    }
    const int r2Final = r2 - span2 / 2 + 1;

    particle->unknown_00D4 = 0.0f;
    particle->unknown_00D0 = static_cast<float>(r1Final);
    particle->unknown_00D8 = static_cast<float>(r2Final);

    this->sub_7096B0(particle);
    InitParticleQuad(particle->vertices);
    this->smokeParticles.push_back(particle);
  }

  const int spriteCount = this->unknown_00E4 * this->unknown_00E4;
  for (int i = 0; i < spriteCount; ++i) {
    CSpriteParticle* particle = static_cast<CSpriteParticle*>(
      CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1)
    );
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetSprite("이팩트\\particle2.spr");
    particle->SetAction("이팩트\\particle2.act");

    this->sub_709570(particle);
    InitParticleQuad(particle->vertices);
    this->spriteParticles.push_back(particle);
  }
}

// OFFSET: 0x00709570
// FUNCTION: fn_07090
void CPowderingEffect::sub_709570(CParticle* particle) {
  CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);

  const int span1 = this->unknown_00E4 * 5;
  int r1 = 0;
  if (span1 != 0) {
    r1 = std::max(rand() % span1, 0);
  }
  const int r1Final = r1 - span1 / 2 + 1;

  const int yVal = std::max(rand() % 7, 0) + 7;

  const int span2 = this->unknown_00E4 * 5;
  int r2 = 0;
  if (span2 != 0) {
    r2 = std::max(rand() % span2, 0);
  }
  const int r2Final = r2 - span2 / 2 + 1;

  spriteParticle->velocity.y = -static_cast<float>(yVal);
  spriteParticle->velocity.x = static_cast<float>(r1Final);
  spriteParticle->velocity.z = static_cast<float>(r2Final);

  const int r3 = std::max(rand() % 3, 0) + 1;

  spriteParticle->alpha = 0;
  spriteParticle->unknown_00A8 = 0;
  spriteParticle->scale = g_depthAverageHalf - static_cast<float>(r3) * 0.1f;
}

// OFFSET: 0x00709690
// FUNCTION: fn_07091
void CPowderingEffect::Slot12() {
  if (this->id == 1104) {
    this->AddEvent(0, "effect\\su_cn_powdering.wav", 0, 0);
  }
}

// OFFSET: 0x007096B0
// FUNCTION: fn_07092
void CPowderingEffect::sub_7096B0(CParticle* particle) {
  const int r1 = rand() % 50;
  particle->scale = 0.0f;
  const int r2 = rand() % 21;
  particle->alpha = 0;

  const int r2Clamped = std::max(r2, 0);
  particle->unknown_0050 = r2Clamped + 30;

  const int r1Clamped = std::max(r1, 0);
  particle->unknown_00C0 =
    (static_cast<float>(r1Clamped + 1) * 0.1f + 6.0f) / static_cast<float>(particle->unknown_0050 * 2);

  const int r3 = std::max(rand() % 100, 0);
  const int targetAlpha = r3 + 50;
  *reinterpret_cast<int*>(particle->unknown_0048) = targetAlpha;
  *reinterpret_cast<float*>(particle->unknown_0058) = static_cast<float>(targetAlpha / particle->unknown_0050);

  const int r4 = rand() % 100;
  const int r5 = rand() % 50;
  const short s4 = static_cast<short>(std::max(r4, 0));
  const short s5 = static_cast<short>(std::max(r5, 0));
  particle->SetColor(115 - s5, 255, 150 - s4);
  particle->unknown_00A8 = 0;
}

// OFFSET: 0x007097B0
// FUNCTION: fn_07093
bool CPowderingEffect::Slot06() {
  for (CParticle* particle : this->smokeParticles) {
    if (particle->unknown_00A8 != 2) {
      return true;
    }
  }

  for (CParticle* particle : this->spriteParticles) {
    if (particle->unknown_00A8 != 2) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00709800
// FUNCTION: fn_07094
void CPowderingEffect::Slot03() {
  for (CParticle* particle : this->smokeParticles) {
    if (!particle || particle->unknown_00A8 == 2) {
      continue;
    }

    const int targetAlpha = *reinterpret_cast<int*>(particle->unknown_0048);
    const float scale = particle->unknown_00C0 + particle->scale;
    int alpha = particle->alpha;
    const int step = targetAlpha / particle->unknown_0050;

    if (particle->unknown_00A8 == 0) {
      alpha += step;
      if (alpha >= targetAlpha) {
        alpha = targetAlpha;
        particle->unknown_00A8 = 1;
      }
    } else if (particle->unknown_00A8 == 1) {
      alpha -= step;
      if (alpha < 0) {
        if (this->ActorExists()) {
          particle->unknown_00A8 = 0;
          this->sub_7096B0(particle);
          continue;
        } else {
          alpha = 0;
          particle->unknown_00A8 = 2;
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

    particle->position.x = this->position[0] + 0.0f + particle->unknown_00D0;
    particle->position.y = this->position[1] - 10.0f + particle->unknown_00D4;
    particle->position.z = this->position[2] + 0.0f + particle->unknown_00D8;
    ++particle->unknown_00E8;
  }

  for (CParticle* particle : this->spriteParticles) {
    if (!particle || particle->unknown_00A8 == 2) {
      continue;
    }

    int alpha = particle->alpha;
    if (particle->unknown_00A8 == 0) {
      alpha += std::max(rand() % 30, 0);
      if (alpha >= 255) {
        alpha = 254;
        particle->unknown_00A8 = 1;
      }
    } else if (particle->unknown_00A8 == 1) {
      alpha -= std::max(rand() % 30, 0);
      if (alpha < 0) {
        if (this->ActorExists()) {
          particle->unknown_00A8 = 0;
          this->sub_709570(particle);
          continue;
        } else {
          alpha = 0;
          particle->unknown_00A8 = 2;
        }
      }
    }

    CSpriteParticle* spriteParticle = static_cast<CSpriteParticle*>(particle);
    vector3d velocity = spriteParticle->velocity;
    particle->alpha = alpha;
    velocity.y += static_cast<float>(std::max(rand() % 10, 0)) * 0.01f;
    spriteParticle->velocity = velocity;
    ++particle->unknown_00E8;
    particle->position.x = velocity.x + this->position[0];
    particle->position.y = velocity.y + this->position[1];
    particle->position.z = velocity.z + this->position[2];
  }
}

// OFFSET: 0x00709E40
// FUNCTION: fn_07095
void CPowderingEffect::Slot04() {
  for (CParticle* particle : this->smokeParticles) {
    if (particle && particle->unknown_00A8 != 2) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (CParticle* particle : this->spriteParticles) {
    if (particle && particle->unknown_00A8 != 2) {
      static_cast<CSpriteParticle*>(particle)->sub_B40F30(1);
    }
  }
}
