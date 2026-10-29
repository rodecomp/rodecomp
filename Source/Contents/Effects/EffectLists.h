#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <vector>

struct CAllocator {
  virtual ~CAllocator() = default;
  virtual CEffect* Create() = 0;
};

class EffectLists {
public:
  /* 0004 */ bool initialized;
  /* 0008 */ std::map<int, CAllocator*> allocators;
  /* 0010 */ std::map<int, std::list<CEffect*>> active;
  /* 0018 */ std::list<CEffect*> retired;
  /* 0020 */ std::map<int, std::vector<CEffect*>> indexed;

  virtual ~EffectLists();

  CEffect* Spawn(int actor, int id, float x, float y, float z, std::uint32_t sourceActor, int frame, unsigned char reuse, int instance);
  CEffect* Find(int actor, int id) const;
  CEffect* Find(int actor, int instance, int id) const;
  CEffect* At(int id, unsigned index) const;
  void Add(int actor, CEffect* effect);
  void Retire(CEffect* effect, unsigned char removeActive);
  void Retire(int actor, int id);
  void Retire(int actor, int instance, int id);
  void RetireActor(int actor);
  void Clear();
  void Release();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectLists) == 0x28);
static_assert(offsetof(EffectLists, allocators) == 0x08);
static_assert(offsetof(EffectLists, active) == 0x10);
static_assert(offsetof(EffectLists, retired) == 0x18);
static_assert(offsetof(EffectLists, indexed) == 0x20);
#endif

void DestroyEffectMgr();

extern EffectLists* g_effectMgr;

#endif
