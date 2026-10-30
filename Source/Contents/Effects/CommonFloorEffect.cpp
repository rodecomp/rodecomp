#include "Contents/Effects/CommonFloorEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x006FD760
// FUNCTION: fn_06933
CCommonFloorEffect::CCommonFloorEffect() : CEffect(1056) {
}

// OFFSET: 0x006FD8C0
// FUNCTION: fn_06935
void CCommonFloorEffect::Slot08(int red, int green, int blue) {
  for (CParticle* particle : this->particles) {
    particle->SetColor(red, green, blue);
  }
}

// OFFSET: 0x006FD900
// FUNCTION: fn_06936
void CCommonFloorEffect::Slot11(std::string name) {
  for (CParticle* particle : this->particles) {
    particle->SetTexture(name);
  }
}

// OFFSET: 0x006FDB30
// FUNCTION: fn_06937
void CCommonFloorEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FDB70
// FUNCTION: fn_06938
void CCommonFloorEffect::Slot01() {
  for (int i = 0; i < 2; ++i) {
    const int angle = i * 180;
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (particle) {
      particle->SetTexture("effect\\ra_unlimit.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_0008 = 1;
      particle->unknown_000C = i;
      particle->unknown_0010 = 0;
      particle->alpha = 90;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
      particle->unknown_004C = angle;
      particle->unknown_00C0 = 6.0f;
      particle->scale = 15.0f;
      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x006FDCE0
// FUNCTION: fn_06939
void CCommonFloorEffect::Slot07() {
  for (CParticle* particle : this->particles) {
    particle->flags = 5;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    particle->scale = *reinterpret_cast<float*>(&this->unknown_0048);
    *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(*reinterpret_cast<int*>(this->unknown_004C));
    particle->unknown_004C = static_cast<int>(static_cast<float>(particle->unknown_0010) * *reinterpret_cast<float*>(&this->unknown_0050));
    particle->alpha = *reinterpret_cast<int*>(this->unknown_0054);
    particle->sourceBlend = 5;
    particle->destinationBlend = 7;
    InitParticleQuad(particle->vertices);
    particle->unknown_00E8 = 0;
  }
}

// OFFSET: 0x006FDD90
// FUNCTION: fn_06940
bool CCommonFloorEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FDDA0
// FUNCTION: fn_06941
void CCommonFloorEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      int angle = static_cast<int>(particle->unknown_00C0 + *reinterpret_cast<float*>(&particle->unknown_00C4));
      if (angle >= 360) {
        angle -= 360;
      } else if (angle < 0) {
        angle += 360;
      }

      const float degrees = static_cast<float>(angle);
      *reinterpret_cast<float*>(&particle->unknown_00C4) = degrees;

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
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x006FE060
// FUNCTION: fn_06942
void CCommonFloorEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
