#include "Contents/Effects/EnergyOrbEffect.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B50D10
// FUNCTION: fn_19022
CEnergyOrbEffect::CEnergyOrbEffect() : CEffect(-1), count(0), mode(2), particles() {
}

// OFFSET: 0x00B50E00
// FUNCTION: fn_19023
CEnergyOrbEffect::~CEnergyOrbEffect() {
}

// OFFSET: 0x00B50E80
// FUNCTION: fn_19024
void CEnergyOrbEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B50EC0
// FUNCTION: fn_19025
void CEnergyOrbEffect::Slot01() {
  for (int i = 0; i < this->count; ++i) {
    const int angle = std::max(rand() % 360, 0);
    const int phase = std::max(rand() % 3, 0);

    for (int j = 0; j < this->mode; ++j) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_0010 = i;
      particle->unknown_000C = j;
      particle->unknown_0008 = 1;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(360 / this->count * i);
      particle->SetTexture("effect\\thunder_center.bmp");
      particle->alpha = 0;
      particle->scale = 0.8f;
      particle->unknown_00C0 = 0.15f;
      particle->unknown_0054 = 8.0f;
      particle->unknown_004C = angle;
      *reinterpret_cast<float*>(&particle->unknown_0058[0]) = static_cast<float>(phase);
      *reinterpret_cast<float*>(&particle->unknown_0058[4]) = -12.0f;
      if (j == 0) {
        particle->destinationBlend = 6;
      }

      switch (this->id) {
      case 228:
        particle->SetColor(particle->unknown_000C != 0 ? -1 : 0, particle->unknown_000C != 0 ? -1 : 0, particle->unknown_000C != 0 ? 0 : 255);
        break;
      case 735:
        particle->SetTexture("effect\\FireOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 736:
        particle->SetTexture("effect\\LightningOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 737:
        particle->SetTexture("effect\\WaterOrb.bmp");
        particle->scale = 1.3f;
        break;
      case 738:
        particle->SetTexture("effect\\StoneOrb.bmp");
        particle->scale = 2.3f;
        break;
      case 1005:
        particle->SetTexture("effect\\fog1.tga");
        particle->scale = 6.0f;
        particle->unknown_00C0 = 0.05f;
        particle->unknown_0054 = 4.0f;
        *reinterpret_cast<float*>(&particle->unknown_0058[0]) = 0.0f;
        *reinterpret_cast<float*>(&particle->unknown_0058[4]) = -8.0f;
        particle->destinationBlend = 6;
        break;
      }

      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B51240
// FUNCTION: fn_19026
bool CEnergyOrbEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B51750
// FUNCTION: fn_19028
void CEnergyOrbEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00B517F0
// FUNCTION: fn_19029
void CEnergyOrbEffect::SetCount(int value) {
  if (this->count != value) {
    this->count = value;
    this->Slot01();
  }
}
