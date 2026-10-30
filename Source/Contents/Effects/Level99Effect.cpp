#include "Contents/Effects/Level99Effect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B6A0A0
// FUNCTION: fn_19079
CLevel99Effect::CLevel99Effect() : CEffect(200) {
}

// OFFSET: 0x00B6A250
// FUNCTION: fn_19081
void CLevel99Effect::Slot08(int red, int green, int blue) {
  for (CParticle* sparkle : this->sparkles) {
    sparkle->SetColor(red, green, blue);
  }
  for (CParticle* sprite : this->sprites) {
    sprite->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B6A2D0
// FUNCTION: fn_19082
void CLevel99Effect::Slot11(std::string name) {
  for (auto it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    (*it)->SetTexture(name);
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->SetTexture(name);
  }
}

// OFFSET: 0x00B6A520
// FUNCTION: fn_19083
void CLevel99Effect::AddRings() {
  int row = 0;
  int startAngle = 0;
  int tilt = 55;
  do {
    for (int segment = 0; segment < 22; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\ring_blue.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 3;
      particle->alpha = 120;
      particle->SetColor(100, 100, 255);
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(startAngle);
      particle->unknown_0050 = 315;
      particle->unknown_00C0 = 15.0f - static_cast<float>(row) * 2.0f;
      particle->scale = static_cast<float>(row) * 0.2f + 3.9f;
      particle->unknown_004C = tilt;
      particle->unknown_0054 = 0.0f;
      InitParticleQuad(particle->vertices);
      this->sprites.push_back(particle);

      const int index = particle->unknown_000C;
      const float radius = g_sinTable[index * 9] * particle->unknown_00C0;
      const float baseAngle = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      const int angle = static_cast<int>(static_cast<int>(particle->unknown_0050 / 20.0f) * index + baseAngle) % 360;
      const int ringTilt = particle->unknown_004C;
      const float spread = radius * g_cosTable[ringTilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float height = radius * g_sinTable[ringTilt];

      vector3d points[2] = {};
      points[0].x = particle->scale * cosine;
      points[0].y = 0.0f;
      points[0].z = particle->scale * sine;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = 0.0f - height;
      points[1].z = points[0].z + spread * sine;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;

      if (index != 0) {
        particle->vertices[0].x = this->unknown_00CC[1].x;
        particle->vertices[0].y = this->unknown_00CC[1].y;
        particle->vertices[0].z = this->unknown_00CC[1].z;
        particle->vertices[2].x = this->unknown_00CC[0].x;
        particle->vertices[2].y = this->unknown_00CC[0].y;
        particle->vertices[2].z = this->unknown_00CC[0].z;
      }

      this->unknown_00CC[0] = points[0];
      this->unknown_00CC[1] = points[1];
    }

    ++row;
    tilt -= 5;
    startAngle += 90;
  } while (tilt > 40);
}

// OFFSET: 0x00B6AA60
// FUNCTION: fn_19084
void CLevel99Effect::Close() {
  for (CParticle* sparkle : this->sparkles) {
    FreeParticle(sparkle);
  }
  this->sparkles.clear();

  for (CParticle* sprite : this->sprites) {
    FreeParticle(sprite);
  }
  this->sprites.clear();
}

// OFFSET: 0x00B6AAD0
// FUNCTION: fn_19085
void CLevel99Effect::Slot01() {
  int index = 0;
  int tilt = 0;
  int angle = 0;
  do {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
    if (particle) {
      particle->SetTexture("effect\\pikapika2.bmp");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = index;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->alpha = 200;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
      particle->unknown_004C = tilt;
      particle->scale = 9.0f;
      InitParticleQuad(particle->vertices);
      this->sparkles.push_back(particle);
    }

    tilt += 180;
    angle += 22;
    ++index;
  } while (angle < 44);

  this->AddRings();
}

// OFFSET: 0x00B6ABB0
// FUNCTION: fn_19086
bool CLevel99Effect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B6AC90
// FUNCTION: fn_19087
void CLevel99Effect::Slot03() {
  for (auto it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];

    if (CParticle* particle = *it) {
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(*reinterpret_cast<const float*>(&particle->unknown_00C4) / 180.0f * 3.141592f);
      rotation.AppendXRotation(1.570796f);

      int phase = particle->unknown_004C + 3;
      if (phase >= 360) {
        phase -= 360;
      }
      particle->unknown_004C = phase;

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      const float size = (g_sinTable[phase] + 1.0f) * (particle->scale * 0.1f) + particle->scale;
      scale.SetScale(size, size, 1.0f);

      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;
    }
    ++(*it)->unknown_00E8;
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];

    if (CParticle* particle = *it) {
      float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      const float degrees = static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0010) + baseAngle + 3.0f));
      baseAngle = degrees;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(degrees / 180.0f * 3.141592f);
      particle->transform = rotation;
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B6B0E0
// FUNCTION: fn_19088
void CLevel99Effect::Slot04() {
  for (CParticle* sparkle : this->sparkles) {
    if (sparkle) {
      sparkle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (CParticle* sprite : this->sprites) {
    if (sprite && sprite->unknown_000C > 0) {
      const int frame = sprite->unknown_000C;
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(frame - 1) / 21.0f;
      quad.right = static_cast<float>(frame) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      sprite->Render(quad);
    }
  }
}
