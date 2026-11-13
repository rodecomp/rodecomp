#include "Contents/Effects/Skills/EvilsPawEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdio>
#include <cstring>

// OFFSET: 0x006FEA20
// FUNCTION: fn_06954
CEvilsPawEffect::CEvilsPawEffect()
    : CEffect(1039), particles(), frameDelay(5), size(50), delayCounter(5), offset{0.0f, -40.0f, 0.0f}, textures(), textureIndex(0) {
}

// OFFSET: 0x006FECE0
// FUNCTION: fn_06956
void CEvilsPawEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x006FED20
// FUNCTION: fn_06957
void CEvilsPawEffect::Open() {
  CParticle* particle = CreateParticle(&this->positionVector, 5);
  if (particle) {
    particle->SetColor(255, 255, 255);
    particle->alpha = 255;
    particle->scale = static_cast<float>(this->size);
    particle->offsetX = this->offset.x;
    particle->offsetY = this->offset.y;
    particle->offsetZ = this->offset.z;
    particle->sourceBlend = 5;
    particle->destinationBlend = 6;
    particle->InitParticleQuad(particle->vertices);
    this->particles.push_back(particle);
  }

  for (int set = 0; set < 2; ++set) {
    for (int frame = 0; frame < 10; ++frame) {
      char buffer[64];
      std::sprintf(buffer, "effect\\%d-%d.tga", set + 1, frame + 1);
      this->textures.push_back(std::string(buffer));
    }
  }
}

// OFFSET: 0x006FEFC0
// FUNCTION: fn_06958
void CEvilsPawEffect::ResetParticles() {
  this->textureIndex = 0;
  this->delayCounter = this->frameDelay;
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->SetColor(255, 255, 255);
    particle->alpha = 255;
    particle->scale = static_cast<float>(this->size);
    particle->offsetX = this->offset.x;
    particle->offsetY = this->offset.y;
    particle->offsetZ = this->offset.z;
  }
}

// OFFSET: 0x006FF060
// FUNCTION: fn_06959
bool CEvilsPawEffect::IsAlive() {
  return true;
}

// OFFSET: 0x006FF070
// FUNCTION: fn_06960
void CEvilsPawEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      particle->SetTexture(this->textures[this->textureIndex].c_str());

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(3.141592f);
      const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
      rotation.AppendXRotation(view->camera.pitch / 180.0f * 3.141592f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;

      particle->position = vector3d{this->position[0] + this->offset.x, this->position[1] + this->offset.y, this->position[2] + this->offset.z};
    }
    ++(*it)->elapsedFrames;
  }

  if (--this->delayCounter < 0) {
    ++this->textureIndex;
    if (this->textures.size() - 1 < this->textureIndex) {
      this->textureIndex = 0;
    }
    this->delayCounter = this->frameDelay;
  }
}

// OFFSET: 0x006FF3E0
// FUNCTION: fn_06961
void CEvilsPawEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
