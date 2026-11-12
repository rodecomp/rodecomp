#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPRESOURCE_H

#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Resource.h"

#include <cstddef>

class ResourceBytes {
public:
  virtual bool Open(const char* name) = 0;
  virtual const void* Data() const = 0;
  virtual int Size() const = 0;
  virtual void Close() = 0;

protected:
  ~ResourceBytes() = default;
};

class CBitmapRes : public CRes {
public:
  CBitmapRes();
  CBitmapRes(const char* extension, const char* prefix);
  ~CBitmapRes() override;
  CBitmapRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  virtual bool Decode(const char* name, const void* data, int bytes, bool iconOnly);
  bool CreateAuxiliary(int mode);
  std::uint32_t Pixel(int x, int y) const;
  void ClearAuxiliary();
  bool DecodeBitmap(const void* data, int bytes, bool iconOnly);
  bool DecodeTga(const void* data, int bytes);
  bool DecodePng(const void* data, int bytes, const char* name);
  bool DecodeJpeg(const void* data, int bytes);

  /* 0110 */ BitmapImage image;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBitmapRes) == 292);
static_assert(offsetof(CBitmapRes, image) == 0x0110);
#endif

#endif
