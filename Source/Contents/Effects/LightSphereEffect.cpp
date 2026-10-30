#include "Contents/Effects/LightSphereEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B6C9F0

// OFFSET: 0x00B6D160

// OFFSET: 0x00B6D1C0

// OFFSET: 0x00B6D990
// FUNCTION: fn_19110
void CLightSphereEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->spheres.begin(); it != this->spheres.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (std::vector<CParticle*>::iterator it = this->rays.begin(); it != this->rays.end(); ++it) {
    CParticle* particle = *it;

    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
