#include "Contents/Effects/ElectricEffect.h"
#include "Contents/Effects/Particle.h"

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
