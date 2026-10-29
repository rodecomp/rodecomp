#include "Contents/Effects/NyanggrassEffect.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/Vector3d.h"

#include <deque>
#include <new>

namespace {

class IEffectMemoryPool {
  public:
  virtual ~IEffectMemoryPool() {
  }

  virtual CParticle* Alloc() = 0;
  virtual void Free(CParticle* particle) = 0;
};

template <class T>
class CEffectMemoryPool : public IEffectMemoryPool {
  public:
  /* 0004 */ std::deque<T*> m_free;

  CParticle* Alloc() override;
  void Free(CParticle* particle) override;
};

// OFFSET: 0x00B81D00
// FUNCTION: fn_19267
template <>
CParticle* CEffectMemoryPool<CEZ2STRParticle>::Alloc() {
  if (!this->m_free.empty()) {
    CEZ2STRParticle* particle = this->m_free.front();
    particle->Clear();
    this->m_free.pop_front();
    return particle;
  }

  return new (std::nothrow) CEZ2STRParticle(vector3d(), -1);
}

// OFFSET: 0x00B81EE0
// FUNCTION: fn_19269
template <>
CParticle* CEffectMemoryPool<CParticle>::Alloc() {
  if (!this->m_free.empty()) {
    CParticle* particle = this->m_free.front();
    particle->Reset();
    this->m_free.pop_front();
    return particle;
  }

  return new (std::nothrow) CParticle(vector3d(), -1);
}

// OFFSET: 0x00B825B0
// OFFSET: 0x00B826D0
// OFFSET: 0x00B82640
// OFFSET: 0x00B82520
// FUNCTION: fn_19276
// FUNCTION: fn_19278
// FUNCTION: fn_19277
// FUNCTION: fn_19275
template <class T>
void CEffectMemoryPool<T>::Free(CParticle* particle) {
  this->m_free.push_back(static_cast<T*>(particle));
}

template class CEffectMemoryPool<CParticle>;
template class CEffectMemoryPool<CEZ2STRParticle>;
template void CEffectMemoryPool<CSpriteParticle>::Free(CParticle* particle);
template void CEffectMemoryPool<CMultiTexParticle>::Free(CParticle* particle);

}
