#include "Contents/Effects/BindTrapEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x006FB2F0
// FUNCTION: fn_06903
CBindTrapEffect::CBindTrapEffect() : CEffect(-1), particles() {
  this->unknown_002C = 8000;
}

// OFFSET: 0x006FB4E0
// FUNCTION: fn_06906
void CBindTrapEffect::AddParticle(int index) {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\hero_011.tga");
  particle->unknown_0004 = this->id;
  particle->unknown_000C = 0;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  particle->alpha = 255;
  particle->unknown_00C4 = 0;
  particle->unknown_004C = 0;
  particle->scale = static_cast<float>(index) * 6.0f + 9.0f;
  InitParticleQuad(particle->vertices);
  this->particles.push_back(particle);
}

// OFFSET: 0x006FB5B0
// FUNCTION: fn_06907
void CBindTrapEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x006FB5F0
// FUNCTION: fn_06908
void CBindTrapEffect::Slot01() {
  switch (this->id) {
  case 1069:
    this->AddParticle(0);
    break;
  case 1070:
    this->AddParticle(1);
    break;
  case 1071:
    this->AddParticle(2);
    break;
  }
}

// OFFSET: 0x006FB620
// FUNCTION: fn_06909
bool CBindTrapEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FB630
// FUNCTION: fn_06910
void CBindTrapEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x006FB8B0
// FUNCTION: fn_06911
void CBindTrapEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
