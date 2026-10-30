#include "Contents/Effects/AnimatedEmitterEffect.h"

#include "Contents/Effects/Particle.h"

#include <cstdlib>

// OFFSET: 0x006FA0F0
// FUNCTION: fn_06890
void CAnimatedEmitterEffect::Slot01() {
  this->frames[0].push_back(std::string("EFFECT\\shockwave_a.bmp"));
  this->frames[0].push_back(std::string("EFFECT\\shockwave_b.bmp"));
  this->frames[0].push_back(std::string("EFFECT\\shockwave_c.bmp"));
  this->frames[0].push_back(std::string("EFFECT\\shockwave_d.bmp"));
  this->frames[0].push_back(std::string("EFFECT\\shockwave_e.bmp"));
  this->frames[1].push_back(std::string("EFFECT\\plazma_a.bmp"));
  this->frames[1].push_back(std::string("EFFECT\\plazma_b.bmp"));
  this->frames[1].push_back(std::string("EFFECT\\plazma_c.bmp"));
  this->frames[1].push_back(std::string("EFFECT\\plazma_a.bmp"));
  this->frames[1].push_back(std::string("EFFECT\\plazma_b.bmp"));
  CEmitterEffect::Slot01();
}

// OFFSET: 0x006FA7C0
// FUNCTION: fn_06891
void CAnimatedEmitterEffect::InitParticle(CParticle* particle) {
  particle->unknown_0054 = static_cast<float>(rand() % 2);
  CEmitterEffect::InitParticle(particle);
}

// OFFSET: 0x006FA800
// FUNCTION: fn_06892
void CAnimatedEmitterEffect::UpdateParticle(CParticle* particle) {
  particle->SetTexture(this->frames[static_cast<int>(particle->unknown_0054)][particle->unknown_00E8 / 8 % 5]);
  CEmitterEffect::UpdateParticle(particle);
}
