#include "Contents/Effects/Skills/CommonFloorEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x006FD760
// FUNCTION: fn_06933
CCommonFloorEffect::CCommonFloorEffect() : CEffect(1056) {
}

// OFFSET: 0x006FD8C0
// FUNCTION: fn_06935
void CCommonFloorEffect::SetColor(int red, int green, int blue) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x006FD900
// FUNCTION: fn_06936
void CCommonFloorEffect::SetParticleTexture(std::string name) {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetTexture(name);
  }
}

// OFFSET: 0x006FDB30
// FUNCTION: fn_06937
void CCommonFloorEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x006FDB70
// FUNCTION: fn_06938
void CCommonFloorEffect::Open() {
  for (int i = 0; i < 2; ++i) {
    const int angle = i * 180;
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->SetTexture("effect\\ra_unlimit.tga");
      particle->effectId = this->id;
      particle->groupIndex = 1;
      particle->columnIndex = i;
      particle->rowIndex = 0;
      particle->alpha = 90;
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);
      particle->phaseParameter = angle;
      particle->speedParameter = 6.0f;
      particle->scale = 15.0f;
      particle->InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x006FDCE0
// FUNCTION: fn_06939
void CCommonFloorEffect::ResetParticles() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->flags = 5;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    particle->scale = *reinterpret_cast<float*>(&this->scaleParameter);
    *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(*reinterpret_cast<int*>(this->angleParameter));
    particle->phaseParameter = static_cast<int>(static_cast<float>(particle->rowIndex) * *reinterpret_cast<float*>(&this->rotationParameter));
    particle->alpha = *reinterpret_cast<int*>(this->alphaParameters);
    particle->sourceBlend = 5;
    particle->destinationBlend = 7;
    particle->InitParticleQuad(particle->vertices);
    particle->elapsedFrames = 0;
  }
}
// OFFSET: 0x006FDD90
// FUNCTION: fn_06940
bool CCommonFloorEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006FDDA0
// FUNCTION: fn_06941
void CCommonFloorEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      int angle = static_cast<int>(particle->speedParameter + *reinterpret_cast<float*>(&particle->angleBits));
      if (angle >= 360) {
        angle -= 360;
      } else if (angle < 0) {
        angle += 360;
      }

      const float degrees = static_cast<float>(angle);
      *reinterpret_cast<float*>(&particle->angleBits) = degrees;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(degrees / 180.0f * 3.141592f);
      rotation.AppendXRotation(1.570796f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;
    }
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x006FE060
// FUNCTION: fn_06942
void CCommonFloorEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CParticle* particle = *it) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
