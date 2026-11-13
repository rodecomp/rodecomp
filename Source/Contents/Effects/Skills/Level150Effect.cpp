#include "Contents/Effects/Skills/Level150Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x00B463F0
// FUNCTION: fn_18906
CLevel150Effect::CLevel150Effect() : CEffect(-1) {
}

// OFFSET: 0x00B465A0
// FUNCTION: fn_18908
void CLevel150Effect::SetDestinationBlend(int blend) {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }
}

// OFFSET: 0x00B46600
// FUNCTION: fn_18909
void CLevel150Effect::SetColor(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B46680
// FUNCTION: fn_18910
void CLevel150Effect::AddBubbleRing(int alphaOffset) {
  const int effectId = this->id;
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture(FormatOwnedString("effect\\W_bubble%02d.tga", segment + 1).c_str());
      particle->effectId = this->id;
      particle->rowIndex = ring;
      particle->columnIndex = segment;
      particle->groupIndex = 4;
      particle->alpha = alphaOffset + (3 - ring) * 45;

      if (effectId == 978) {
        particle->SetColor(100, 100, 255);
      } else if (effectId == 1022) {
        particle->SetColor(255, 255, 100);
      } else if (effectId == 1226) {
        particle->SetColor(255, 155, 155);
      } else if (effectId == 2275) {
        particle->SetColor(255, 155, 0);
      }

      particle->angle = static_cast<float>((ring + 2) * 90 % 360);
      particle->angleParameter = 360;
      particle->speedParameter = 30.0f - static_cast<float>(ring);
      particle->scale = 3.5f;
      particle->phaseParameter = 80;
      particle->animationParameter = 0.0f;
      particle->elapsedFrames = 0;

      const int index = particle->columnIndex;
      const float radius = particle->speedParameter * g_sinTable[index * 9] * 0.15f + particle->speedParameter;
      const int direction =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * index) +
                         particle->angle) %
        360;
      const int tilt = particle->phaseParameter;
      const float spread = radius * g_cosTable[tilt];
      const float height = radius * g_sinTable[tilt];
      const float cosine = g_cosTable[direction];
      const float sine = g_sinTable[direction];
      const float offsetX = spread * cosine;
      const float offsetZ = spread * sine;

      EffectPoint points[2];
      const float innerX = cosine * particle->scale;
      const float innerY = 5.0f;
      const float innerZ = sine * particle->scale;
      const float outerX = innerX + offsetX;
      const float outerY = 5.0f - height;
      const float outerZ = innerZ + offsetZ;

      particle->vertices[3].x = innerX;
      particle->vertices[3].y = innerY;
      particle->vertices[3].z = innerZ;
      particle->vertices[1].x = outerX;
      particle->vertices[1].y = outerY;
      particle->vertices[1].z = outerZ;

      if (index != 0) {
        particle->vertices[0].x = this->lastPoints[1].x;
        particle->vertices[0].y = this->lastPoints[1].y;
        particle->vertices[0].z = this->lastPoints[1].z;
        particle->vertices[2].x = this->lastPoints[0].x;
        particle->vertices[2].y = this->lastPoints[0].y;
        particle->vertices[2].z = this->lastPoints[0].z;
      }

      this->lastPoints[0].x = innerX;
      this->lastPoints[0].y = innerY;
      this->lastPoints[0].z = innerZ;
      this->lastPoints[1].x = outerX;
      this->lastPoints[1].y = outerY;
      this->lastPoints[1].z = outerZ;
      this->bubbles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B469F0
// FUNCTION: fn_18911
void CLevel150Effect::AddShock(int kind) {
  const int effectId = this->id;
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 13);
    if (particle) {
      particle->effectId = this->id;
      particle->columnIndex = index;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->phaseParameter = index * 180;
      particle->alpha = 200;
      particle->animationParameter = -1.0f;
      particle->elapsedFrames = 0;

      if (kind == 3) {
        particle->SetTexture("effect\\cir0002.tga");
        particle->animationParameters[0] = 18.0f;
      } else {
        particle->SetTexture("effect\\emp shock.tga");
        particle->animationParameters[0] = 11.0f;
      }

      this->shocks.push_back(particle);

      if (effectId == 978) {
        if (kind == 3) {
          particle->SetColor(35, 35, 255);
        } else {
          particle->SetColor(105, 105, 255);
        }
      } else if (effectId == 1022) {
        if (kind == 3) {
          particle->SetColor(255, 255, 35);
        } else {
          particle->SetColor(255, 255, 105);
        }
      } else if (effectId == 1226) {
        if (kind == 3) {
          particle->SetColor(255, 55, 55);
        } else {
          particle->SetColor(255, 155, 155);
        }
      } else if (effectId == 2275) {
        particle->SetColor(255, 155, 0);
      }
    }
  }
}

// OFFSET: 0x00B46D10
// FUNCTION: fn_18912
void CLevel150Effect::Close() {
  for (auto it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    FreeParticle(*it);
  }

  this->bubbles.clear();

  for (auto it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    FreeParticle(*it);
  }

  this->shocks.clear();
}

// OFFSET: 0x00B46D80
// FUNCTION: fn_18913
void CLevel150Effect::Open() {
  this->AddBubbleRing(45);
  this->AddBubbleRing(25);
  this->AddShock(2);
  this->AddShock(3);
}

// OFFSET: 0x00B46DB0
// FUNCTION: fn_18914
bool CLevel150Effect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B46F00
// FUNCTION: fn_18915
void CLevel150Effect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      int tilt = particle->phaseParameter - 1;
      float scale = particle->scale + 0.07f;
      int alpha = particle->alpha;
      if (tilt < 10) {
        tilt = 10;
        alpha = 0;
      }

      if (scale >= 3.5f) {
        alpha -= 3;
        if (alpha <= 0) {
          tilt = 74;
          scale = 2.87f;
        }
        alpha = std::max(0, alpha);
      } else {
        alpha += 10;
      }

      particle->alpha = alpha;
      particle->scale = scale;
      particle->phaseParameter = tilt;

      const float radius = particle->speedParameter;
      const int segment = particle->columnIndex;
      matrix transform;
      std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
      transform.SetScale(scale * 0.5f, radius * g_sinTable[tilt] / (radius * g_sinTable[segment * 9] * 0.15f + radius), scale * 0.5f);
      particle->transform = transform;
    }

    ++particle->elapsedFrames;
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      int tilt = particle->phaseParameter + 3;
      if (tilt >= 360) {
        tilt -= 360;
      }
      particle->phaseParameter = tilt;

      const float size = particle->animationParameters[0];
      const float scale = size * 0.1f * g_sinTable[tilt] + 1.0f + size * 0.8f;
      const int segment = particle->columnIndex;
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const int yaw = static_cast<int>(mode->camera->camera.yaw);

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(90.0f / 180.0f * 3.141592f);
      rotation.AppendYRotation(static_cast<float>(segment * 23) / 180.0f * 3.141592f - 160.0f / 180.0f * 3.141592f -
                               static_cast<float>(yaw) / 180.0f * 3.141592f);
      matrix scaling;
      std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
      scaling.SetScale(scale, scale, 1.0f);
      matrix result;
      result.Compose(scaling, rotation);
      particle->transform = result;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B474C0
// FUNCTION: fn_18916
void CLevel150Effect::Render() {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->columnIndex <= 0) {
      continue;
    }

    const int segment = particle->columnIndex;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment) / 21.0f, static_cast<float>(segment + 1) / 21.0f, false, true};
    particle->SetTexture(FormatOwnedString("effect\\W_bubble%02d.tga", particle->elapsedFrames % 27 + 1).c_str());
    particle->Render(quad);
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B47670
// FUNCTION: fn_18917
CLevel150SubEffect::CLevel150SubEffect() : CEffect(-1) {
}

// OFFSET: 0x00B477D0
// FUNCTION: fn_18919
void CLevel150SubEffect::SetDestinationBlend(int blend) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }
}

// OFFSET: 0x00B47810
// FUNCTION: fn_18920
void CLevel150SubEffect::SetColor(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B47850
// FUNCTION: fn_18921
void CLevel150SubEffect::Init() {
  const int red[4][3] = {{255, 105, 105}, {255, 5, 5}, {255, 55, 55}, {255, 155, 155}};
  const int yellow[4][3] = {{255, 255, 105}, {255, 255, 5}, {255, 255, 55}, {255, 255, 155}};
  const int blue[4][3] = {{105, 105, 255}, {5, 5, 255}, {55, 55, 255}, {155, 155, 255}};
  const int effectId = this->id;
  for (int ring = 0; ring < 4; ++ring) {
    for (int index = 0; index < 2; ++index) {
      CParticle* particle = CreateParticle(&this->positionVector, 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\emp shock.tga");
      particle->angle = static_cast<float>(std::ranges::max(rand() % 360, 0));
      particle->alpha = 200;
      particle->animationParameter = 10.0f;
      particle->animationParameters[0] = 10.0f;
      particle->effectId = this->id;
      particle->columnIndex = index;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->elapsedFrames = -20 * ring;
      this->particles.push_back(particle);

      if (effectId == 979) {
        particle->SetColor(blue[ring][0], blue[ring][1], blue[ring][2]);
      } else if (effectId == 1023) {
        particle->SetColor(yellow[ring][0], yellow[ring][1], yellow[ring][2]);
      } else if (effectId == 1227) {
        particle->SetColor(red[ring][0], red[ring][1], red[ring][2]);
      } else if (effectId == 2276) {
        particle->SetColor(255, 155, 0);
      }
    }
  }
}

// OFFSET: 0x00B47A90
// FUNCTION: fn_18922
void CLevel150SubEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B47AD0
// FUNCTION: fn_18923
void CLevel150SubEffect::Open() {
  this->Init();
}

// OFFSET: 0x00B47AE0
// FUNCTION: fn_18924
bool CLevel150SubEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B47AF0
// FUNCTION: fn_18925
void CLevel150SubEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle && particle->elapsedFrames > 0) {
      float size = particle->animationParameter * 1.02;
      if (size > 20.0f) {
        int alpha = particle->alpha - 3;
        if (alpha <= 0) {
          size = 10.0f;
          particle->angle = static_cast<float>(std::ranges::max(rand() % 360, 0));
          alpha = 200;
        }
        particle->alpha = alpha;
      }
      particle->animationParameter = size;

      const int angle = static_cast<int>(particle->angle);
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const int yaw = static_cast<int>(mode->camera->camera.yaw);
      const float scale = particle->animationParameter;
      const float height = particle->animationParameters[0];

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f - static_cast<float>(yaw) / 180.0f * 3.141592f);
      matrix scaling;
      std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
      scaling.SetScale(scale * 0.5f, scale * 0.5f, 1.0f);
      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(0.0f, -height, 0.0f);
      matrix first;
      first.Compose(scaling, rotation);
      matrix result;
      result.Compose(first, translate);
      particle->transform = result;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B47FD0
// FUNCTION: fn_18926
void CLevel150SubEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}
