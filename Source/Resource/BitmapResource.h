#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAPRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAPRESOURCE_H

#include "Resource/BitmapImage.h"
#include "Resource/Resource.h"

class IJL15Runtime;

// Exact original file-owner boundary: bytes come from CFile's archive-aware
// read path, not a fallback that silently ignores archive lookup precedence.
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

// RTTI CBitmapRes. Source-only context pointer follows all original fields.
class CBitmapRes : public CRes {
public:
  explicit CBitmapRes(BitmapResourceContext& context);
  ~CBitmapRes() override;
  CBitmapRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  virtual bool Decode(const char* name, const void* data, int bytes, bool iconOnly);

  BitmapImage image;

private:
  BitmapResourceContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBitmapRes) == 296);
#endif

#endif
