#include "Contents/Effects/NyanggrassEffect.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/Vector3d.h"

#include <deque>
#include <new>
#include <vector>

#include <windows.h>

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

}

class CEffectMemoryMgr {
  public:
  virtual ~CEffectMemoryMgr();

  bool Init();

  /* 0004 */ bool m_initialized = false;
  /* 0008 */ std::vector<IEffectMemoryPool*> m_pools;
};

// GLOBAL: gl_24796
CEffectMemoryMgr* g_effectMemoryMgr;

namespace {

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

// OFFSET: 0x00B81DF0
// FUNCTION: fn_19268
template <>
CParticle* CEffectMemoryPool<CMultiTexParticle>::Alloc() {
  if (!this->m_free.empty()) {
    CMultiTexParticle* particle = this->m_free.front();
    particle->Reset();
    this->m_free.pop_front();
    return particle;
  }

  return new (std::nothrow) CMultiTexParticle(vector3d(), -1);
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

// OFFSET: 0x00B81FD0
// FUNCTION: fn_19270
template <>
CParticle* CEffectMemoryPool<CSpriteParticle>::Alloc() {
  if (!this->m_free.empty()) {
    CSpriteParticle* particle = this->m_free.front();
    particle->Reset();
    this->m_free.pop_front();
    return particle;
  }

  return new (std::nothrow) CSpriteParticle(vector3d(), -1);
}

}

// OFFSET: 0x00B820E0
// FUNCTION: fn_19272
bool CEffectMemoryMgr::Init() {
  if (this->m_pools.size() != 0) {
    MessageBoxA(NULL, "Input sequence is incorrect.", "CEffectMemoryMgr error", 0);
    return false;
  }

  this->m_pools.push_back(new CEffectMemoryPool<CParticle>());
  if (this->m_pools.size() != 1) {
    MessageBoxA(NULL, "Input sequence is incorrect.", "CEffectMemoryMgr error", 0);
    return false;
  }

  this->m_pools.push_back(new CEffectMemoryPool<CSpriteParticle>());
  if (this->m_pools.size() != 2) {
    MessageBoxA(NULL, "Input sequence is incorrect.", "CEffectMemoryMgr error", 0);
    return false;
  }

  this->m_pools.push_back(new CEffectMemoryPool<CEZ2STRParticle>());
  if (this->m_pools.size() != 3) {
    MessageBoxA(NULL, "Input sequence is incorrect.", "CEffectMemoryMgr error", 0);
    return false;
  }

  this->m_pools.push_back(new CEffectMemoryPool<CMultiTexParticle>());
  return true;
}

// OFFSET: 0x00B823F0
bool CreateEffectMemoryMgr() {
  if (g_effectMemoryMgr != NULL) {
    return false;
  }

  CEffectMemoryMgr* mgr = new (std::nothrow) CEffectMemoryMgr;
  if (mgr != NULL && mgr->Init()) {
    mgr->m_initialized = true;
  }

  g_effectMemoryMgr = mgr;

  if (mgr == NULL) {
    return false;
  }

  if (mgr->m_initialized) {
    return true;
  }

  delete mgr;
  g_effectMemoryMgr = NULL;
  return false;
}

namespace {

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
