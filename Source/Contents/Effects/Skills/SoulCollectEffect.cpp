#include "Contents/Effects/Skills/SoulCollectEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cmath>

// OFFSET: 0x0072A210
// FUNCTION: fn_07455
CSoulCollectEffect::CSoulCollectEffect() : CEffect(-1) {
  this->rotationParameter = 0;
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
void CSoulCollectEffect::Open() {
  const int count = this->effectLevel;
  if (count < 0) {
    return;
  }

  const int current = static_cast<int>(this->particles.size() / 2);
  if (current < count) {
    for (int index = 0; index < this->effectLevel - current; ++index) {
      for (int layer = 0; layer < 2; ++layer) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        const int column = static_cast<int>(this->particles.size() / 2);
        particle->SetTexture("effect\\soul_collect\\soul_collect.bmp");
        particle->effectId = this->id;
        particle->columnIndex = column;
        particle->rowIndex = layer;
        particle->groupIndex = 1;
        particle->alpha = 0;
        if (layer == 0) {
          particle->depthOffset = 0.00001f;
        } else if (layer == 1) {
          particle->depthOffset = 0.00002f;
        }
        particle->InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);
      }
    }
  } else if (current > count) {
    for (int index = 0; index < current - this->effectLevel; ++index) {
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
bool CSoulCollectEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0072A800
// FUNCTION: fn_07463
void CSoulCollectEffect::OnProcess() {
  float& rotation = reinterpret_cast<float&>(this->rotationParameter);

  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const unsigned int pairs = this->particles.size() / 2;
    const int index = particle->columnIndex;
    if (index >= static_cast<int>(pairs - 5)) {
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

      if (particle->rowIndex == 1) {
        particle->scale *= 0.5f;
        particle->SetColor(255, 255, 255);
        maxAlpha = 255;
      }

      matrix rotationMatrix;
      rotationMatrix.MakeZRotation(3.141592f);
      rotationMatrix.AppendXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
      matrix scaling;
      scaling.SetScale(particle->scale, particle->scale, 1.0f);
      matrix result;
      result.Compose(scaling, rotationMatrix);

      particle->alpha = std::min(maxAlpha, particle->alpha + 2);
      particle->transform = result;

      const int step = 360 / std::min(5u, pairs);
      const float angle = (static_cast<float>(step * (index - group * 5)) + rotation) / 180.0f * 3.141592f;
      const float cosine = ClientCos(angle) * 10.0f;
      const float sine = ClientSin(angle) * 10.0f;
      const float y = this->position[1] - 10.0f;
      const float z = cosine + this->position[2];
      const float x = sine + this->position[0];
      particle->position.y = y;
      particle->position.z = z;
      particle->position.x = x;
    } else {
      particle->alpha = 0;
    }
  }

  rotation += 0.5f;
  if (rotation > 360.0f) {
    rotation -= 360.0f;
  }
}

// OFFSET: 0x0072AD90
// FUNCTION: fn_07464
void CSoulCollectEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->alpha > 0) {
      const float left = g_textureOffsetX / particle->texture->TextureWidth();
      const float right = (particle->texture->Width() + g_textureOffsetX) / particle->texture->TextureWidth();
      const float top = g_textureOffsetY / particle->texture->TextureHeight();
      const float bottom = (particle->texture->Height() + g_textureOffsetY) / particle->texture->TextureHeight();

      particle->Render(false, top, bottom, left, right, false, true);
    }
  }
}
