#include "Contents/Effects/SoulCollectEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cmath>

// OFFSET: 0x0072A210
// FUNCTION: fn_07455
CSoulCollectEffect::CSoulCollectEffect() : CEffect(-1) {
  this->unknown_0050 = 0;
}

// OFFSET: 0x0072A2A0
// FUNCTION: fn_07456
CSoulCollectEffect::~CSoulCollectEffect() {
}

// OFFSET: 0x0072A580
// FUNCTION: fn_07459
void CSoulCollectEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0072A5F0
// FUNCTION: fn_07460
void CSoulCollectEffect::Slot01() {
  const int count = this->unknown_00B0;
  if (count < 0) {
    return;
  }

  const int current = static_cast<int>(this->particles.size() / 2);
  if (current < count) {
    for (int index = 0; index < this->unknown_00B0 - current; ++index) {
      for (int layer = 0; layer < 2; ++layer) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->SetTexture("effect\\soul_collect\\soul_collect.bmp");
        particle->unknown_0004 = this->id;
        particle->unknown_000C = static_cast<int>(this->particles.size() / 2);
        particle->unknown_0010 = layer;
        particle->unknown_0008 = 1;
        particle->alpha = 0;
        if (layer == 0) {
          particle->depthOffset = 0.00001f;
        } else if (layer == 1) {
          particle->depthOffset = 0.00002f;
        }
        InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);
      }
    }
  } else if (current > count) {
    for (int index = 0; index < current - this->unknown_00B0; ++index) {
      if (this->particles.size() != 0) {
        for (int layer = 0; layer < 2; ++layer) {
          FreeParticle(this->particles.back());
          this->particles.pop_back();
        }
      }
    }
  }
}

// OFFSET: 0x0072A7F0
// FUNCTION: fn_07462
bool CSoulCollectEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0072A800
// FUNCTION: fn_07463
void CSoulCollectEffect::Slot03() {
  float& rotation = reinterpret_cast<float&>(this->unknown_0050);

  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const unsigned int pairs = this->particles.size() / 2;
    const int index = particle->unknown_000C;
    if (index < static_cast<int>(pairs - 5)) {
      particle->alpha = 0;
      continue;
    }

    int maxAlpha = 255;
    const int group = index / 5;
    switch (group) {
    case 0:
      particle->scale = 2.0f;
      particle->SetColor(0, 255, 0);
      maxAlpha = 150;
      break;
    case 1:
      particle->scale = 3.5f;
      particle->SetColor(0, 255, 0);
      maxAlpha = 150;
      break;
    case 2:
      particle->scale = 3.5f;
      particle->SetColor(255, 0, 0);
      maxAlpha = 255;
      break;
    case 3:
      particle->scale = 5.5f;
      particle->SetColor(255, 0, 0);
      maxAlpha = 255;
      break;
    }

    if (particle->unknown_0010 == 1) {
      particle->scale *= 0.5f;
      particle->SetColor(255, 255, 255);
      maxAlpha = 255;
    }

    matrix rotationMatrix;
    rotationMatrix.MakeZRotation(3.1415927f);
    rotationMatrix.AppendXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
    matrix scaling;
    scaling.SetScale(particle->scale, particle->scale, 1.0f);
    matrix result;
    result.Compose(scaling, rotationMatrix);

    particle->alpha = std::min(particle->alpha + 2, maxAlpha);
    particle->transform = result;

    const int step = 360 / std::min(pairs, 5u);
    const float angle = (static_cast<float>(step * (index - group * 5)) + rotation) / 180.0f * 3.141592f;
    const float cosine = std::cos(angle) * 10.0f;
    const float sine = std::sin(angle) * 10.0f;
    particle->position.y = this->position[1] - 10.0f;
    particle->position.z = cosine + this->position[2];
    particle->position.x = sine + this->position[0];
  }

  rotation += 0.5f;
  if (rotation > 360.0f) {
    rotation -= 360.0f;
  }
}

// OFFSET: 0x0072AD90
// FUNCTION: fn_07464
void CSoulCollectEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->alpha > 0) {
      CTexture* texture = particle->texture;
      const float left = g_textureOffsetX / texture->TextureWidth();
      const float right = (texture->Width() + g_textureOffsetX) / texture->TextureWidth();
      const float top = g_textureOffsetY / texture->TextureHeight();
      const float bottom = (texture->Height() + g_textureOffsetY) / particle->texture->TextureHeight();
      particle->Render(false, top, bottom, left, right, false, true);
    }
  }
}
