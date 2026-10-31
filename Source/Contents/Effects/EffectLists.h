#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTLISTS_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Utility/Math/Vector3d.h"

class CRenderObject;

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
  bool Has(int actor, int id) const;
  bool HasAllocator(int id) const;
  void Add(int actor, CEffect* effect);
  void Retire(CEffect* effect, unsigned char removeActive);
  void Retire(int actor, int id);
  void Retire(int actor, int instance, int id);
  void RetireActor(int actor);
  void Render();
  void Clear();
  void Release();
  void SpawnLevel99(CRenderObject* actor, unsigned char flag, int extra);
  bool RegisterAllocators();
  bool RegisterAllocator(int id, CAllocator* allocator);
  void Update();
  void LoadMapEffectScript(bool reload);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectLists) == 0x28);
static_assert(offsetof(EffectLists, allocators) == 0x08);
static_assert(offsetof(EffectLists, active) == 0x10);
static_assert(offsetof(EffectLists, retired) == 0x18);
static_assert(offsetof(EffectLists, indexed) == 0x20);
#endif

bool CreateEffectMgr();
void DestroyEffectMgr();
float DirectionAngle(float x, float z);
double ScreenAngle(vector3d from, vector3d to);
std::string CurrentMapScriptName();
std::string EffectFunctionName(std::string name);
bool HasEffectScript(std::string path, std::string name);
void ReadEffectScriptV1(std::string name);
void ReadEffectScriptV2(std::string name);

extern EffectLists* g_effectMgr;

#endif
