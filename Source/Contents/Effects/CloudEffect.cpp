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

namespace {
  // INLINED
  __forceinline void StepDrift(int& angle, int& target) {
    int goal = target;
    if (goal > angle) {
      const int next = std::min(360, angle + 1);
      angle = next;
      if (goal < next) {
        goal = next ? std::max(rand() % next, 0) : 0;
      }
    } else {
      const int next = std::max(0, angle - 1);
      angle = next;
      if (goal >= next) {
        goal = 360 - (next ? std::max(rand() % next, 0) : 0);
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
  this->particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
  if (CParticle* particle = this->particle) {
    particle->unknown_0004 = this->id;
    particle->unknown_000C = 0;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    this->particle->sourceBlend = 5;
    this->particle->destinationBlend = 6;
  }
}

// OFFSET: 0x006FBC30
// FUNCTION: fn_06918
void CCloud::ResetParticles() {
  this->driftX = std::max(0, rand() % 360);
  this->driftTargetX = std::max(0, rand() % 360);
  this->driftZ = std::max(0, rand() % 360);
  this->driftTargetZ = std::max(0, rand() % 360);
  this->phase = std::max(0, rand() % 360);

  const int scaleRange = static_cast<int>(this->scaleRange);
  const int scaleOffset = scaleRange ? std::max(0, rand() % scaleRange) : 0;
  this->alpha = 0.0f;
  this->baseScale = static_cast<float>(scaleOffset) + this->scaleBase;
  this->fadeInEnd = this->fadeInBase + (this->fadeInRange ? std::max(0, rand() % this->fadeInRange) : 0);
  this->fadeOutStart = this->fadeOutBase + (this->fadeOutRange ? std::max(0, rand() % this->fadeOutRange) : 0);

  if (!this->particle) {
    return;
  }

  this->particle->unknown_00E8 = 0;
  CParticle* particle = this->particle;
  const int rangeZ = static_cast<int>(this->offsetRangeZ);
  const int offsetZ = rangeZ ? std::max(0, rand() % rangeZ) : 0;
  const int rangeY = static_cast<int>(this->offsetRangeY);
  const int offsetY = rangeY ? std::max(0, rand() % rangeY) : 0;
  const int rangeX = static_cast<int>(this->offsetRangeX);
  const int offsetX = rangeX ? std::max(0, rand() % rangeX) : 0;
  particle->unknown_00D0 = static_cast<float>(offsetX);
  particle->unknown_00D4 = static_cast<float>(offsetY);
  particle->unknown_00D8 = static_cast<float>(offsetZ);

  this->particle->SetColor(this->red, this->green, this->blue);
  this->particle->alpha = 0;
  this->particle->scale = this->baseScale;
  this->particle->SetTexture(FormatOwnedString("effect\\cloud%d.tga", std::max(0, rand() % 4) + 1).c_str());
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

  const int frame = particle->unknown_00E8;
  if (frame < this->fadeInEnd) {
    const float value = this->alpha + this->fadeInStep;
    this->alpha = value > 255.0f ? 255.0f : value;
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
  ++this->particle->unknown_00E8;
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
  float offsetX = particle->unknown_00D0;
  const float offsetY = particle->unknown_00D4;
  float offsetZ = particle->unknown_00D8;
  offsetX = g_sinTable[this->driftX] * 0.05f + offsetX;
  offsetZ = g_sinTable[this->driftZ] * 0.05f + offsetZ;
  particle->unknown_00D0 = offsetX;
  particle->unknown_00D4 = offsetY;
  particle->unknown_00D8 = offsetZ;

  this->particle->position.x = offsetX + this->position[0];
  this->particle->position.y = offsetY + this->position[1];
  this->particle->position.z = offsetZ + this->position[2];
}
