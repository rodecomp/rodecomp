#include "Contents/Effects/EffectMemoryMgr.h"
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
  // INLINED
  CEffectMemoryMgr() : m_initialized(false) {
    if (this->Init()) {
      this->m_initialized = true;
    }
  }

  // INLINED
  virtual ~CEffectMemoryMgr() {
    for (auto it = this->m_pools.begin(); it != this->m_pools.end(); ++it) {
      delete *it;
    }
  }

  bool Init();
  CParticle* Alloc(int type);
  void Free(int type, CParticle* particle);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::vector<IEffectMemoryPool*> m_pools;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEffectMemoryMgr) == 0x14);
static_assert(offsetof(CEffectMemoryMgr, m_pools) == 0x8);
#endif

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
// FUNCTION: fn_19273
bool CreateEffectMemoryMgr() {
  if (g_effectMemoryMgr != NULL) {
    return false;
  }

  CEffectMemoryMgr* mgr = new (std::nothrow) CEffectMemoryMgr;

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

// OFFSET: 0x00B820C0
// FUNCTION: fn_19271
CParticle* CEffectMemoryMgr::Alloc(int type) {
  return this->m_pools[type]->Alloc();
}

// OFFSET: 0x00B82500
// FUNCTION: fn_19274
void DestroyEffectMemoryMgr() {
  if (g_effectMemoryMgr) {
    delete g_effectMemoryMgr;
    g_effectMemoryMgr = nullptr;
  }
}

// OFFSET: 0x00B82760
// FUNCTION: fn_19279
void CEffectMemoryMgr::Free(int type, CParticle* particle) {
  this->m_pools[type]->Free(particle);
}

// OFFSET: 0x00B82ED0
// FUNCTION: fn_19290
CParticle* CreateEZ2STRParticle(const vector3d* position, int flags) {
  CParticle* particle = g_effectMemoryMgr->Alloc(2);
  particle->unknown_02B0 = 2;
  particle->position = *position;
  particle->flags = flags;
  return particle;
}

// OFFSET: 0x00B82F10
// FUNCTION: fn_19291
CParticle* CreateMultiTexParticle(const vector3d* position, int flags) {
  CParticle* particle = g_effectMemoryMgr->Alloc(3);
  particle->unknown_02B0 = 3;
  particle->position = *position;
  particle->flags = flags;
  return particle;
}

// OFFSET: 0x00B82F50
// FUNCTION: fn_19292
CParticle* CreateParticle(const vector3d* position, int flags) {
  CParticle* particle = g_effectMemoryMgr->Alloc(0);
  particle->unknown_02B0 = 0;
  particle->position = *position;
  particle->flags = flags;
  return particle;
}

// OFFSET: 0x00B82F90
// FUNCTION: fn_19293
CParticle* CreateSpriteParticle(const vector3d* position, int flags) {
  CParticle* particle = g_effectMemoryMgr->Alloc(1);
  particle->unknown_02B0 = 1;
  particle->position = *position;
  particle->flags = flags;
  return particle;
}

// OFFSET: 0x00B82FD0
// FUNCTION: fn_19294
void FreeParticle(CParticle* particle) {
  if (particle) {
    particle->Slot01();
  }
  g_effectMemoryMgr->Free(particle->unknown_02B0, particle);
}
