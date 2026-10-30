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

// OFFSET: 0x00B465A0

// OFFSET: 0x00B46600

// OFFSET: 0x00B46680

// OFFSET: 0x00B469F0

// OFFSET: 0x00B46D10

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

// OFFSET: 0x00B477D0

// OFFSET: 0x00B47810

// OFFSET: 0x00B47850

// OFFSET: 0x00B47A90

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
