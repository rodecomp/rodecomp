#include "Contents/Effects/EnergyOrbEffect.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B50D10
// FUNCTION: fn_19022
CEnergyOrbEffect::CEnergyOrbEffect() : CEffect(-1), count(0), mode(2), particles() {
}

// OFFSET: 0x00B50E00
// FUNCTION: fn_19023
CEnergyOrbEffect::~CEnergyOrbEffect() {
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
