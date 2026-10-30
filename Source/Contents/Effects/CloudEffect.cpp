#include "Contents/Effects/CloudEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

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
void CCloud::Slot01() {
  this->particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
  if (this->particle) {
    this->particle->unknown_0004 = this->id;
    this->particle->unknown_000C = 0;
    this->particle->unknown_0010 = 0;
    this->particle->unknown_0008 = 1;
    this->particle->sourceBlend = 5;
    this->particle->destinationBlend = 6;
  }
}

// OFFSET: 0x006FBC30
// FUNCTION: fn_06918
void CCloud::Slot07() {
  this->driftX = std::max(rand() % 360, 0);
  this->driftTargetX = std::max(rand() % 360, 0);
  this->driftZ = std::max(rand() % 360, 0);
  this->driftTargetZ = std::max(rand() % 360, 0);
  this->phase = std::max(rand() % 360, 0);

  const int scaleRange = static_cast<int>(this->scaleRange);
  const int scaleOffset = scaleRange ? std::max(rand() % scaleRange, 0) : 0;
  this->alpha = 0.0f;
  this->baseScale = static_cast<float>(scaleOffset) + this->scaleBase;
  this->fadeInEnd = this->fadeInBase + (this->fadeInRange ? std::max(rand() % this->fadeInRange, 0) : 0);
  this->fadeOutStart = this->fadeOutBase + (this->fadeOutRange ? std::max(rand() % this->fadeOutRange, 0) : 0);

  if (!this->particle) {
    return;
  }

  this->particle->unknown_00E8 = 0;
  CParticle* particle = this->particle;
  const int rangeZ = static_cast<int>(this->offsetRangeZ);
  const int offsetZ = rangeZ ? std::max(rand() % rangeZ, 0) : 0;
  const int rangeY = static_cast<int>(this->offsetRangeY);
  const int offsetY = rangeY ? std::max(rand() % rangeY, 0) : 0;
  const int rangeX = static_cast<int>(this->offsetRangeX);
  const int offsetX = rangeX ? std::max(rand() % rangeX, 0) : 0;
  particle->unknown_00D0 = static_cast<float>(offsetX);
  particle->unknown_00D4 = static_cast<float>(offsetY);
  particle->unknown_00D8 = static_cast<float>(offsetZ);

  this->particle->SetColor(this->red, this->green, this->blue);
  this->particle->alpha = 0;
  this->particle->scale = this->baseScale;
  this->particle->SetTexture(FormatOwnedString("effect\\cloud%d.tga", std::max(rand() % 4, 0) + 1).c_str());
  this->particle->SetRenderFlag(this->renderFlag);
}

// OFFSET: 0x006FBEC0
// FUNCTION: fn_06919
bool CCloud::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FBED0
// FUNCTION: fn_06920
void CCloud::Slot03() {
  CParticle* particle = this->particle;
  if (!particle) {
    return;
  }

  const int frame = particle->unknown_00E8;
  if (frame < this->fadeInEnd) {
    const float value = this->alpha + this->fadeInStep;
    this->alpha = value > 255.0f ? 255.0f : value;
    particle->alpha = static_cast<int>(this->alpha);
  } else if (frame > this->fadeOutStart) {
    this->alpha -= this->fadeOutStep;
    if (this->alpha <= 0.0f) {
      this->Slot07();
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
  ++this->particle->unknown_00E8;
}

// OFFSET: 0x006FC090
// FUNCTION: fn_06921
void CCloud::Slot04() {
  if (this->particle) {
    this->particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x006FC160
// FUNCTION: fn_06922
void CCloud::Drift() {
  int targetX = this->driftTargetX;
  if (targetX > this->driftX) {
    this->driftX = std::min(this->driftX + 1, 360);
    if (targetX < this->driftX) {
      targetX = this->driftX ? std::max(rand() % this->driftX, 0) : 0;
    }
  } else {
    this->driftX = std::max(this->driftX - 1, 0);
    if (targetX >= this->driftX) {
      targetX = 360 - (this->driftX ? std::max(rand() % this->driftX, 0) : 0);
    }
  }
  this->driftTargetX = targetX;

  int targetZ = this->driftTargetZ;
  if (targetZ > this->driftZ) {
    this->driftZ = std::min(this->driftZ + 1, 360);
    if (targetZ < this->driftZ) {
      targetZ = this->driftZ ? std::max(rand() % this->driftZ, 0) : 0;
    }
  } else {
    this->driftZ = std::max(this->driftZ - 1, 0);
    if (targetZ >= this->driftZ) {
      targetZ = 360 - (this->driftZ ? std::max(rand() % this->driftZ, 0) : 0);
    }
  }
  this->driftTargetZ = targetZ;

  CParticle* particle = this->particle;
  const float offsetX = particle->unknown_00D0;
  const float offsetY = particle->unknown_00D4;
  const float offsetZ = particle->unknown_00D8;
  particle->unknown_00D4 = offsetY;
  particle->unknown_00D0 = g_sinTable[this->driftX] * 0.05f + offsetX;
  particle->unknown_00D8 = g_sinTable[this->driftZ] * 0.05f + offsetZ;

  this->particle->position.x = particle->unknown_00D0 + this->position[0];
  this->particle->position.y = offsetY + this->position[1];
  this->particle->position.z = particle->unknown_00D8 + this->position[2];
}
