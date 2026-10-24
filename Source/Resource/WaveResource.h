#ifndef RAGNAROK_SOURCE_RESOURCE_WAVERESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WAVERESOURCE_H

#include "Resource/Resource.h"

#include <cstddef>

class ResourceBytes;
class ResourceMgr;

struct WaveResourceContext {
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
};

// RTTI CWave. Original size is 0x118;
// only the borrowed source context is appended (total 0x11C in x86).
// Destructor does NOT call Unload; ResourceMgr handles unloading.
class CWave : public CRes {
public:
  explicit CWave(WaveResourceContext& context);
  ~CWave() override;
  CWave* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ void* data;
  /* 0114 */ int milesAllocated;

private:
  /* 0118 */ WaveResourceContext* m_context;
};

// Registration portion of the wave resource initializer. The application owns the
// prototype and its context; both must outlive the manager and loaded waves.
// ResourceBytes still requires the original archive-aware read implementation.
void RegisterWaveResource(ResourceMgr& resourceMgr, CWave& prototype);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWave) == 284);
static_assert(offsetof(CWave, data) == 0x110);
static_assert(offsetof(CWave, milesAllocated) == 0x114);
#endif

#endif
