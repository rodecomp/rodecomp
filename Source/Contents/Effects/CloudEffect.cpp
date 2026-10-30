#include "Contents/Effects/CloudEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// OFFSET: 0x006FB9A0

// OFFSET: 0x006FBBA0
// FUNCTION: fn_06916
void CCloud::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }
}

// OFFSET: 0x006FBBD0

// OFFSET: 0x006FBC30

// OFFSET: 0x006FBEC0
// FUNCTION: fn_06919
bool CCloud::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x006FBED0

// OFFSET: 0x006FC090
// FUNCTION: fn_06921
void CCloud::Slot04() {
  if (this->particle) {
    this->particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x006FC160
