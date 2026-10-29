#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCE_H

#include "Archive/PakName.h"

#include <cstddef>
#include <cstdint>

class CRenderSystemBase;

struct ResourceName {
  /* 0000 */ std::uint32_t hash;
  /* 0004 */ char text[252];
  explicit ResourceName(const char* name = "");
  void Assign(const char* name);
};
static_assert(sizeof(ResourceName) == 256);

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
  std::uint32_t Lock();
  void Unlock();

  /* 0004 */ std::uint32_t references;
  /* 0008 */ std::uint32_t lastUse;
  /* 000C */ int type;
  /* 0010 */ ResourceName name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRes) == 272);
static_assert(offsetof(ResourceName, text) == 0x04);
#endif

#endif
