#include "Contents/Effects/SaintCastingEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
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

      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->SetTexture(texture);
      particle->alpha = alpha;
      particle->unknown_0050 = 360;
      particle->scale = 4.1f;
      particle->unknown_004C = 80;
      particle->unknown_0054 = 0.0f;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>((particle->unknown_0010 + 2) * 90 % 360);

      switch (this->id) {
      case 342:
        particle->alpha += (3 - particle->unknown_0010) * 45;
        particle->unknown_00C0 = 20.0f - static_cast<float>(particle->unknown_0010);
        break;
      case 367:
        particle->alpha = 0;
        particle->unknown_00E8 = -5 * particle->unknown_0010;
        particle->unknown_00C0 = 15.0f - static_cast<float>(particle->unknown_0010);
        break;
      case 12:
      case 58:
      case 441:
      case 454:
      case 564:
      case 826:
      case 911:
        particle->alpha = 0;
        particle->unknown_00E8 = -5 * particle->unknown_0010;
        particle->unknown_00C0 = 20.0f - static_cast<float>(particle->unknown_0010);
        break;
      }

      switch (this->id) {
      case 12:
        particle->SetColor(255, 255, 170);
        break;
      case 342:
      case 564:
        particle->SetColor(255, 89, 182);
        break;
      case 441:
        particle->SetColor(100, 100, 255);
        break;
      case 454:
      case 826:
        particle->SetColor(50, 50, 50);
        break;
      case 911:
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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B752B0
// FUNCTION: fn_19163
void CSaintCastingEffect::Slot01() {
  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;

  switch (this->id) {
  case 12:
  case 58:
  case 342:
  case 441:
  case 454:
  case 564:
  case 911:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);
    break;
  }

  switch (this->id) {
  case 12:
    this->AddRing("effect\\ring_yellow.tga", 0);
    this->AddRing("effect\\ring_yellow.tga", 0);
    break;
  case 367:
    this->AddRing("effect\\ring_white.tga", 0);
    this->AddRing("effect\\ring_yellow.tga", 0);
    break;
  case 441:
    this->AddRing("effect\\ring_blue.tga", 0);
    this->AddRing("effect\\ring_blue.tga", 0);
    break;
  case 342:
  case 564:
    this->AddRing("effect\\ring_red.tga", 45);
    this->AddRing("effect\\ring_red.tga", 25);
    break;
  case 454:
  case 826:
    this->AddRing("effect\\ring_black.tga", 0);
    this->AddRing("effect\\ring_black.tga", 0);
    break;
  case 58:
  case 911:
    this->AddRing("effect\\ring_white.tga", 0);
    this->AddRing("effect\\ring_white.tga", 0);
    break;
  }

  switch (this->id) {
  case 12:
  case 58:
    this->unknown_002C = 56;
    break;
  case 342:
    this->unknown_002C = 1000;
    break;
  case 367:
  case 454:
  case 564:
  case 826:
  case 911:
    this->unknown_002C = 60;
    break;
  }
}

// OFFSET: 0x00B756A0
// FUNCTION: fn_19164
bool CSaintCastingEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B756B0
// FUNCTION: fn_19165
void CSaintCastingEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle && particle->unknown_00E8 > 0) {
      const int previousTilt = particle->unknown_004C - 1;
      float scale = particle->scale + 0.07f;
      int tilt = previousTilt < 10 ? 10 : previousTilt;
      int alpha = previousTilt >= 10 ? particle->alpha : 0;

      if (scale < 4.0f) {
        alpha += this->id == 367 ? 5 : 10;
      } else {
        alpha -= 3;
        if (alpha <= 0) {
          alpha = 0;
          if (particle->unknown_00E8 < this->unknown_002C - 30) {
            tilt = this->id == 367 ? 64 : 74;
            scale = 3.37f;
          }
        }
      }

      particle->scale = scale;
      particle->unknown_004C = tilt;
      particle->alpha = alpha;

      const int ring = particle->unknown_0010;
      const int wave = ring < 2 ? ring * 90 + particle->unknown_00E8 : (ring * 45 + particle->unknown_00E8) * 2;
      const int segment = particle->unknown_000C;
      const float radius =
        particle->unknown_00C0 * g_sinTable[segment * 9] * 0.3f * g_sinTable[wave % 360] + particle->unknown_00C0;
      const float height = radius * g_sinTable[tilt];
      const float spread = radius * g_cosTable[tilt];
      const int angle =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * segment) +
                         *reinterpret_cast<const float*>(&particle->unknown_00C4)) %
        360;
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];

      const vector3d inner = {cosine * particle->scale, 0.0f, sine * particle->scale};
      const vector3d outer = {inner.x + spread * cosine, 0.0f - height, inner.z + spread * sine};

      particle->vertices[3].x = inner.x;
      particle->vertices[3].y = inner.y;
      particle->vertices[3].z = inner.z;
      particle->vertices[1].x = outer.x;
      particle->vertices[1].y = outer.y;
      particle->vertices[1].z = outer.z;

      if (segment != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner = inner;
      this->lastOuter = outer;
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B75BB0
// FUNCTION: fn_19166
void CSaintCastingEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->unknown_000C <= 0) {
      continue;
    }

    if (this->id == 454 || this->id == 826) {
      particle->destinationBlend = 6;
    }

    const int segment = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
    particle->Render(quad);
  }
}
