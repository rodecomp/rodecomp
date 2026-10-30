#include "Contents/Effects/Level150Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
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
void CLevel150Effect::Slot10(int blend) {
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
void CLevel150Effect::Slot08(int red, int green, int blue) {
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
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture(FormatOwnedString("effect\\W_bubble%02d.tga", segment + 1).c_str());
      particle->unknown_0004 = this->id;
      particle->unknown_0010 = ring;
      particle->unknown_000C = segment;
      particle->unknown_0008 = 4;
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

      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>((ring + 2) * 90 % 360);
      particle->unknown_0050 = 360;
      particle->unknown_00C0 = 30.0f - static_cast<float>(ring);
      particle->scale = 3.5f;
      particle->unknown_004C = 80;
      particle->unknown_0054 = 0.0f;
      particle->unknown_00E8 = 0;

      const int index = particle->unknown_000C;
      const float radius = particle->unknown_00C0 * g_sinTable[index * 9] * 0.15f + particle->unknown_00C0;
      const int direction =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * index) +
                         *reinterpret_cast<const float*>(&particle->unknown_00C4)) %
        360;
      const int tilt = particle->unknown_004C;
      const float spread = radius * g_cosTable[tilt];
      const float height = radius * g_sinTable[tilt];
      const float cosine = g_cosTable[direction];
      const float sine = g_sinTable[direction];

      EffectPoint points[2];
      points[0].x = cosine * particle->scale;
      points[0].y = 5.0f;
      points[0].z = sine * particle->scale;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = 5.0f - height;
      points[1].z = points[0].z + spread * sine;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;

      if (index != 0) {
        particle->vertices[0].x = this->lastPoints[1].x;
        particle->vertices[0].y = this->lastPoints[1].y;
        particle->vertices[0].z = this->lastPoints[1].z;
        particle->vertices[2].x = this->lastPoints[0].x;
        particle->vertices[2].y = this->lastPoints[0].y;
        particle->vertices[2].z = this->lastPoints[0].z;
      }

      this->lastPoints[0].x = points[0].x;
      this->lastPoints[0].y = points[0].y;
      this->lastPoints[0].z = points[0].z;
      this->lastPoints[1].x = points[1].x;
      this->lastPoints[1].y = points[1].y;
      this->lastPoints[1].z = points[1].z;
      this->bubbles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B469F0
// FUNCTION: fn_18911
void CLevel150Effect::AddShock(int kind) {
  const int effectId = this->id;
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = index;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->unknown_004C = index * 180;
      particle->alpha = 200;
      particle->unknown_0054 = -1.0f;
      particle->unknown_00E8 = 0;

      if (kind == 3) {
        particle->SetTexture("effect\\cir0002.tga");
        *reinterpret_cast<float*>(particle->unknown_0058) = 18.0f;
      } else {
        particle->SetTexture("effect\\emp shock.tga");
        *reinterpret_cast<float*>(particle->unknown_0058) = 11.0f;
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
  for (CParticle* particle : this->bubbles) {
    FreeParticle(particle);
  }

  this->bubbles.clear();

  for (CParticle* particle : this->shocks) {
    FreeParticle(particle);
  }

  this->shocks.clear();
}

// OFFSET: 0x00B46D80
// FUNCTION: fn_18913
void CLevel150Effect::Slot01() {
  this->AddBubbleRing(45);
  this->AddBubbleRing(25);
  this->AddShock(2);
  this->AddShock(3);
}

// OFFSET: 0x00B46DB0
// FUNCTION: fn_18914
bool CLevel150Effect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B46F00
// FUNCTION: fn_18915
void CLevel150Effect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      int tilt = particle->unknown_004C - 1;
      float scale = particle->scale + 0.07f;
      int alpha = particle->alpha;
      if (tilt < 10) {
        tilt = 10;
        alpha = 0;
      }

      if (scale < 3.5f) {
        alpha += 10;
      } else {
        alpha -= 3;
        if (alpha <= 0) {
          alpha = 0;
          tilt = 74;
          scale = 2.87f;
        }
      }

      particle->alpha = alpha;
      particle->scale = scale;
      particle->unknown_004C = tilt;

      const float radius = particle->unknown_00C0;
      const int segment = particle->unknown_000C;
      matrix transform;
      std::memcpy(transform.values, g_identityMatrix, sizeof(transform.values));
      transform.SetScale(scale * 0.5f, radius * g_sinTable[tilt] / (radius * g_sinTable[segment * 9] * 0.15f + radius), scale * 0.5f);
      particle->transform = transform;
    }

    ++particle->unknown_00E8;
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      int tilt = particle->unknown_004C + 3;
      if (tilt >= 360) {
        tilt -= 360;
      }
      particle->unknown_004C = tilt;

      const float size = *reinterpret_cast<const float*>(particle->unknown_0058);
      const float scale = size * 0.1f * g_sinTable[tilt] + 1.0f + size * 0.8f;
      const int segment = particle->unknown_000C;
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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B474C0
// FUNCTION: fn_18916
void CLevel150Effect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->unknown_000C <= 0) {
      continue;
    }

    const int segment = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment) / 21.0f, static_cast<float>(segment + 1) / 21.0f, false, true};
    particle->SetTexture(FormatOwnedString("effect\\W_bubble%02d.tga", particle->unknown_00E8 % 27 + 1).c_str());
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
void CLevel150SubEffect::Slot10(int blend) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }
}

// OFFSET: 0x00B47810
// FUNCTION: fn_18920
void CLevel150SubEffect::Slot08(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B47850
// FUNCTION: fn_18921
void CLevel150SubEffect::Init() {
  const int blue[4][3] = {{105, 105, 255}, {5, 5, 255}, {55, 55, 255}, {155, 155, 255}};
  const int yellow[4][3] = {{255, 255, 105}, {255, 255, 5}, {255, 255, 55}, {255, 255, 155}};
  const int red[4][3] = {{255, 105, 105}, {255, 5, 5}, {255, 55, 55}, {255, 155, 155}};
  const int effectId = this->id;
  for (int ring = 0; ring < 4; ++ring) {
    for (int index = 0; index < 2; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\emp shock.tga");
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(std::max(rand() % 360, 0));
      particle->alpha = 200;
      particle->unknown_0054 = 10.0f;
      *reinterpret_cast<float*>(particle->unknown_0058) = 10.0f;
      particle->unknown_0004 = this->id;
      particle->unknown_000C = index;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->unknown_00E8 = -20 * ring;
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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B47AD0
// FUNCTION: fn_18923
void CLevel150SubEffect::Slot01() {
  this->Init();
}

// OFFSET: 0x00B47AE0
// FUNCTION: fn_18924
bool CLevel150SubEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B47AF0
// FUNCTION: fn_18925
void CLevel150SubEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle && particle->unknown_00E8 > 0) {
      float size = particle->unknown_0054 * 1.02;
      if (size > 20.0f) {
        int alpha = particle->alpha - 3;
        if (alpha <= 0) {
          size = 10.0f;
          *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(std::max(rand() % 360, 0));
          alpha = 200;
        }
        particle->alpha = alpha;
      }
      particle->unknown_0054 = size;

      const int angle = static_cast<int>(*reinterpret_cast<const float*>(&particle->unknown_00C4));
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const int yaw = static_cast<int>(mode->camera->camera.yaw);
      const float scale = particle->unknown_0054;
      const float height = *reinterpret_cast<const float*>(particle->unknown_0058);

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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B47FD0
// FUNCTION: fn_18926
void CLevel150SubEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}
