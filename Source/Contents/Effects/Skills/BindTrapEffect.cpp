#include "Contents/Effects/Skills/BindTrapEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x006FB2F0
// FUNCTION: fn_06903
CBindTrapEffect::CBindTrapEffect() : CEffect(-1), particles() {
  this->effectLifetime = 8000;
}

// OFFSET: 0x006FB4E0
// FUNCTION: fn_06906
void CBindTrapEffect::AddParticle(int index) {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\hero_011.tga");
  particle->effectId = this->id;
  particle->columnIndex = 0;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  particle->alpha = 255;
  particle->angle = 0;
  particle->phaseParameter = 0;
  particle->scale = static_cast<float>(index) * 6.0f + 9.0f;
  particle->InitParticleQuad(particle->vertices);
  this->particles.push_back(particle);
}

// OFFSET: 0x006FB5B0
// FUNCTION: fn_06907
void CBindTrapEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x006FB5F0
// FUNCTION: fn_06908
void CBindTrapEffect::Open() {
  switch (this->id) {
  case EF_BIND_TRAP1:
    this->AddParticle(0);
    break;
  case EF_BIND_TRAP2:
    this->AddParticle(1);
    break;
  case EF_BIND_TRAP3:
    this->AddParticle(2);
    break;
  }
}

// OFFSET: 0x006FB620
// FUNCTION: fn_06909
bool CBindTrapEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x006FB630
// FUNCTION: fn_06910
void CBindTrapEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(1.570796f);
      rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / -180.0f * 3.141592f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix composed;
      composed.Compose(scale, rotation);
      particle->transform = composed;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x006FB8B0
// FUNCTION: fn_06911
void CBindTrapEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
