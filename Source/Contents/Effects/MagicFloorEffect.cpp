#include "Contents/Effects/MagicFloorEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x00B48220
// FUNCTION: fn_18927
CMagicFloorEffect::CMagicFloorEffect() : CEffect(0x401) {
}

// OFFSET: 0x00B48400
// FUNCTION: fn_18929
void CMagicFloorEffect::AddFloors() {
  std::string textures[6] = {
    "effect\\hero_011.tga",
    "effect\\hero_001.tga",
    "effect\\hero_021.tga",
    "effect\\hero_002.tga",
    "effect\\hero_031.tga",
    "effect\\hero_003.tga",
  };

  for (int index = 0; index < 6; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture(textures[index]);
    particle->unknown_0004 = this->id;
    particle->unknown_000C = index;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->alpha = 255;
    *reinterpret_cast<float*>(&particle->unknown_00C4) = 0.0f;
    particle->unknown_004C = 0;
    particle->scale = 30.0f;
    InitParticleQuad(particle->vertices);
    this->floors.push_back(particle);
  }
}

// OFFSET: 0x00B48880
// FUNCTION: fn_18930
void CMagicFloorEffect::AddClouds(int layer) {
  for (int ring = 0; ring < 4; ++ring) {
    const int angle = std::max(rand() % 360, 0);
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\cloud11.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->alpha = 200;
      particle->unknown_0050 = 30;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
      particle->unknown_004C = std::max(rand() % 21, 0) + 80;
      particle->unknown_0054 = 1.0f;
      particle->SetColor(255, 100, 100);

      if (layer == 0) {
        particle->unknown_00C0 = 48.0f - static_cast<float>(ring) * 5.0f;
        particle->scale = 11.0f;
      } else if (layer == 1) {
        particle->unknown_00C0 = 28.0f - static_cast<float>(ring) * 5.0f;
        particle->scale = 22.0f;
      }

      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B48D50
// FUNCTION: fn_18931
void CMagicFloorEffect::Close() {
  for (CParticle* particle : this->floors) {
    FreeParticle(particle);
  }

  this->floors.clear();

  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B48DC0
// FUNCTION: fn_18932
void CMagicFloorEffect::Slot01() {
  this->AddFloors();
  this->AddClouds(0);
  this->AddClouds(1);
}

// OFFSET: 0x00B48E70
// FUNCTION: fn_18933
void CMagicFloorEffect::Slot07() {
  for (std::vector<CParticle*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it) {
    CParticle* particle = *it;
    particle->flags = 5;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    particle->scale = *reinterpret_cast<const float*>(&this->unknown_0048);
    *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(*reinterpret_cast<const int*>(this->unknown_004C));
    particle->unknown_004C = static_cast<int>(static_cast<float>(particle->unknown_0010) * *reinterpret_cast<const float*>(&this->unknown_0050));
    particle->alpha = *reinterpret_cast<const int*>(this->unknown_0054);
    particle->sourceBlend = 5;
    particle->destinationBlend = 7;
    InitParticleQuad(particle->vertices);
    particle->unknown_00E8 = 0;
  }
}

// OFFSET: 0x00B48F20
// FUNCTION: fn_18934
bool CMagicFloorEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B48F30
// FUNCTION: fn_18935
void CMagicFloorEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      const float current = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      int angle = 0;
      switch (particle->unknown_000C) {
      case 0:
      case 1:
        angle = static_cast<int>(current + 1.0f);
        break;
      case 2:
      case 3:
        angle = static_cast<int>(current + 2.0f);
        break;
      case 4:
      case 5:
        angle = static_cast<int>(current - 1.0f);
        break;
      }

      if (angle >= 360) {
        angle -= 360;
      } else if (angle < 0) {
        angle += 360;
      }

      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
      rotation.AppendXRotation(90.0f / 180.0f * 3.141592f);
      matrix scaling;
      std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
      scaling.SetScale(particle->scale, particle->scale, 1.0f);
      matrix result;
      result.Compose(scaling, rotation);
      particle->transform = result;
    }

    ++particle->unknown_00E8;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      const int direction = static_cast<int>(*reinterpret_cast<const float*>(&particle->unknown_00C4) + 5.0f) % 360;
      const int sweep = std::min(particle->unknown_0050 + 3, 180);
      particle->unknown_0050 = sweep;
      int alpha = particle->alpha;
      const int phase = particle->unknown_00E8 % 300;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(direction);
      if (phase == 0) {
        alpha = 50;
      } else if (phase > 100) {
        alpha = std::max(alpha - 1, 0);
      }
      if (phase < 12) {
        ++alpha;
      }
      particle->alpha = alpha;

      const int segment = particle->unknown_000C;
      const float radius = particle->unknown_0054;
      const int angle =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(sweep) / 20.0f) * segment) +
                         *reinterpret_cast<const float*>(&particle->unknown_00C4)) %
        360;
      const int tilt = particle->unknown_004C;
      const float spread = radius * g_cosTable[tilt];
      const float height = radius * g_sinTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float floorRadius = *reinterpret_cast<const float*>(&this->unknown_0048);

      EffectPoint points[4];
      points[0].x = floorRadius * cosine;
      points[0].y = -particle->unknown_00C0;
      points[0].z = floorRadius * sine;
      points[1].x = points[0].x + cosine * spread;
      points[1].y = points[0].y - height;
      points[1].z = points[0].z + sine * spread;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;

      if (segment != 0) {
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
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B49760
// FUNCTION: fn_18936
void CMagicFloorEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->floors.begin(); it != this->floors.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(particle->unknown_000C - 1) / 21.0f;
      quad.right = static_cast<float>(particle->unknown_000C) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
