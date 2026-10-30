#include "Contents/Effects/Level150Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x00B463F0
// FUNCTION: fn_18906
CLevel150Effect::CLevel150Effect() : CEffect(-1) {
}

// OFFSET: 0x00B465A0
// FUNCTION: fn_18908
void CLevel150Effect::Slot10(int blend) {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }
}

// OFFSET: 0x00B46600
// FUNCTION: fn_18909
void CLevel150Effect::Slot08(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->bubbles.begin(); it != this->bubbles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }

  for (std::vector<CParticle*>::iterator it = this->shocks.begin(); it != this->shocks.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B46680

// OFFSET: 0x00B469F0
// FUNCTION: fn_18911
void CLevel150Effect::AddShock(int kind) {
  const int effectId = this->id;
  for (int index = 0; index < 2; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 13);
    if (particle) {
      particle->unknown_0004 = this->id;
      particle->unknown_000C = index;
      particle->unknown_0010 = 0;
      particle->unknown_0008 = 1;
      particle->unknown_004C = index * 180;
      particle->alpha = 200;
      particle->unknown_0054 = -1.0f;
      particle->unknown_00E8 = 0;

      if (kind == 3) {
        particle->SetTexture("effect\\cir0002.tga");
        *reinterpret_cast<float*>(particle->unknown_0058) = 18.0f;
      } else {
        particle->SetTexture("effect\\emp shock.tga");
        *reinterpret_cast<float*>(particle->unknown_0058) = 11.0f;
      }

      this->shocks.push_back(particle);

      if (effectId == 978) {
        if (kind == 3) {
          particle->SetColor(35, 35, 255);
        } else {
          particle->SetColor(105, 105, 255);
        }
      } else if (effectId == 1022) {
        if (kind == 3) {
          particle->SetColor(255, 255, 35);
        } else {
          particle->SetColor(255, 255, 105);
        }
      } else if (effectId == 1226) {
        if (kind == 3) {
          particle->SetColor(255, 55, 55);
        } else {
          particle->SetColor(255, 155, 155);
        }
      } else if (effectId == 2275) {
        particle->SetColor(255, 155, 0);
      }
    }
  }
}

// OFFSET: 0x00B46D10
// FUNCTION: fn_18912
void CLevel150Effect::Close() {
  for (CParticle* particle : this->bubbles) {
    FreeParticle(particle);
  }

  this->bubbles.clear();

  for (CParticle* particle : this->shocks) {
    FreeParticle(particle);
  }

  this->shocks.clear();
}

// OFFSET: 0x00B46D80
// FUNCTION: fn_18913
void CLevel150Effect::Slot01() {
  this->AddBubbleRing(45);
  this->AddBubbleRing(25);
  this->AddShock(2);
  this->AddShock(3);
}

// OFFSET: 0x00B46DB0
// FUNCTION: fn_18914
bool CLevel150Effect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B46F00

// OFFSET: 0x00B474C0

// OFFSET: 0x00B47670
// FUNCTION: fn_18917
CLevel150SubEffect::CLevel150SubEffect() : CEffect(-1) {
}

// OFFSET: 0x00B477D0
// FUNCTION: fn_18919
void CLevel150SubEffect::Slot10(int blend) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (blend != -1) {
      (*it)->destinationBlend = blend;
    }
  }
}

// OFFSET: 0x00B47810
// FUNCTION: fn_18920
void CLevel150SubEffect::Slot08(int red, int green, int blue) {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->SetColor(red, green, blue);
  }
}

// OFFSET: 0x00B47850

// OFFSET: 0x00B47A90
// FUNCTION: fn_18922
void CLevel150SubEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B47AD0
// FUNCTION: fn_18923
void CLevel150SubEffect::Slot01() {
  this->Init();
}

// OFFSET: 0x00B47AE0
// FUNCTION: fn_18924
bool CLevel150SubEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B47AF0

// OFFSET: 0x00B47FD0
// FUNCTION: fn_18926
void CLevel150SubEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}
