#include "Contents/Effects/Skills/CloudEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

namespace {
  // INLINED
  __forceinline int Random(int range) {
    if (range == 0) {
      return 0;
    }

    const int value = rand() % range;
    return value >= 0 ? value : 0;
  }

  // INLINED
  __forceinline void StepDrift(int& angle, int& target) {
    int goal = target;
    if (goal > angle) {
      angle = std::min(360, angle + 1);
      if (goal < angle) {
        goal = Random(angle);
      }
    } else {
      angle = std::max(0, angle - 1);
      if (goal >= angle) {
        goal = 360 - Random(angle);
      }
    }
    target = goal;
  }
}

// OFFSET: 0x006FB9A0
// FUNCTION: fn_06914
CCloud::CCloud()
    : CEffect(-1), driftX(0), driftTargetX(0), driftZ(0), driftTargetZ(0), phase(0), baseScale(0), alpha(0), fadeInEnd(0), fadeOutStart(0),
      red(255), green(255), blue(255), scaleBase(0), scaleRange(0), scaleAmplitude(0), fadeInStep(0), fadeOutStep(0), fadeInBase(0),
      fadeInRange(0), fadeOutBase(0), fadeOutRange(0), offsetRangeY(0), offsetRangeX(0), offsetRangeZ(0), renderFlag(1) {
}

// OFFSET: 0x006FBBA0
// FUNCTION: fn_06916
void CCloud::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }
}

// OFFSET: 0x006FBBD0
// FUNCTION: fn_06917
void CCloud::Open() {
  this->particle = CreateParticle(&this->positionVector, 1);
  if (CParticle* particle = this->particle) {
    particle->effectId = this->id;
    particle->columnIndex = 0;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    this->particle->sourceBlend = 5;
    this->particle->destinationBlend = 6;
  }
}

// OFFSET: 0x006FBC30
// FUNCTION: fn_06918
void CCloud::ResetParticles() {
  this->driftX = Random(360);
  this->driftTargetX = Random(360);
  this->driftZ = Random(360);
  this->driftTargetZ = Random(360);
  this->phase = Random(360);

  const int scaleRange = static_cast<int>(this->scaleRange);
  const int scaleOffset = Random(scaleRange);
  this->alpha = 0.0f;
  this->baseScale = static_cast<float>(scaleOffset) + this->scaleBase;
  this->fadeInEnd = this->fadeInBase + Random(this->fadeInRange);
  this->fadeOutStart = this->fadeOutBase + Random(this->fadeOutRange);

  if (!this->particle) {
    return;
  }

  this->particle->elapsedFrames = 0;
  CParticle* particle = this->particle;
  const int rangeZ = static_cast<int>(this->offsetRangeZ);
  const int offsetZ = Random(rangeZ);
  const int rangeY = static_cast<int>(this->offsetRangeY);
  const int offsetY = Random(rangeY);
  const int rangeX = static_cast<int>(this->offsetRangeX);
  const int offsetX = Random(rangeX);
  particle->offsetX = static_cast<float>(offsetX);
  particle->offsetY = static_cast<float>(offsetY);
  particle->offsetZ = static_cast<float>(offsetZ);

  this->particle->SetColor(this->red, this->green, this->blue);
  this->particle->alpha = 0;
  this->particle->scale = this->baseScale;
  this->particle->SetTexture(FormatOwnedString("effect\\cloud%d.tga", Random(4) + 1).c_str());
  this->particle->SetRenderFlag(this->renderFlag);
}

// OFFSET: 0x006FBEC0
// FUNCTION: fn_06919
bool CCloud::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006FBED0
// FUNCTION: fn_06920
void CCloud::OnProcess() {
  CParticle* particle = this->particle;
  if (!particle) {
    return;
  }

  const int frame = particle->elapsedFrames;
  if (frame < this->fadeInEnd) {
    this->alpha = std::min({this->alpha + this->fadeInStep, 255.0f});
    particle->alpha = static_cast<int>(this->alpha);
  } else if (frame > this->fadeOutStart) {
    this->alpha -= this->fadeOutStep;
    if (this->alpha <= 0.0f) {
      this->ResetParticles();
    } else {
      particle->alpha = static_cast<int>(this->alpha);
    }
  } else {
    particle->alpha = static_cast<int>(this->alpha);
  }

  this->Drift();
  this->phase = (this->phase + 1) % 360;
  this->particle->scale = this->baseScale * this->scaleAmplitude * g_sinTable[this->phase] + this->baseScale;

  matrix scale;
  scale.SetScale(this->particle->scale, this->particle->scale, 1.0f);
  matrix rotation;
  rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
  matrix transform;
  transform.Compose(scale, rotation);
  this->particle->transform = transform;
  ++this->particle->elapsedFrames;
}

// OFFSET: 0x006FC090
// FUNCTION: fn_06921
void CCloud::Render() {
  if (this->particle) {
    this->particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x006FC160
// FUNCTION: fn_06922
void CCloud::Drift() {
  StepDrift(this->driftX, this->driftTargetX);
  StepDrift(this->driftZ, this->driftTargetZ);

  CParticle* particle = this->particle;
  vector3d offset = *reinterpret_cast<const vector3d*>(&particle->offsetX);
  offset.x = g_sinTable[this->driftX] * 0.05f + offset.x;
  offset.z = g_sinTable[this->driftZ] * 0.05f + offset.z;
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;

  const vector3d position = {offset.x + this->position[0], offset.y + this->position[1], offset.z + this->position[2]};
  this->particle->position = position;
}
