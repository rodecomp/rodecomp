#include "Contents/CombatTypes.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Skills/ThrowEffects.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00B7CB10
// FUNCTION: fn_19227
CThrowItemEffect::CThrowItemEffect()
    : CEffect(-1), travelFrames(0), launchDelay(0), projectileCount(1), trailCount(4), heightDelta(0), arcHeight(0), trailX(0),
      trailY(0), trailZ(0) {
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
    this->projectileCount = 8;
    break;
  case EF_THROW_MULTIPLE_COIN:
    this->projectileCount = 10;
    break;
  case EF_KAIHOU:
    if (CGameActor* owner = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
      this->projectileCount = owner->charmCount;
      this->trailCount = 1;
    }
    break;
  }

  for (int row = 0; row < this->projectileCount; ++row) {
    for (int column = 0; column < this->trailCount; ++column) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = column;
      particle->rowIndex = row;
      particle->groupIndex = 1;

      switch (this->id) {
      case EF_THROWITEM:
        this->effectLifetime = 300;
        this->arcHeight = 30.0f;
        this->InitTrajectory(particle);
        particle->SetTexture("유저인터페이스\\item\\빈병.bmp");
        particle->scale = 1.5f;
        particle->SetColor(135, 255, 135);
        particle->angle = 0;
        particle->speedParameter = particle->animationParameter / 25.0f;
        particle->alpha = 255;
        particle->offsetX = this->position[0];
        particle->offsetY = this->position[1] - 8.0f;
        particle->offsetZ = this->position[2];
        particle->destinationBlend = 6;
        break;
      case EF_THROWITEM2:
        particle->SetTexture("유저인터페이스\\item\\빈포션병.bmp");
        this->InitPotionParticle(particle, this->effectArgument == -1 ? 1 : this->effectArgument);
        break;
      case EF_THROWITEM3:
        this->effectLifetime = 300;
        this->arcHeight = 10.0f;
        particle->SetTexture("유저인터페이스\\item\\돌.bmp");
        particle->scale = 1.0f;
        particle->SetColor(255, 255, 255);
        particle->angle = 0;
        particle->speedParameter = 2.0f;
        particle->alpha = 255;
        particle->offsetX = this->position[0];
        particle->offsetY = this->position[1] - 8.0f;
        particle->offsetZ = this->position[2];
        particle->destinationBlend = 6;
        break;
      case EF_THROWITEM4:
        this->launchDelay = 5;
        particle->SetTexture("유저인터페이스\\item\\화염병.bmp");
        this->InitFlameParticle(particle);
        break;
      case EF_THROWITEM5:
        particle->SetTexture("유저인터페이스\\item\\버서크포션.bmp");
        this->InitPotionParticle(particle, this->effectArgument == -1 ? 1 : this->effectArgument);
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
        this->InitPotionParticle(particle, this->effectArgument == -1 ? 1 : this->effectArgument);
        break;
      case EF_THROWITEM4_1:
        this->launchDelay = 10;
        particle->SetTexture("유저인터페이스\\item\\염산병.bmp");
        this->InitFlameParticle(particle);
        break;
      case EF_THROW_BAKURETSU:
        particle->SetTexture("유저인터페이스\\item\\쿠나이_화.bmp");
        this->InitSpinParticle(particle);
        particle->speedParameter = 1.0f;
      case EF_THROW_HAPPOKUNAI:
        this->effectLifetime = 200;
        this->arcHeight = 1.0f;
        particle->SetTexture("유저인터페이스\\item\\쿠나이_독.bmp");
        particle->scale = 3.0f;
        particle->SetColor(255, 200, 255);
        particle->angle = 330.0f;
        particle->speedParameter = 1.5f;
        particle->alpha = 255;
        particle->offsetX = this->position[0];
        particle->offsetZ = this->position[2];
        particle->offsetY = this->position[1] - 8.0f;
        particle->destinationBlend = 6;
        if (this->sourceActor == 0) {
          this->sourcePosition[1] = this->position[1];
          this->sourcePosition[2] = this->position[2];
          this->sourcePosition[0] = this->position[0] - 8.0f;
        }
        break;
      case EF_THROW_MULTIPLE_COIN:
        this->effectLifetime = 200;
        this->arcHeight = 10.0f;
        particle->SetTexture("effect\\coin_a.bmp");
        particle->scale = 2.5f;
        particle->SetColor(255, 200, 0);
        particle->angle = 330.0f;
        particle->speedParameter = 2.0f;
        particle->alpha = 255;
        particle->destinationBlend = 6;
        particle->offsetZ = this->position[2];
        particle->offsetY = this->position[1] - 8.0f;
        particle->offsetX = this->position[0];
        break;
      case EF_KAIHOU:
        if (CGameActor* owner = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
          if (owner->charmType == CHARM_TYPE_WATER) {
            particle->SetTexture("effect\\WaterOrb.bmp");
            particle->scale = 1.0f;
          } else if (owner->charmType == CHARM_TYPE_LAND) {
            particle->SetTexture("effect\\StoneOrb.bmp");
          } else if (owner->charmType == CHARM_TYPE_FIRE) {
            particle->SetTexture("effect\\FireOrb.bmp");
          } else if (owner->charmType == CHARM_TYPE_WIND) {
            particle->SetTexture("effect\\LightningOrb.bmp");
          }

          this->effectLifetime = 200;
          particle->scale = 2.0f;
          particle->angle = 330.0f;
          particle->speedParameter = 1.5f;
          particle->alpha = 255;
          particle->offsetZ = this->position[2];
          particle->offsetY = this->position[1] - 15.0f;
          particle->offsetX = this->position[0];
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
  this->effectLifetime = 300;
  this->arcHeight = 1.0f;
  this->InitTrajectory(particle);
  particle->scale = 1.5f;
  particle->SetColor(255, 255, 255);
  particle->angle = 0;
  particle->alpha = 255;
  particle->speedParameter = particle->animationParameter / 25.0f;
  const vector3d offset = {this->position[0], this->position[1] - 8.0f, this->position[2]};
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;
  particle->destinationBlend = 6;
}

// OFFSET: 0x00B7D4D0
// FUNCTION: fn_19232
void CThrowItemEffect::InitPotionParticle(CParticle* particle, int kind) {
  this->effectLifetime = 300;
  this->arcHeight = 30.0f;
  this->InitTrajectory(particle);
  particle->scale = 1.5f;
  particle->angle = 0;
  particle->alpha = 255;
  particle->speedParameter = particle->animationParameter / 25.0f;
  const vector3d offset = {this->position[0], this->position[1] - 8.0f, this->position[2]};
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;

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
__declspec(noinline) void CThrowItemEffect::InitSpinParticle(CParticle* particle) {
  this->effectLifetime = 200;
  this->arcHeight = 1.0f;
  particle->scale = 2.0f;
  particle->SetColor(255, 200, 255);
  particle->angle = 330.0f;
  particle->speedParameter = 3.0f;
  particle->alpha = 255;
  const vector3d offset = {this->position[0], this->position[1] - 8.0f, this->position[2]};
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;
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
    vector3d& targetPosition = (*particle)->position;
    targetPosition.x = this->position[0];
    targetPosition.y = this->position[1];
    targetPosition.z = this->position[2];

    if (target != nullptr && this->launchDelay <= this->frame) {
      float& angle = target->speedParameter;
      float& rotation = target->angle;
      float& offsetX = target->offsetX;
      float& offsetY = target->offsetY;
      float& offsetZ = target->offsetZ;

      if (target->columnIndex == 0) {
        rotation = static_cast<float>(static_cast<int>(rotation + 45.0f) % 360);
      } else {
        this->trailAlpha -= target->columnIndex != 1 ? 25 : 150;
        target->alpha = this->trailAlpha;
        offsetX = this->trailX;
        offsetY = this->trailY;
        offsetZ = this->trailZ;
        rotation = this->trailRotation;
      }

      const float progress = static_cast<float>(this->frame) / static_cast<float>(this->travelFrames);
      float height = 0.0f;

      if (progress > 1.05f) {
        target->alpha = std::ranges::max(target->alpha - 20, 0);
      } else {
        const int index = target->phaseParameter;
        const float radius = angle / target->animationParameter;
        offsetX = angle * g_cosTable[index] + offsetX;
        offsetY = radius * this->heightDelta + offsetY;
        offsetZ = angle * g_sinTable[index] + offsetZ;
        height = g_sinTable[-static_cast<int>(progress * -180.0f)] * this->arcHeight;
      }

      this->trailX = offsetX;
      this->trailY = offsetY;
      this->trailZ = offsetZ;
      this->trailRotation = rotation;
      this->trailAlpha = target->alpha;

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

        if (this->id == 0x3D5 && target->rowIndex == 0 && target->columnIndex == 0) {
          CRenderObject* object = FindRenderObject(this->actor);

          if (object != nullptr) {
            float& objectAngle = object->render.angle;
            objectAngle = static_cast<float>(static_cast<int>(objectAngle + 45.0f) % 360);
          }
        }
      }
    }

    ++target->elapsedFrames;
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
  this->heightDelta = this->sourcePosition[1] - this->position[1];

  const unsigned short heading = static_cast<unsigned short>(static_cast<int>(DirectionAngle(deltaX, deltaZ)));
  int offset = 0;
  if (this->id == 981) {
    offset = static_cast<unsigned short>(static_cast<short>(360 / this->projectileCount) * static_cast<short>(particle->rowIndex));
  }

  particle->phaseParameter = static_cast<short>(heading + offset) % 360;

  const float distance = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  particle->animationParameter = distance;

  for (int step = 0; step < 100; ++step) {
    if (static_cast<float>(step) * particle->speedParameter > distance) {
      this->travelFrames = step;
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
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = row;
      particle->groupIndex = 1;

      switch (this->id) {
      case EF_GANBANTEIN:
        particle->speedParameter = 15.0f;
        particle->phaseParameter = 70;
        particle->animationParameter = 0.0f;
        particle->angleParameter = 360;
        particle->alpha = 1;
        particle->angle = static_cast<float>(row * 90);
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
        particle->speedParameter = 25.0f;
        particle->phaseParameter = 80;
        particle->animationParameter = 0.0f;
        particle->angleParameter = 360;
        particle->alpha = 1;
        particle->angle = static_cast<float>(baseAngle);
        particle->scale = static_cast<float>(row) + 1.0f;
        break;
      case EF_GUMGANG5:
        particle->alpha = 1;
        particle->angleParameter = 360;
        particle->speedParameter = 25.0f;
        particle->phaseParameter = 80;
        particle->angle = static_cast<float>(row * 90);
        particle->animationParameter = 0.0f;
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

      particle->InitParticleQuad(particle->vertices);
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
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];

    if (particle) {
      const float scale = particle->scale + (this->id == EF_GUMGANG2 ? 0.1f : 0.3f);
      particle->scale = scale;

      int tilt = particle->phaseParameter - 1;
      if (this->id == EF_GUMGANG2) {
        tilt = tilt < 30 ? 30 : tilt;
      } else {
        tilt = tilt < 40 ? 40 : tilt;
      }
      particle->phaseParameter = tilt;

      float& spin = particle->angle;
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

      if (particle->elapsedFrames > 10) {
        const int alpha = particle->alpha - fadeOut;
        particle->alpha = alpha > 0 ? alpha : 0;
      } else {
        particle->alpha += fadeIn;
      }

      const int segment = particle->columnIndex;
      const float radius = g_sinTable[segment * 9] * particle->speedParameter;
      const int angle =
        static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) + spin) % 360;
      const float spread = radius * g_cosTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float height = radius * g_sinTable[tilt];

      EffectPoint corners[2];
      corners[0].x = scale * cosine;
      corners[0].y = 0.0f;
      corners[0].z = scale * sine;
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

      this->lastInner.x = corners[0].x;
      this->lastInner.y = corners[0].y;
      this->lastInner.z = corners[0].z;
      this->lastOuter.x = corners[1].x;
      this->lastOuter.y = corners[1].y;
      this->lastOuter.z = corners[1].z;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B7F7B0
// FUNCTION: fn_19245
void CVolcanoEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->columnIndex <= 0) {
      continue;
    }

    particle->destinationBlend = 6;

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
