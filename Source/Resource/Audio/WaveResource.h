#ifndef RAGNAROK_SOURCE_RESOURCE_AUDIO_WAVERESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_AUDIO_WAVERESOURCE_H

#include "Resource/Resource.h"

#include <cstddef>

class ResourceMgr;

class CWave : public CRes {
public:
  CWave();
  ~CWave() override;
  CWave* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ void* data;
  /* 0114 */ int milesAllocated;
};

void RegisterWaveResource(ResourceMgr& resourceMgr, CWave& prototype);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWave) == 0x118);
static_assert(offsetof(CWave, data) == 0x110);
static_assert(offsetof(CWave, milesAllocated) == 0x114);
#endif

#endif
