#include "Contents/Effects/ElectricEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B4E890

// OFFSET: 0x00B4EDB0
// FUNCTION: fn_19003
void CElectricEffect::Close() {
  for (auto it = this->bolts.begin(); it != this->bolts.end(); ++it) {
    FreeParticle(*it);
  }

  this->bolts.clear();
}

// OFFSET: 0x00B4EE80
// FUNCTION: fn_19004
void CElectricEffect::Slot01() {
  this->unknown_002C = 300;
}

// OFFSET: 0x00B4EF40
// FUNCTION: fn_19005
bool CElectricEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4EF50

// OFFSET: 0x00B4F930
// FUNCTION: fn_19007
void CElectricEffect::Slot04() {
  for (CParticle* bolt : this->bolts) {
    if (bolt && static_cast<unsigned>(bolt->unknown_000C - 1) <= 17 && bolt->unknown_0008 > 0) {
      const int frame = bolt->unknown_000C;
      const int alpha = bolt->alpha;
      const int faded = alpha - frame * 5;
      bolt->alpha = faded < 0 ? 0 : faded;
      bolt->SetColor(static_cast<short>(frame * 6), static_cast<short>(frame * 6), 255);
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = static_cast<float>(bolt->unknown_000C - 1) / 21.0f;
      quad.bottom = static_cast<float>(bolt->unknown_000C) / 21.0f;
      quad.left = 0.0f;
      quad.right = 1.0f;
      quad.unused = false;
      quad.depth = true;
      bolt->Render(quad);
      bolt->alpha = alpha;
    }
  }
}
