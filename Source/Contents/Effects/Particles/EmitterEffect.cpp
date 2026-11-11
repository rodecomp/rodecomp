#include "Contents/Effects/Particles/EmitterEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

namespace {
// INLINED
__forceinline bool IsParticleAlive(const CParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x00B4FAF0
// FUNCTION: fn_19008
CEmitterEffect::CEmitterEffect()
    : CEffect(974), velocityMin{-2.0f, 2.0f, -2.0f}, velocityMax{2.0f, -1.0f, 2.0f}, acceleration{0.7f, -2.0f, 1.0f}, spread{-1.0f, 2.0f, -1.0f},
      emitCarry(0.0f), timeScale(1.0f / 60.0f), scaleMin(3.0f), scaleMax(8.0f), lifeMin(5.0f), lifeMax(10.0f), rateMin(20.0f), rateMax(100.0f),
      width(1.0f), height(1.0f), red(255), green(255), blue(255), alpha(255), texture("smoke2.bmp"), sourceBlend(5), destinationBlend(2), count(40),
      particleSpeed(1.0f), depth(1), billboard(0), rotation{0.0f, 0.0f, 0.0f}, fixedAlpha(0), particles(), pool(), duration(-1) {
}

// OFFSET: 0x00B4FD90
// FUNCTION: fn_19009
CEmitterEffect::~CEmitterEffect() {
}

// OFFSET: 0x00B50010
// FUNCTION: fn_19011
void CEmitterEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
  this->pool.Clear();
}

// OFFSET: 0x00B50050
// FUNCTION: fn_19012
void CEmitterEffect::Open() {
  for (int i = 0; i < this->count; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->speedParameter = this->particleSpeed;
      this->InitParticle(particle);
      this->pool.Add(particle);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B500E0
// FUNCTION: fn_19013
void CEmitterEffect::InitParticle(CParticle* particle) {
  particle->position.x = this->position[0];
  particle->position.y = this->position[1];
  particle->position.z = this->position[2];
  particle->SetTexture(this->texture.c_str());
  particle->life = RandomRange(this->lifeMin, this->lifeMax);
  particle->scale = RandomRange(this->scaleMin, this->scaleMax);
  particle->motionVelocity.x = RandomRange(this->velocityMin.x, this->velocityMax.x);
  particle->motionVelocity.y = RandomRange(this->velocityMin.y, this->velocityMax.y);
  particle->motionVelocity.z = RandomRange(this->velocityMin.z, this->velocityMax.z);
  const float x = 0.0f + RandomRange(-this->spread.x, this->spread.x);
  const float y = 0.0f + (RandomRange(-this->spread.y, this->spread.y) - 10.0f);
  const float z = 0.0f + RandomRange(-this->spread.z, this->spread.z);
  particle->offsetX = x;
  particle->offsetY = y;
  particle->alpha = 0;
  particle->offsetZ = z;
  particle->elapsedFrames = 0;

  if (this->sourceBlend != -1) {
    particle->sourceBlend = this->sourceBlend;
  }

  if (this->destinationBlend != -1) {
    particle->destinationBlend = this->destinationBlend;
  }
}

// OFFSET: 0x00B502B0
// FUNCTION: fn_19014
void CEmitterEffect::ResetParticles() {
  if (this->fixedAlpha == 1 && 0.0f == this->rateMin && 0.0f == this->rateMax) {
    this->pool.ActivateAll();
  }
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->InitParticle(particle);
    if (this->pool.IsActive(particle) == true && (IsParticleAlive(particle) == true || this->fixedAlpha == 1)) {
      particle->alpha = this->alpha;
    }
  }
}

// OFFSET: 0x00B50370
// FUNCTION: fn_19015
bool CEmitterEffect::IsAlive() {
  if (this->duration > 0 && this->frame > this->duration) {
    return false;
  }

  return this->ActorExists();
}

// OFFSET: 0x00B50390
// FUNCTION: fn_19016
void CEmitterEffect::OnProcess() {
  if (!this->emissionStopped) {
    const float rate = static_cast<float>(static_cast<int>(RandomRange(this->rateMin, this->rateMax))) * this->timeScale;
    int spawn = static_cast<int>(rate);
    float carry = rate - static_cast<float>(spawn) + this->emitCarry;
    this->emitCarry = carry;

    if (carry > 1.0f) {
      const int whole = static_cast<int>(carry);
      spawn += whole;
      this->emitCarry = carry - static_cast<float>(whole);
    }

    for (int i = 0; i < spawn; ++i) {
      CParticle* particle = this->pool.Acquire();
      if (particle) {
        this->InitParticle(particle);
        if (this->fixedAlpha == 1) {
          particle->alpha = this->alpha;
        }
      }
    }
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->UpdateParticle(*it);
  }
}

// OFFSET: 0x00B507E0
// FUNCTION: fn_19017
void CEmitterEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CParticle* particle = *it) {
      ParticleQuad quad;
      quad.billboard = this->billboard;
      quad.unused = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = 0.0f;
      quad.right = 1.0f;
      quad.depth = this->depth;
      particle->Render(quad);
    }
  }
}

// OFFSET: 0x00B508B0
// FUNCTION: fn_19018
void CEmitterEffect::UpdateParticle(CParticle* particle) {
  particle->position.x = this->position[0];
  particle->position.y = this->position[1];
  particle->position.z = this->position[2];

  if (this->pool.IsActive(particle) == true) {
    const int age = particle->elapsedFrames;

    if (IsParticleAlive(particle) != true && (!this->fixedAlpha || this->emissionStopped)) {
      this->pool.Release(particle);
    } else {
      particle->SetColor(static_cast<short>(this->red), static_cast<short>(this->green), static_cast<short>(this->blue));

      if (!this->fixedAlpha) {
        const float fade = this->timeScale * 255.0f;
        const float value = static_cast<float>(particle->alpha);

        if (60.0f > particle->life * 60.0f - static_cast<float>(age)) {
          const int faded = static_cast<int>(value - fade);
          particle->alpha = faded < 0 ? 0 : faded;
        } else {
          const int raised = static_cast<int>(fade + value);
          particle->alpha = raised > 255 ? 255 : raised;
        }
      }

      const float step = this->timeScale;
      const float speed = particle->speedParameter;
      particle->offsetY += particle->motionVelocity.y * speed * step;
      particle->offsetZ += particle->motionVelocity.z * speed * step;
      particle->offsetX += particle->motionVelocity.x * speed * step;
      particle->position.y += particle->offsetY;
      particle->position.z += particle->offsetZ;
      particle->position.x += particle->offsetX;
      particle->motionVelocity.x += step * this->acceleration.x;
      particle->motionVelocity.y += step * this->acceleration.y;
      particle->motionVelocity.z += step * this->acceleration.z;

      if (this->billboard == 1) {
        matrix transform;
        transform.SetScale(particle->scale * this->width, particle->scale * this->height, 1.0f);
        transform.AppendXRotation(GetRadian(static_cast<int>(this->rotation[0])));
        transform.AppendYRotation(GetRadian(static_cast<int>(this->rotation[1])));
        transform.AppendZRotation(GetRadian(static_cast<int>(this->rotation[2])));
        transform.AppendTranslate(&particle->position.x);
        particle->transform = transform;
      } else {
        matrix rotation;
        std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
        const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
        rotation.MakeXRotation(view->camera.pitch / 180.0f * 3.141592f);
        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(particle->scale * this->width, particle->scale * this->height, 1.0f);
        matrix result;
        result.Compose(scale, rotation);
        particle->transform = result;
      }
    }
  }

  ++particle->elapsedFrames;
}

// OFFSET: 0x00B50C60
// FUNCTION: fn_19019
void CEmitterEffect::SetCount(int value) {
  if (this->count != value) {
    this->count = value;
    this->Close();
    this->Open();
  }
}

// OFFSET: 0x00B50C90
// FUNCTION: fn_19020
void CEmitterEffect::SetParticleSpeed(float value) {
  if (this->particleSpeed != value) {
    this->particleSpeed = value;
    for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
      (*it)->speedParameter = this->particleSpeed;
    }
  }
}

// OFFSET: 0x00B50CE0
// FUNCTION: fn_19021
void CEmitterEffect::CheckRange() {
  if (this->InRange() == true) {
    this->outsideRange = 0;
    this->emissionStopped = 0;
    return;
  }

  this->emissionStopped = 1;
  if (this->pool.active == 0) {
    this->outsideRange = 1;
  }
}

// OFFSET: 0x00B7F9C0
// FUNCTION: fn_19246
void CEmitterEffect::SetTexture(std::string name) {
  this->texture = name;
}
