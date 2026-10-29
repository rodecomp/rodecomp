#include "Contents/Effects/ParticlePool.h"

// OFFSET: 0x00B402F0
// FUNCTION: fn_18849
ParticlePool::ParticlePool() {
  for (auto& entry : this->particles) {
    entry.second = false;
  }
  this->active = 0;
}

// OFFSET: 0x00B40560
// FUNCTION: fn_18851
ParticlePool::~ParticlePool() {
  this->Clear();
}

// OFFSET: 0x00B40740
// FUNCTION: fn_18854
CParticle* ParticlePool::Acquire() {
  if (this->particles.size() <= this->active) {
    return nullptr;
  }

  CParticle* particle = nullptr;
  auto it = this->particles.begin();
  for (; it != this->particles.end(); ++it) {
    if (!it->second) {
      break;
    }
  }

  if (it != this->particles.end()) {
    particle = it->first;
    it->second = true;
    ++this->active;
  }

  return particle;
}

// OFFSET: 0x00B407A0
// FUNCTION: fn_18855
void ParticlePool::Clear() {
  this->particles.clear();
  this->active = 0;
}

// OFFSET: 0x00B407F0
// FUNCTION: fn_18856
void ParticlePool::Release(CParticle* particle) {
  if (this->particles.find(particle) != this->particles.end() && this->particles[particle]) {
    --this->active;
    this->particles[particle] = false;
  }
}

// OFFSET: 0x00B40890
// FUNCTION: fn_18857
bool ParticlePool::IsActive(CParticle* particle) {
  if (this->particles.find(particle) != this->particles.end()) {
    return this->particles[particle];
  }

  return false;
}

// OFFSET: 0x00B408E0
// FUNCTION: fn_18858
void ParticlePool::ActivateAll() {
  for (auto& entry : this->particles) {
    entry.second = true;
  }
  this->active = this->particles.size();
}

// OFFSET: 0x00B40B20
// FUNCTION: fn_18860
void ParticlePool::Add(CParticle* particle) {
  if (this->particles.find(particle) == this->particles.end()) {
    this->particles[particle] = false;
  }
}
