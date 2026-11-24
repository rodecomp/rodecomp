#include "Contents/Effects/Skills/Level99Effect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B6A0A0
// FUNCTION: fn_19079
CLevel99Effect::CLevel99Effect() : CEffect(200) {
}

// OFFSET: 0x00B6A150
// FUNCTION: fn_46121
__declspec(noinline) CLevel99Effect::~CLevel99Effect() {
}

// OFFSET: 0x00B6A250
// FUNCTION: fn_19081
void CLevel99Effect::SetColor(short red, short green, short blue) {
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
      CParticle* particle = CreateParticle(&this->positionVector, 13);
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

      this->UpdateRingVertices(particle);
    }

    ++row;
    startAngle += 90;
    tilt -= 5;
  } while (tilt > 40);
}

// OFFSET: 0x00B6A7E0
// FUNCTION: fn_46122
__forceinline void CLevel99Effect::AddSparkles() {
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 13);
    if (particle) {
      particle->SetTexture("effect\\pikapika2.bmp");
      particle->effectId = this->id;
      particle->columnIndex = index;
      particle->rowIndex = 0;
      particle->groupIndex = 1;
      particle->alpha = 200;
      particle->angle = static_cast<float>(index * 22);
      particle->phaseParameter = index * 180;
      particle->scale = 9.0f;
      particle->InitParticleQuad(particle->vertices);
      this->sparkles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B6A8C0
// FUNCTION: fn_46123
__forceinline void CLevel99Effect::UpdateRingVertices(CParticle* particle) {
  if (particle) {
    const int index = particle->columnIndex;
    const float radius = g_sinTable[index * 9] * particle->speedParameter;
    const float baseAngle = particle->angle;
    const int angle = static_cast<int>(static_cast<int>(particle->angleParameter / g_fireflyGroundOffset) * index + baseAngle) % 360;
    const int ringTilt = particle->phaseParameter;
    const float spread = radius * g_cosTable[ringTilt];
    const float cosine = g_cosTable[angle];
    const float sine = g_sinTable[angle];
    const float height = radius * g_sinTable[ringTilt];

    EffectPoint points[2];
    points[0].y = 0.0f;
    points[1].y = 0.0f - height;
    points[0].x = particle->scale * cosine;
    points[0].z = particle->scale * sine;
    points[1].x = points[0].x + spread * cosine;
    points[1].z = spread * sine + points[0].z;

    particle->vertices[3].y = points[0].y;
    particle->vertices[1].y = points[1].y;
    particle->vertices[3].x = points[0].x;
    particle->vertices[3].z = points[0].z;
    particle->vertices[1].x = points[1].x;
    particle->vertices[1].z = points[1].z;

    if (index != 0) {
      particle->vertices[0].x = this->previousRingPoints[1].x;
      particle->vertices[0].y = this->previousRingPoints[1].y;
      particle->vertices[0].z = this->previousRingPoints[1].z;
      particle->vertices[2].x = this->previousRingPoints[0].x;
      particle->vertices[2].y = this->previousRingPoints[0].y;
      particle->vertices[2].z = this->previousRingPoints[0].z;
    }

    this->previousRingPoints[0].x = points[0].x;
    this->previousRingPoints[0].y = points[0].y;
    this->previousRingPoints[0].z = points[0].z;
    this->previousRingPoints[1].x = points[1].x;
    this->previousRingPoints[1].y = points[1].y;
    this->previousRingPoints[1].z = points[1].z;
  }
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
  this->AddSparkles();
  this->AddRings();
}

// OFFSET: 0x00B6ABB0
// FUNCTION: fn_19086
bool CLevel99Effect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B6ABD0
// FUNCTION: fn_46124
__forceinline void CLevel99Effect::UpdateRing(CParticle* particle) {
  if (particle) {
    float& baseAngle = particle->angle;
    const float degrees = static_cast<float>(static_cast<int>(static_cast<float>(particle->rowIndex) + baseAngle + 3.0f));
    baseAngle = degrees;

    matrix rotation;
    std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
    rotation.MakeYRotation(degrees / 180.0f * 3.141592f);
    particle->transform = rotation;
  }
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

    this->UpdateSparkle(particle);
    ++particle->elapsedFrames;
  }

  for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    this->UpdateRing(particle);
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B6AF10
// FUNCTION: fn_46125
__forceinline void CLevel99Effect::UpdateSparkle(CParticle* particle) {
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
}

// OFFSET: 0x00B6B060
// FUNCTION: fn_46126
__forceinline void CLevel99Effect::RenderRing(CParticle* particle) {
  if (particle && particle->columnIndex > 0) {
    const int frame = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(frame - 1) / 21.0f;
    quad.right = static_cast<float>(frame) / 21.0f;
    particle->Render(quad);
  }
}

// OFFSET: 0x00B6B0E0
// FUNCTION: fn_19088
void CLevel99Effect::Render() {
  for (std::vector<CParticle*>::iterator it = this->sparkles.begin(); it != this->sparkles.end(); ++it) {
    this->RenderSparkle(*it);
  }

  for (std::vector<CParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    this->RenderRing(*it);
  }
}

// OFFSET: 0x00B6B1D0
// FUNCTION: fn_46127
__forceinline void CLevel99Effect::RenderSparkle(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}
