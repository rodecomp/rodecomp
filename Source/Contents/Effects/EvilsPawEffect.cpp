#include "Contents/Effects/EvilsPawEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FED20
void CEvilsPawEffect::Slot01() {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (particle) {
    particle->SetColor(255, 255, 255);
    particle->alpha = 255;
    particle->scale = static_cast<float>(this->size);
    particle->unknown_00D0 = this->offset.x;
    particle->unknown_00D4 = this->offset.y;
    particle->unknown_00D8 = this->offset.z;
    particle->sourceBlend = 5;
    particle->destinationBlend = 6;
    InitParticleQuad(particle->vertices);
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
void CEvilsPawEffect::Slot07() {
  this->textureIndex = 0;
  this->delayCounter = this->frameDelay;
  for (CParticle* particle : this->particles) {
    particle->SetColor(255, 255, 255);
    particle->alpha = 255;
    particle->scale = static_cast<float>(this->size);
    particle->unknown_00D0 = this->offset.x;
    particle->unknown_00D4 = this->offset.y;
    particle->unknown_00D8 = this->offset.z;
  }
}

// OFFSET: 0x006FF060
// FUNCTION: fn_06959
bool CEvilsPawEffect::Slot06() {
  return true;
}

// OFFSET: 0x006FF070
// FUNCTION: fn_06960
void CEvilsPawEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
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

      particle->position.x = this->position[0] + this->offset.x;
      particle->position.y = this->position[1] + this->offset.y;
      particle->position.z = this->position[2] + this->offset.z;
    }
    ++(*it)->unknown_00E8;
  }

  if (--this->delayCounter < 0) {
    ++this->textureIndex;
    this->delayCounter = this->frameDelay;
    if (this->textureIndex > this->textures.size() - 1) {
      this->textureIndex = 0;
    }
  }
}

// OFFSET: 0x006FF3E0
// FUNCTION: fn_06961
void CEvilsPawEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
