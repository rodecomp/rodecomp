#include "Contents/Combat/CombatTypes.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/ThrowEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00B7CB10
// FUNCTION: fn_19227
CThrowItemEffect::CThrowItemEffect()
    : CEffect(-1), unknown_00CC(0), unknown_00D0(0), unknown_00D4(1), unknown_00D8(4), unknown_00DC(0), unknown_00E0(0), unknown_00EC(0),
      unknown_00F0(0), unknown_00F4(0) {
}

// OFFSET: 0x00B7CCC0
// FUNCTION: fn_19229
void CThrowItemEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B7CD30
// FUNCTION: fn_19230
void CThrowItemEffect::Open() {
  switch (this->id) {
  case EF_THROW_HAPPOKUNAI:
    this->unknown_00D4 = 8;
    break;
  case EF_THROW_MULTIPLE_COIN:
    this->unknown_00D4 = 10;
    break;
  case EF_KAIHOU:
    if (CGameActor* owner = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
      this->unknown_00D4 = owner->unknown_03D0;
      this->unknown_00D8 = 1;
    }
    break;
  }

  for (int row = 0; row < this->unknown_00D4; ++row) {
    for (int column = 0; column < this->unknown_00D8; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = column;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 1;

      switch (this->id) {
      case EF_THROWITEM:
        this->unknown_002C = 300;
        this->unknown_00E0 = 30.0f;
        this->InitTrajectory(particle);
        particle->SetTexture("유저인터페이스\\item\\빈병.bmp");
        particle->scale = 1.5f;
        particle->SetColor(135, 255, 135);
        particle->unknown_00C4 = 0;
        particle->unknown_00C0 = particle->unknown_0054 / 25.0f;
        particle->alpha = 255;
        particle->unknown_00D0 = this->position[0];
        particle->unknown_00D4 = this->position[1] - 8.0f;
        particle->unknown_00D8 = this->position[2];
        particle->destinationBlend = 6;
        break;
      case EF_THROWITEM2:
        particle->SetTexture("유저인터페이스\\item\\빈포션병.bmp");
        this->InitPotionParticle(particle, this->unknown_0030 == -1 ? 1 : this->unknown_0030);
        break;
      case EF_THROWITEM3:
        this->unknown_002C = 300;
        this->unknown_00E0 = 10.0f;
        particle->SetTexture("유저인터페이스\\item\\돌.bmp");
        particle->scale = 1.0f;
        particle->SetColor(255, 255, 255);
        particle->unknown_00C4 = 0;
        particle->unknown_00C0 = 2.0f;
        particle->alpha = 255;
        particle->unknown_00D0 = this->position[0];
        particle->unknown_00D4 = this->position[1] - 8.0f;
        particle->unknown_00D8 = this->position[2];
        particle->destinationBlend = 6;
        break;
      case EF_THROWITEM4:
        this->unknown_00D0 = 5;
        particle->SetTexture("유저인터페이스\\item\\화염병.bmp");
        this->InitFlameParticle(particle);
        break;
      case EF_THROWITEM5:
        particle->SetTexture("유저인터페이스\\item\\버서크포션.bmp");
        this->InitPotionParticle(particle, this->unknown_0030 == -1 ? 1 : this->unknown_0030);
        break;
      case EF_THROWITEM6:
        particle->SetTexture("유저인터페이스\\item\\베넘나이프.bmp");
        this->InitSpinParticle(particle);
        break;
      case EF_THROWITEM7:
        particle->SetTexture("유저인터페이스\\item\\수리검.bmp");
        this->InitSpinParticle(particle);
        break;
      case EF_THROWITEM8:
        particle->SetTexture("유저인터페이스\\item\\쿠나이_독.bmp");
        this->InitSpinParticle(particle);
        break;
      case EF_THROWITEM9:
        particle->SetTexture("유저인터페이스\\item\\풍마_열화.bmp");
        this->InitSpinParticle(particle);
        break;
      case EF_THROWITEM10:
        particle->SetTexture("effect\\coin_a.bmp");
        this->InitSpinParticle(particle);
        break;
      case EF_THROWITEM11:
        particle->SetTexture("유저인터페이스\\item\\폭탄버섯포자.bmp");
        this->InitPotionParticle(particle, this->unknown_0030 == -1 ? 1 : this->unknown_0030);
        break;
      case EF_THROWITEM4_1:
        this->unknown_00D0 = 10;
        particle->SetTexture("유저인터페이스\\item\\염산병.bmp");
        this->InitFlameParticle(particle);
        break;
      case EF_THROW_BAKURETSU:
        particle->SetTexture("유저인터페이스\\item\\쿠나이_화.bmp");
        this->InitSpinParticle(particle);
        particle->unknown_00C0 = 1.0f;
      case EF_THROW_HAPPOKUNAI:
        this->unknown_002C = 200;
        this->unknown_00E0 = 1.0f;
        particle->SetTexture("유저인터페이스\\item\\쿠나이_독.bmp");
        particle->scale = 3.0f;
        particle->SetColor(255, 200, 255);
        *reinterpret_cast<float*>(&particle->unknown_00C4) = 330.0f;
        particle->unknown_00C0 = 1.5f;
        particle->alpha = 255;
        particle->unknown_00D0 = this->position[0];
        particle->unknown_00D8 = this->position[2];
        particle->unknown_00D4 = this->position[1] - 8.0f;
        particle->destinationBlend = 6;
        if (this->sourceActor == 0) {
          this->sourcePosition[1] = this->position[1];
          this->sourcePosition[2] = this->position[2];
          this->sourcePosition[0] = this->position[0] - 8.0f;
        }
        break;
      case EF_THROW_MULTIPLE_COIN:
        this->unknown_002C = 200;
        this->unknown_00E0 = 10.0f;
        particle->SetTexture("effect\\coin_a.bmp");
        particle->scale = 2.5f;
        particle->SetColor(255, 200, 0);
        *reinterpret_cast<float*>(&particle->unknown_00C4) = 330.0f;
        particle->unknown_00C0 = 2.0f;
        particle->alpha = 255;
        particle->destinationBlend = 6;
        particle->unknown_00D8 = this->position[2];
        particle->unknown_00D4 = this->position[1] - 8.0f;
        particle->unknown_00D0 = this->position[0];
        break;
      case EF_KAIHOU:
        if (CGameActor* owner = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
          switch (owner->charmType) {
          case CHARM_TYPE_WATER:
            particle->SetTexture("effect\\WaterOrb.bmp");
            particle->scale = 1.0f;
            break;
          case CHARM_TYPE_LAND:
            particle->SetTexture("effect\\StoneOrb.bmp");
            break;
          case CHARM_TYPE_FIRE:
            particle->SetTexture("effect\\FireOrb.bmp");
            break;
          case CHARM_TYPE_WIND:
            particle->SetTexture("effect\\LightningOrb.bmp");
            break;
          }

          this->unknown_002C = 200;
          particle->scale = 2.0f;
          *reinterpret_cast<float*>(&particle->unknown_00C4) = 330.0f;
          particle->unknown_00C0 = 1.5f;
          particle->alpha = 255;
          particle->unknown_00D8 = this->position[2];
          particle->unknown_00D4 = this->position[1] - 15.0f;
          particle->unknown_00D0 = this->position[0];
        }
        break;
      }

      this->InitTrajectory(particle);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B7D3B0
// FUNCTION: fn_19231
void CThrowItemEffect::InitFlameParticle(CParticle* particle) {
  this->unknown_002C = 300;
  this->unknown_00E0 = 1.0f;
  this->InitTrajectory(particle);
  particle->scale = 1.5f;
  particle->SetColor(255, 255, 255);
  particle->unknown_00C4 = 0;
  particle->alpha = 255;
  particle->unknown_00C0 = particle->unknown_0054 / 25.0f;
  particle->unknown_00D0 = this->position[0];
  particle->unknown_00D8 = this->position[2];
  particle->destinationBlend = 6;
  particle->unknown_00D4 = this->position[1] - 8.0f;
}

// OFFSET: 0x00B7D4D0
// FUNCTION: fn_19232
void CThrowItemEffect::InitPotionParticle(CParticle* particle, int kind) {
  this->unknown_002C = 300;
  this->unknown_00E0 = 30.0f;
  this->InitTrajectory(particle);
  particle->scale = 1.5f;
  particle->unknown_00C4 = 0;
  particle->alpha = 255;
  particle->unknown_00C0 = particle->unknown_0054 / 25.0f;
  particle->unknown_00D0 = this->position[0];
  particle->unknown_00D8 = this->position[2];
  particle->unknown_00D4 = this->position[1] - 8.0f;

  if (kind != 6) {
    particle->destinationBlend = 6;
  }

  if (kind == 1) {
    particle->SetColor(255, 135, 135);
  } else if (kind == 2) {
    particle->SetColor(255, 205, 135);
  } else if (kind == 3) {
    particle->SetColor(255, 255, 135);
  } else if (kind == 4) {
    particle->SetColor(255, 255, 255);
  } else if (kind == 5) {
    particle->SetColor(135, 135, 255);
  } else if (kind == 6) {
    particle->SetColor(255, 255, 255);
  }
}

// OFFSET: 0x00B7D810
// FUNCTION: fn_19233
void CThrowItemEffect::InitSpinParticle(CParticle* particle) {
  this->unknown_002C = 200;
  this->unknown_00E0 = 1.0f;
  particle->scale = 2.0f;
  particle->SetColor(255, 200, 255);
  *reinterpret_cast<float*>(&particle->unknown_00C4) = 330.0f;
  particle->unknown_00C0 = 3.0f;
  particle->alpha = 255;
  particle->unknown_00D0 = this->position[0];
  particle->unknown_00D4 = this->position[1] - 8.0f;
  particle->unknown_00D8 = this->position[2];
  particle->destinationBlend = 6;
}

// OFFSET: 0x00B7D8B0
// FUNCTION: fn_19234
void CThrowItemEffect::InitEvents() {
  switch (this->id) {
  case EF_THROWITEM7:
  case EF_THROWITEM8:
  case EF_THROWITEM9:
  case EF_THROWITEM10:
    this->AddEvent(0, "effect\\닌자_던지기.wav", 0, 0);
    break;
  case EF_THROWITEM:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;
  case EF_THROWITEM11:
    this->AddEvent(0, "effect\\G스포어익스플로젼.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B7DE60
// FUNCTION: fn_19235
bool CThrowItemEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B7DE70
// FUNCTION: fn_19236
void CThrowItemEffect::OnProcess() {
  for (auto particle = this->particles.begin(); particle != this->particles.end(); ++particle) {
    CParticle* target = *particle;

    target->position.x = this->position[0];
    target->position.y = this->position[1];
    target->position.z = this->position[2];

    if (target != nullptr && this->unknown_00D0 <= this->frame) {
      float& angle = target->unknown_00C0;
      float& rotation = *reinterpret_cast<float*>(&target->unknown_00C4);
      float& offsetX = *reinterpret_cast<float*>(&target->unknown_00D0);
      float& offsetY = *reinterpret_cast<float*>(&target->unknown_00D4);
      float& offsetZ = *reinterpret_cast<float*>(&target->unknown_00D8);

      if (target->unknown_000C == 0) {
        rotation = static_cast<float>(static_cast<int>(rotation + 45.0f) % 360);
      } else {
        this->unknown_00E4 -= target->unknown_000C == 1 ? 150 : 25;
        target->alpha = this->unknown_00E4;
        offsetX = this->unknown_00EC;
        offsetY = this->unknown_00F0;
        offsetZ = this->unknown_00F4;
        rotation = this->unknown_00E8;
      }

      const float progress = static_cast<float>(this->frame) / static_cast<float>(this->unknown_00CC);
      float height = 0.0f;

      if (progress > 1.05f) {
        const int alpha = target->alpha - 20;
        target->alpha = alpha > 0 ? alpha : 0;
      } else {
        const int index = *reinterpret_cast<const int*>(&target->unknown_0048[4]);
        const float radius = angle / *reinterpret_cast<const float*>(&target->unknown_0048[0xC]);
        offsetX = angle * g_cosTable[index] + offsetX;
        offsetY = radius * this->unknown_00DC + offsetY;
        offsetZ = angle * g_sinTable[index] + offsetZ;
        height = g_sinTable[-static_cast<int>(progress * -180.0f)] * this->unknown_00E0;
      }

      this->unknown_00EC = offsetX;
      this->unknown_00F0 = offsetY;
      this->unknown_00F4 = offsetZ;
      this->unknown_00E8 = rotation;
      this->unknown_00E4 = target->alpha;

      if (target->alpha > 0) {
        matrix view;
        std::memcpy(view.values, g_identityMatrix, sizeof(view.values));
        view.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
        view.AppendYRotation(rotation / 180.0f * 3.141592f);

        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(target->scale, target->scale, 1.0f);

        matrix translate;
        std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
        translate.MakeTranslate(offsetX, offsetY - height, offsetZ);

        matrix rotationMatrix;
        rotationMatrix.Compose(scale, view);

        matrix transform;
        transform.Compose(rotationMatrix, translate);
        target->transform = transform;

        if (this->id == 0x3D5 && target->unknown_0010 == 0 && target->unknown_000C == 0) {
          CRenderObject* object = FindRenderObject(this->actor);

          if (object != nullptr) {
            float& objectAngle = *reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(object) + 0x4C);
            objectAngle = static_cast<float>(static_cast<int>(objectAngle + 45.0f) % 360);
          }
        }
      }
    }

    ++target->unknown_00E8;
  }
}

// OFFSET: 0x00B7E660
// FUNCTION: fn_19237
void CThrowItemEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00B7E700
// FUNCTION: fn_19238
void CThrowItemEffect::InitTrajectory(CParticle* particle) {
  const float deltaX = this->sourcePosition[0] - this->position[0];
  const float deltaZ = this->sourcePosition[2] - this->position[2];
  this->unknown_00DC = this->sourcePosition[1] - this->position[1];

  const unsigned short heading = static_cast<unsigned short>(static_cast<int>(DirectionAngle(deltaX, deltaZ)));
  unsigned short offset = 0;
  if (this->id == 981) {
    offset = static_cast<unsigned short>(static_cast<short>(360 / this->unknown_00D4) * static_cast<short>(particle->unknown_0010));
  }

  particle->unknown_004C = static_cast<short>(heading + offset) % 360;

  const float distance = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  particle->unknown_0054 = distance;

  for (int step = 0; step < 100; ++step) {
    if (static_cast<float>(step) * particle->unknown_00C0 > distance) {
      this->unknown_00CC = step;
      return;
    }
  }
}

// OFFSET: 0x00B7EA40
// FUNCTION: fn_19240
void CVolcanoEffect::AddRings() {
  int row = 0;
  for (int baseAngle = 0; baseAngle < 360; baseAngle += 90) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 1;

      switch (this->id) {
      case EF_GANBANTEIN:
        particle->unknown_00C0 = 15.0f;
        particle->unknown_004C = 70;
        particle->unknown_0054 = 0.0f;
        particle->unknown_0050 = 360;
        particle->alpha = 1;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(row * 90);
        particle->scale = static_cast<float>(row) + 1.0f;
        break;
      case EF_VOLCANO:
      case EF_DELUGE:
      case EF_VIOLENTGALE:
      case EF_LANDPROTECTOR:
      case EF_GUMGANG2:
      case EF_GUMGANG3:
      case EF_GUMGANG4:
      case EF_GUMGANG6:
      case EF_GUMGANG7:
      case EF_GUMGANG8:
      case EF_GUMGANG9:
        particle->unknown_00C0 = 25.0f;
        particle->unknown_004C = 80;
        particle->unknown_0054 = 0.0f;
        particle->unknown_0050 = 360;
        particle->alpha = 1;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(baseAngle);
        particle->scale = static_cast<float>(row) + 1.0f;
        break;
      case EF_GUMGANG5:
        particle->alpha = 1;
        particle->unknown_0050 = 360;
        particle->unknown_00C0 = 25.0f;
        particle->unknown_004C = 80;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(row * 90);
        particle->unknown_0054 = 0.0f;
        particle->scale = static_cast<float>(row) + 1.0f;
        particle->SetColor(0, 0, 255);
        break;
      }

      switch (this->id) {
      case EF_GANBANTEIN:
      case EF_LANDPROTECTOR:
      case EF_GUMGANG6:
        particle->SetTexture("effect\\ring_white.tga");
        break;
      case EF_VOLCANO:
      case EF_GUMGANG7:
        particle->SetTexture("effect\\ring_red.tga");
        break;
      case EF_DELUGE:
      case EF_GUMGANG5:
        particle->SetTexture("effect\\ring_blue.tga");
        break;
      case EF_VIOLENTGALE:
        particle->SetTexture("effect\\magic_green.tga");
        break;
      case EF_GUMGANG2:
      case EF_GUMGANG3:
      case EF_GUMGANG8:
        particle->SetTexture("effect\\ring_yellow.tga");
        break;
      case EF_GUMGANG4:
        particle->SetTexture("effect\\ring_purple.tga");
        break;
      case EF_GUMGANG9:
        particle->SetTexture("effect\\ring_brown.tga");
        break;
      }

      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }

    ++row;
  }
}

// OFFSET: 0x00B7EE80
// FUNCTION: fn_19241
void CVolcanoEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B7EEC0
// FUNCTION: fn_19242
void CVolcanoEffect::Open() {
  switch (this->id) {
  case EF_GANBANTEIN:
  case EF_VOLCANO:
  case EF_DELUGE:
  case EF_VIOLENTGALE:
  case EF_LANDPROTECTOR:
  case EF_GUMGANG2:
  case EF_GUMGANG3:
  case EF_GUMGANG4:
  case EF_GUMGANG5:
  case EF_GUMGANG6:
  case EF_GUMGANG7:
  case EF_GUMGANG8:
  case EF_GUMGANG9:
    this->AddRings();
    break;
  }
}

// OFFSET: 0x00B7F190
// FUNCTION: fn_19243
bool CVolcanoEffect::IsAlive() {
  for (auto particle = this->particles.begin(); particle != this->particles.end(); ++particle) {
    if ((*particle)->alpha > 0) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00B7F1C0
// FUNCTION: fn_19244
void CVolcanoEffect::OnProcess() {
  int fadeIn;
  int fadeOut;

  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      particle->scale += this->id == EF_GUMGANG2 ? 0.1f : 0.3f;

      int tilt = particle->unknown_004C - 1;
      if (this->id == EF_GUMGANG2) {
        if (tilt < 30) {
          tilt = 30;
        }
      } else if (tilt < 40) {
        tilt = 40;
      }
      particle->unknown_004C = tilt;

      float& spin = *reinterpret_cast<float*>(&particle->unknown_00C4);
      spin = static_cast<float>(static_cast<int>(spin) + (this->id != EF_GUMGANG2) * 2 + 3);

      switch (this->id) {
      case EF_GANBANTEIN:
        fadeIn = 20;
        fadeOut = 8;
        break;
      case EF_VOLCANO:
      case EF_DELUGE:
      case EF_VIOLENTGALE:
      case EF_LANDPROTECTOR:
      case EF_GUMGANG2:
        fadeIn = 20;
        fadeOut = 2;
        break;
      case EF_GUMGANG3:
      case EF_GUMGANG4:
        fadeIn = 10;
        fadeOut = 2;
        break;
      case EF_GUMGANG5:
      case EF_GUMGANG6:
      case EF_GUMGANG7:
      case EF_GUMGANG8:
      case EF_GUMGANG9:
        fadeIn = 20;
        fadeOut = 5;
        break;
      }

      if (particle->unknown_00E8 > 10) {
        const int alpha = particle->alpha - fadeOut;
        particle->alpha = alpha > 0 ? alpha : 0;
      } else {
        particle->alpha += fadeIn;
      }

      const int segment = particle->unknown_000C;
      const float radius = g_sinTable[segment * 9] * particle->unknown_00C0;
      const int angle =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * segment) + spin) % 360;
      const float spread = radius * g_cosTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float height = radius * g_sinTable[tilt];

      vector3d corners[2] = {};
      corners[0].x = particle->scale * cosine;
      corners[0].y = 0.0f;
      corners[0].z = particle->scale * sine;
      corners[1].x = corners[0].x + spread * cosine;
      corners[1].y = 0.0f - height;
      corners[1].z = corners[0].z + spread * sine;

      particle->vertices[3].x = corners[0].x;
      particle->vertices[3].y = corners[0].y;
      particle->vertices[3].z = corners[0].z;
      particle->vertices[1].x = corners[1].x;
      particle->vertices[1].y = corners[1].y;
      particle->vertices[1].z = corners[1].z;

      if (segment != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner = corners[0];
      this->lastOuter = corners[1];
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B7F7B0
// FUNCTION: fn_19245
void CVolcanoEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->unknown_000C <= 0) {
      continue;
    }

    particle->destinationBlend = 6;

    const int segment = particle->unknown_000C;
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
