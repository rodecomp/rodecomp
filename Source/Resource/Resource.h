#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCE_H

#include "Archive/PakName.h"

#include <cstdint>

class CRenderSystemBase;

// The resource owner's available name region is256 bytes total, unlike the
// archive entry's260-byte reservation. Same unbounded assignment algorithm.
struct ResourceName {
  std::uint32_t hash;
  char text[252];
  explicit ResourceName(const char* name = "");
  void Assign(const char* name);
};
static_assert(sizeof(ResourceName) == 256);

// Resource factory/load
// are abstract; original unload remains a real empty base implementation.
class CRes {
public:
  CRes();
  virtual ~CRes();
  virtual bool Slot01(int argument1, int argument2, int argument3);
  virtual void Slot02();
  virtual bool IsTransient();
  virtual CRes* Create() = 0;
  virtual bool Load(const char* name) = 0;
  virtual void Unload();
  virtual void MissingFile(CRenderSystemBase* system, const char* name);
  std::uint32_t AddRef();
  void Release();

  /* 0004 */ std::uint32_t references;
  /* 0008 */ std::uint32_t lastUse;
  /* 000C */ int type;
  /* 0010 */ ResourceName name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRes) == 272);
#endif

#endif
