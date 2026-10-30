#include "Contents/Effects/RainbowEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B70CB0
// FUNCTION: fn_19128
void CRainbowEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    FreeParticle(*it);
  }

  this->bands.clear();
}

// OFFSET: 0x00B70CF0
// FUNCTION: fn_19129
void CRainbowEffect::Slot01() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const int angle = (360 - static_cast<int>(mode->camera->camera.yaw)) % 360;
  for (int row = 0; row < 7; ++row) {
    for (int column = 0; column < 21; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = column;
        particle->unknown_0010 = row;
        particle->unknown_0008 = 1;
        particle->SetTexture("effect\\alpha_center.tga");
        particle->alpha = 0;
        particle->scale = 50.0f;
        particle->unknown_0050 = 180;
        particle->unknown_00C4 = 0;
        particle->unknown_004C = angle;
        InitParticleQuad(particle->vertices);
        this->bands.push_back(particle);
      }
    }
  }

  this->unknown_002C = 300;
}

// OFFSET: 0x00B70DF0
// FUNCTION: fn_19130
bool CRainbowEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B70E00

// OFFSET: 0x00B714C0
// FUNCTION: fn_19132
void CRainbowEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->bands.begin(); it != this->bands.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, 0.0f, 1.0f, false, true};
      particle->Render(quad);
    }
  }
}
