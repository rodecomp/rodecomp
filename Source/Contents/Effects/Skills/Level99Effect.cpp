#include "Contents/Effects/Skills/Level99Effect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B6A0A0
// FUNCTION: fn_19079
CLevel99Effect::CLevel99Effect() : CEffect(200) {
}

// OFFSET: 0x00B6A250
// FUNCTION: fn_19081
void CLevel99Effect::SetColor(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
  for (std::vector<CParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B6A2D0
// FUNCTION: fn_19082
void CLevel99Effect::SetParticleTexture(std::string name) {
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
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\ring_blue.tga");
      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = row;
      particle->groupIndex = 3;
      particle->alpha = 120;
      particle->SetColor(100, 100, 255);
      particle->angle = static_cast<float>(startAngle);
      particle->angleParameter = 315;
      particle->speedParameter = 15.0f - static_cast<float>(row) * 2.0f;
      particle->scale = static_cast<float>(row) * 0.2f + 3.9f;
      particle->phaseParameter = tilt;
      particle->animationParameter = 0.0f;
      particle->InitParticleQuad(particle->vertices);
      this->sprites.push_back(particle);

      if (particle) {
        const int index = particle->columnIndex;
        const float radius = g_sinTable[index * 9] * particle->speedParameter;
        const float baseAngle = particle->angle;
        const int angle = static_cast<int>(static_cast<int>(particle->angleParameter / 20.0f) * index + baseAngle) % 360;
        const int ringTilt = particle->phaseParameter;
        const float spread = radius * g_cosTable[ringTilt];
        const float cosine = g_cosTable[angle];
        const float sine = g_sinTable[angle];
        const float height = radius * g_sinTable[ringTilt];

        EffectPoint points[2];
        const float innerX = particle->scale * cosine;
        const float innerY = 0.0f;
        const float innerZ = particle->scale * sine;
        const float outerX = innerX + spread * cosine;
        const float outerY = 0.0f - height;
        const float outerZ = innerZ + spread * sine;

        particle->vertices[3].x = innerX;
        particle->vertices[3].y = innerY;
        particle->vertices[3].z = innerZ;
        particle->vertices[1].x = outerX;
        particle->vertices[1].y = outerY;
        particle->vertices[1].z = outerZ;

        if (index != 0) {
          particle->vertices[0].x = this->previousRingPoints[1].x;
          particle->vertices[0].y = this->previousRingPoints[1].y;
          particle->vertices[0].z = this->previousRingPoints[1].z;
          particle->vertices[2].x = this->previousRingPoints[0].x;
          particle->vertices[2].y = this->previousRingPoints[0].y;
          particle->vertices[2].z = this->previousRingPoints[0].z;
        }

        this->previousRingPoints[0].x = innerX;
        this->previousRingPoints[0].y = innerY;
        this->previousRingPoints[0].z = innerZ;
        this->previousRingPoints[1].x = outerX;
        this->previousRingPoints[1].y = outerY;
        this->previousRingPoints[1].z = outerZ;
      }
    }

    ++row;
    tilt -= 5;
    startAngle += 90;
  } while (tilt > 40);
}

// OFFSET: 0x00B6AA60
// FUNCTION: fn_19084
void CLevel99Effect::Close() {
  for (std::vector<CParticle*>::iterator it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    FreeParticle(*it);
  }
  this->sparkles.clear();

  for (std::vector<CParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    FreeParticle(*it);
  }
  this->sprites.clear();
}

// OFFSET: 0x00B6AAD0
// FUNCTION: fn_19085
void CLevel99Effect::Open() {
  int index = 0;
  int tilt = 0;
  int angle = 0;
  do {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
    if (particle) {
      particle->SetTexture("effect\\pikapika2.bmp");
      particle->effectId = this->id;
      particle->columnIndex = index;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->alpha = 200;
      particle->angle = static_cast<float>(angle);
      particle->phaseParameter = tilt;
      particle->scale = 9.0f;
      particle->InitParticleQuad(particle->vertices);
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
bool CLevel99Effect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6AC90
// FUNCTION: fn_19087
void CLevel99Effect::OnProcess() {
  for (auto it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle) {
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(particle->angle / 180.0f * 3.141592f);
      rotation.AppendXRotation(1.570796f);

      int phase = particle->phaseParameter + 3;
      if (phase >= 360) {
        phase -= 360;
      }
      particle->phaseParameter = phase;

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      const float size = (g_sinTable[phase] + 1.0f) * (particle->scale * 0.1f) + particle->scale;
      scale.SetScale(size, size, 1.0f);

      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;
    }
    ++particle->elapsedFrames;
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle) {
      float& baseAngle = particle->angle;
      const float degrees = static_cast<float>(static_cast<int>(static_cast<float>(particle->rowIndex) + baseAngle + 3.0f));
      baseAngle = degrees;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(degrees / 180.0f * 3.141592f);
      particle->transform = rotation;
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B6B0E0
// FUNCTION: fn_19088
void CLevel99Effect::Render() {
  for (std::vector<CParticle*>::iterator it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    CParticle* sparkle = *it;
    if (sparkle) {
      sparkle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    CParticle* sprite = *it;
    if (sprite && sprite->columnIndex > 0) {
      const int frame = sprite->columnIndex;
      ParticleQuad quad;
      quad.billboard = false;
      quad.unused = false;
      quad.depth = true;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(frame - 1) / 21.0f;
      quad.right = static_cast<float>(frame) / 21.0f;
      sprite->Render(quad);
    }
  }
}
