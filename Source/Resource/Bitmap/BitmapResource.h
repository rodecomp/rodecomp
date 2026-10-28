#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPRESOURCE_H

#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Resource.h"

#include <cstddef>

class IJL15Runtime;

class ResourceBytes {
public:
  virtual bool Open(const char* name) = 0;
  virtual const void* Data() const = 0;
  virtual int Size() const = 0;
  virtual void Close() = 0;

protected:
  ~ResourceBytes() = default;
};

struct BitmapResourceContext {
  ResourceBytes& file;
  IJL15Runtime& jpeg;
  CRenderSystemBase* renderSystem;
};

class CBitmapRes : public CRes {
public:
  explicit CBitmapRes(BitmapResourceContext& context);
  ~CBitmapRes() override;
  CBitmapRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  virtual bool Decode(const char* name, const void* data, int bytes, bool iconOnly);
  // INLINED
  BitmapResourceContext& Context() const {
    return *this->m_context;
  }

  /* 0110 */ BitmapImage image;

protected:
  BitmapResourceContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBitmapRes) == 296);
static_assert(offsetof(CBitmapRes, image) == 0x0110);
#endif

#endif
