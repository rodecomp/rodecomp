#include "Contents/Effects/SaintCastingEffect.h"
#include "Contents/Effects/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/EffectPoint.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B74F50
// FUNCTION: fn_19161
void CSaintCastingEffect::AddRing(const char* texture, int alpha) {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->SetTexture(texture);
      particle->alpha = alpha;
      particle->angleParameter = 360;
      particle->scale = 4.1f;
      particle->phaseParameter = 80;
      particle->animationParameter = 0.0f;
      const int row = particle->rowIndex;
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>((row + 2) * 90 % 360);

      switch (this->id) {
      case EF_COUPLECASTING:
        particle->alpha += (3 - row) * 45;
        particle->speedParameter = 20.0f - static_cast<float>(row);
        break;
      case EF_AURABLADE:
        particle->alpha = 0;
        particle->elapsedFrames = -5 * row;
        particle->speedParameter = 15.0f - static_cast<float>(row);
        break;
      case EF_BEGINSPELL:
      case EF_BEGINSPELL6:
      case EF_BLUECASTING:
      case EF_DARKCASTING:
      case EF_HOMUNCASTING:
      case EF_DARKCASTING2:
      case EF_GREENCASTING:
        particle->alpha = 0;
        particle->elapsedFrames = -5 * row;
        particle->speedParameter = 20.0f - static_cast<float>(row);
        break;
      }

      switch (this->id) {
      case EF_BEGINSPELL:
        particle->SetColor(255, 255, 170);
        break;
      case EF_COUPLECASTING:
      case EF_HOMUNCASTING:
        particle->SetColor(255, 89, 182);
        break;
      case EF_BLUECASTING:
        particle->SetColor(100, 100, 255);
        break;
      case EF_DARKCASTING:
      case EF_DARKCASTING2:
        particle->SetColor(50, 50, 50);
        break;
      case EF_GREENCASTING:
        particle->SetColor(100, 255, 100);
        break;
      }

      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B75270
// FUNCTION: fn_19162
void CSaintCastingEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B752B0
// FUNCTION: fn_19163
void CSaintCastingEffect::Open() {
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;

  switch (this->id) {
  case EF_BEGINSPELL:
  case EF_BEGINSPELL6:
  case EF_COUPLECASTING:
  case EF_BLUECASTING:
  case EF_DARKCASTING:
  case EF_HOMUNCASTING:
  case EF_GREENCASTING:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);
    break;
  }

  switch (this->id) {
  case EF_BEGINSPELL:
    this->AddRing("effect\\ring_yellow.tga", 0);
    this->AddRing("effect\\ring_yellow.tga", 0);
    break;
  case EF_AURABLADE:
    this->AddRing("effect\\ring_white.tga", 0);
    this->AddRing("effect\\ring_yellow.tga", 0);
    break;
  case EF_BLUECASTING:
    this->AddRing("effect\\ring_blue.tga", 0);
    this->AddRing("effect\\ring_blue.tga", 0);
    break;
  case EF_COUPLECASTING:
  case EF_HOMUNCASTING:
    this->AddRing("effect\\ring_red.tga", 45);
    this->AddRing("effect\\ring_red.tga", 25);
    break;
  case EF_DARKCASTING:
  case EF_DARKCASTING2:
    this->AddRing("effect\\ring_black.tga", 0);
    this->AddRing("effect\\ring_black.tga", 0);
    break;
  case EF_BEGINSPELL6:
  case EF_GREENCASTING:
    this->AddRing("effect\\ring_white.tga", 0);
    this->AddRing("effect\\ring_white.tga", 0);
    break;
  }

  switch (this->id) {
  case EF_BEGINSPELL:
  case EF_BEGINSPELL6:
    this->effectLifetime = 56;
    break;
  case EF_COUPLECASTING:
    this->effectLifetime = 1000;
    break;
  case EF_AURABLADE:
  case EF_DARKCASTING:
  case EF_HOMUNCASTING:
  case EF_DARKCASTING2:
  case EF_GREENCASTING:
    this->effectLifetime = 60;
    break;
  }
}

// OFFSET: 0x00B756A0
// FUNCTION: fn_19164
bool CSaintCastingEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B756B0
// FUNCTION: fn_19165
void CSaintCastingEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];

    CParticle* particle = *it;
    if (particle && particle->elapsedFrames > 0) {
      const int previousTilt = particle->phaseParameter - 1;
      float scale = particle->scale + 0.07f;
      int tilt = previousTilt < 10 ? 10 : previousTilt;
      int alpha = previousTilt >= 10 ? particle->alpha : 0;

      if (scale >= 4.0f) {
        alpha -= 3;
        if (alpha <= 0) {
          alpha = 0;
          if (particle->elapsedFrames < this->effectLifetime - 30) {
            tilt = this->id == EF_AURABLADE ? 64 : 74;
            scale = 3.37f;
          }
        }
      } else if (this->id == EF_AURABLADE) {
        alpha += 5;
      } else {
        alpha += 10;
      }

      particle->scale = scale;
      particle->phaseParameter = tilt;
      particle->alpha = alpha;

      const int ring = particle->rowIndex;
      const int wave = ring < 2 ? ring * 90 + particle->elapsedFrames : (ring * 45 + particle->elapsedFrames) * 2;
      const int segment = particle->columnIndex;
      const float radius =
        particle->speedParameter * g_sinTable[segment * 9] * 0.3f * g_sinTable[wave % 360] + particle->speedParameter;
      const float height = radius * g_sinTable[tilt];
      const float spread = radius * g_cosTable[tilt];
      const int angle =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) +
                         *reinterpret_cast<const float*>(&particle->angleBits)) %
        360;
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];

      EffectPoint points[2];
      points[0].x = cosine * particle->scale;
      points[0].y = 0.0f;
      points[0].z = sine * particle->scale;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = points[0].y - height;
      points[1].z = points[0].z + spread * sine;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;

      if (segment != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner.x = points[0].x;
      this->lastInner.y = points[0].y;
      this->lastInner.z = points[0].z;
      this->lastOuter.x = points[1].x;
      this->lastOuter.y = points[1].y;
      this->lastOuter.z = points[1].z;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B75BB0
// FUNCTION: fn_19166
void CSaintCastingEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->columnIndex <= 0) {
      continue;
    }

    if (this->id == 454 || this->id == 826) {
      particle->destinationBlend = 6;
    }

    const int segment = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(segment - 1) / 21.0f;
    quad.right = static_cast<float>(segment) / 21.0f;
    particle->Render(quad);
  }
}
