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

    return (std::ranges::max)(rand() % range, 0);
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

  // INLINED
  __forceinline vector3d Sum(const vector3d& left, const vector3d& right) {
    return vector3d{left.x + right.x, left.y + right.y, left.z + right.z};
  }

  // INLINED
  __forceinline void SetOffset(CParticle* particle, float x, float y, float z) {
    particle->offsetX = x;
    particle->offsetY = y;
    particle->offsetZ = z;
  }
}

// OFFSET: 0x006FB9A0
// FUNCTION: fn_06914
CCloud::CCloud()
    : CEffect(-1), driftX(0), driftTargetX(0), driftZ(0), driftTargetZ(0), phase(0), baseScale(0), alpha(0), fadeInEnd(0), fadeOutStart(0),
      red(255), green(255), blue(255), scaleBase(0), scaleRange(0), scaleAmplitude(0), fadeInStep(0), fadeOutStep(0), fadeInBase(0),
      fadeInRange(0), fadeOutBase(0), fadeOutRange(0), offsetRangeY(0), offsetRangeX(0), offsetRangeZ(0), renderFlag(1) {
}

// OFFSET: 0x006FBAE0
// FUNCTION: fn_46097
__declspec(noinline) CCloud::~CCloud() {
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
  SetOffset(particle, static_cast<float>(Random(static_cast<int>(this->offsetRangeX))), static_cast<float>(Random(static_cast<int>(this->offsetRangeY))), static_cast<float>(Random(static_cast<int>(this->offsetRangeZ))));

  this->particle->SetColor(this->red, this->green, this->blue);
  this->particle->alpha = 0;
  this->particle->scale = this->baseScale;
  CParticle* cloud = this->particle;
  int index = Random(4);
  ++index;
  cloud->SetTexture(FormatOwnedString("effect\\cloud%d.tga", index).c_str());
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
  if (!this->particle) {
    return;
  }

  this->UpdateAlpha();
  this->Drift();
  this->UpdateScale();
  this->UpdateTransform();
  ++this->particle->elapsedFrames;
}

// OFFSET: 0x006FC090
// FUNCTION: fn_06921
void CCloud::Render() {
  if (this->particle) {
    this->particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x006FC0D0
// FUNCTION: fn_46170
__forceinline void CCloud::UpdateAlpha() {
  CParticle* particle = this->particle;
  const int frame = particle->elapsedFrames;
  if (frame < this->fadeInEnd) {
    this->alpha = std::min(this->fadeInStep + this->alpha, 255.0f);
  } else if (frame > this->fadeOutStart) {
    this->alpha -= this->fadeOutStep;
    if (this->alpha <= 0.0f) {
      this->ResetParticles();
      return;
    }
  }
  particle->alpha = static_cast<int>(this->alpha);
}

// OFFSET: 0x006FC160
// FUNCTION: fn_06922
void CCloud::Drift() {
  StepDrift(this->driftX, this->driftTargetX);
  StepDrift(this->driftZ, this->driftTargetZ);

  CParticle* particle = this->particle;
  vector3d offset = particle->offset;
  offset.x = g_sinTable[this->driftX] * 0.05f + offset.x;
  offset.z = g_sinTable[this->driftZ] * 0.05f + offset.z;
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;

  this->particle->position = Sum(this->positionVector, offset);
}

// OFFSET: 0x006FC300
// FUNCTION: fn_46171
__forceinline void CCloud::UpdateScale() {
  this->phase = (this->phase + 1) % 360;
  this->particle->scale = this->baseScale * this->scaleAmplitude * g_sinTable[this->phase] + this->baseScale;
}

// OFFSET: 0x006FC350
// FUNCTION: fn_46172
__forceinline void CCloud::UpdateTransform() {
  matrix scale;
  scale.SetScale(this->particle->scale, this->particle->scale, 1.0f);
  matrix rotation;
  rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
  matrix transform;
  transform.Compose(scale, rotation);
  this->particle->transform = matrix(transform);
}
