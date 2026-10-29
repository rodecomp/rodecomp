#include "Contents/Effects/FriggSongEffect.h"

#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x00701BB0
CFriggSongEffect::CFriggSongEffect() : CEffect(0x423), lastInner{} {
  this->unknown_002C = EffectLifetime(0x423);
}

// OFFSET: 0x007021A0
void CFriggSongEffect::Slot12() {
  this->AddEvent(0, "effect\\wm_frigg_song.wav", 0, 0);
}

// OFFSET: 0x007027A0
void CFriggSongEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f, static_cast<float>(texPart) / 21.0f, false, true};
    particle->Render(quad);
  }
}
