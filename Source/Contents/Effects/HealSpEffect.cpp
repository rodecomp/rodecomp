#include "Contents/Effects/HealSpEffect.h"

#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B46230
// FUNCTION: fn_18905
void CHealSp::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int segment = particle->unknown_000C;
    const float height = particle->scale;
    const float outer = *reinterpret_cast<const float*>(particle->unknown_0058);
    const float inner = particle->unknown_0054;
    const float startCos = g_cosTable[segment * 36];
    const float startSin = g_sinTable[segment * 36];
    const float endCos = g_cosTable[segment * 36 + 36];
    const float endSin = g_sinTable[segment * 36 + 36];

    particle->vertices[0].x = startCos * outer;
    particle->vertices[0].y = 0.0f;
    particle->vertices[0].z = startSin * outer;
    particle->vertices[1].x = endCos * outer;
    particle->vertices[1].y = 0.0f;
    particle->vertices[1].z = endSin * outer;
    particle->vertices[2].x = startCos * inner;
    particle->vertices[2].y = -height;
    particle->vertices[2].z = startSin * inner;
    particle->vertices[3].x = endCos * inner;
    particle->vertices[3].y = -height;
    particle->vertices[3].z = endSin * inner;

    const float left = static_cast<float>(particle->unknown_000C % 4) * 0.25f;
    const ParticleQuad quad = {false, 1.0f, 0.0f, left, left + 0.25f, false, true};
    particle->Render(quad);
  }

  for (std::list<CSpriteParticle*>::iterator it = this->sprites.begin(); it != this->sprites.end(); ++it) {
    (*it)->sub_B40F30(1);
  }
}
